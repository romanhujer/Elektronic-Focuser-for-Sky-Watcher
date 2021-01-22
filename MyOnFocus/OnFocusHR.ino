/*
 * Title       MyOnFocus
 * by          Roman Hujer
 * on base     OnFocus 
 * by          Howard Dutton 
 *
 * Copyright (C) 2015 Howard Dutton
 * Copyright (C) 2021 Roman Hujer
 * 
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * 
 *
 * Revision History
 *
 * Date                Version           Comment
 * 06-17-2015          1.0a1             First release
 * 06-24-2015          1.0a2             Added ability to set logic LOW or HIGH for enabling/disabling the stepper driver, pulls microstep mode control lines M1,M2,M3 LOW for full-step mode
 * 01-04-2021          2.0a              Modify for 28BYJ-48 Motor
 * 
*/

#define FirmwareNumber "2.0a"
#define FirmwareName   "On-Focus"

// --------------------------------------------------------------------------------------------
// Settings
#define CHKSUM0_OFF            // checksum, normally not used
 
#define MICROS_PER_STEP 10 // microns (1/1000 of a mm) of focus travel per step
                               // my Meade 8" SCT has about 32tpi on the focuser knob
                               // so... 1/32" per rotation (0.793mm)
                               // 20:1 reduced 24 stepper motor has 480 steps per rotation
                               // 20 tooth motor pulley and 60 tooth focuser pully for a 3:1 reduction
                               // 480*3 = 1440 steps per focuser rotation
                               // 0.793mm/1440 = 0.551 microns
                               // now the SCT has a f2 primary and a final focal ratio of f10 so
                               // I expect that the actual focal plane moves by approximately 5x
                               // the amount that the primary mirror moves... 0.551 * 5 = 2.756 microns
                               // or about 4mm per focuser knob turn, which is about 5/32" (0.156".)
                               // This also works out to about 0.1 thousandths of an inch per step.
 
#define MaxRate 1             // milliseconds per step (default 10, 0.01 seconds)
                               // how fast the focus moves... 1440*0.002=2.9 seconds per turn
                               // this is about 18 seconds for one inch of travel
                               // movement starts and stops at 8x slower than this
// --------------------------------------------------------------------------------------------

// for stepper motor control via step/dir type stepper driver attached to the pins defined below
// the module's enable pin should also be wired in to allow the motor to be powered down
// when not moving.  For this to work properly you must not micro-step the motor (use 1X mode.)
// I used a Big Easy Driver (A4988,) but a DRV8825, etc. would also work...


#include "Config.h"
#include "Setup.h"


#include <EEPROM.h>

// Misc. global variables
long startPos = 0;                  // where we were
long targetPos = 0;                 // where we want to be
long currentPos = 0;                // where we are
long maxPos = 0;                    // furthest step position allowed
long thisRate = MaxRate*8;          // current rate

boolean moving = false;             // are we moving?
boolean invalid_pos_warning=false;  // powered off while moving? 
boolean focuser_dir_out;            // direction of travel

long lastRun;                       // helps time steps

//  EEPROM addresses/offsets for permanent storage
long base=100;
#define EE_key 0        // 4 bytes
#define EE_base 10      // 4 bytes
#define EE_target 0     // 4 bytes
#define EE_current 4    // 4 bytes
#define EE_max 8        // 4 bytes
#define EE_moving 12    // 1 byte

void setup()
{
  // start serial port at 9600 bps
  Serial.begin(9600);

  pinMode(led_pin,  OUTPUT);
  pinMode(BuzzerPin, OUTPUT);
  
  MotorInit();
  buzzer(200);
  focuser_dir_out=false;
  // read the settings
  long key=EEPROM_readLong(EE_key);
  if (key!=1930230196) {
    EEPROM_writeLong(EE_key,1930230196);
    EEPROM_writeLong(EE_base,100); base=100;

    EEPROM_writeLong(base+EE_target,targetPos);
    EEPROM_writeLong(base+EE_current,currentPos);
    EEPROM_writeLong(base+EE_max,maxPos);
    EEPROM.write(base+EE_moving,(byte)moving);
  } else {
    base      =EEPROM_readLong(EE_base);           // only written once per bootup
    targetPos =EEPROM_readLong(base+EE_target);    // written each time a new position is requested
    currentPos=EEPROM_readLong(base+EE_current);
    maxPos    =EEPROM_readLong(base+EE_max);
    moving    =(boolean)EEPROM.read(base+EE_moving);
    
    // move base to the next position, with these rolling writes the EEPROM should be good for >10 million focuser moves
    base+=1; if (base>1000) base=100; EEPROM_writeLong(EE_base,base);
    EEPROM_writeLong(base+EE_target,targetPos);
    EEPROM_writeLong(base+EE_current,currentPos);
    EEPROM_writeLong(base+EE_max,maxPos);
    EEPROM.write(base+EE_moving,(byte)moving);
  }

  // the value of moving serves to validate the last stored position
  if (moving) invalid_pos_warning=true;
  
  // get time
  lastRun=millis();
}

void loop()
{
  // watch for any commands
  processCommands();

  long m=millis();
  if (m>=lastRun+thisRate) {
    lastRun=m;

    // watch for movement
    if (currentPos!=targetPos) {

      // record status, moving
      if (!moving) { moving=true; EEPROM.write(base+EE_moving,(byte)moving); }

      // control accelleration
      long distanceEnd  =abs(targetPos-currentPos);
      long distanceStart=abs(startPos-currentPos);
      double dist;
      if (distanceEnd>distanceStart) dist=sqrt(distanceStart); else dist=sqrt(distanceEnd); // scale speed by the lesser distance
      if (dist>8.0) dist=8.0;           // limit speed to 8x slower
      thisRate=MaxRate*round(9.0-dist); // speed range is 1x to 8x...
                                        // 1x (8, fast for large distance from start and target)
                                        // 8x (1, slow for small distance to start or target)

      // move out
      if (currentPos>targetPos) {
        if (!focuser_dir_out ) {
          focuser_dir_out=true; delay(1);
          
        } else {
         currentPos--;
         motor_step( focuser_dir_out );     
         MyLedChangeStatus( );
        }
      }
      
      // move in
      if (currentPos<targetPos) {
        if (focuser_dir_out ) {
          focuser_dir_out=false; delay(1);
        } else {
          currentPos++;
          motor_step( focuser_dir_out );
          MyLedChangeStatus( );
          
        }
      }
  
    } else {
       // remember start pos
      startPos=currentPos;
      MyLED ( OFF );
      // record status, not moving
      if (moving) { 
        buzzer(100);
        moving=false; 
        EEPROM_writeLong(base+EE_target,targetPos);
        EEPROM_writeLong(base+EE_current,currentPos);
        EEPROM.write(base+EE_moving,(byte)moving); 
      }

      // slow movement
      thisRate=MaxRate*8;
      
    }
  }
}
