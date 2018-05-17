/*
   MyFocuer.ino

   Copyright (c) 2018 Roman Hujer   http://hujer.net

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,ss
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

  Description:
   Focucer for OnStep controller and Sky-Watcher 150/750mm Telescope

   Wiring and PCB on  https://easyeda.com/hujer.roman/myfocuser-for-onstep

   More info about OnStep Controller on https://groups.io/g/onstep/wiki/home


*/

#define Product "MyFocuserForOnStep"
#define Version "1.0b"

#include "Config.h"
#include "Setup.h"


#ifdef LED_DISPLAY_ON
#include <TM1637Display.h>  //https://github.com/avishorp/TM1637
TM1637Display LedDisp(LedDispClk, LedDispDio);
#endif

#ifdef OLED_DISPLAY_ON
#include <U8glib.h>
U8GLIB_SSD1306_128X64 OledDisp(U8G_I2C_OPT_NONE);
#endif


int LastModeSensorValue = 0;
long int CurrentStep = 0;
long int LastDisp = 0;
long int CurrentDisp = 0;

void setup() {

#ifdef DEBUG_ON
  Serial.begin(SERIAL_BAUD);
  Serial.print(".");
  delay(500);
  Serial.println(".");
  delay(500);
  Serial.println("Debug mode ON ");
#endif


  pinMode(step_pin, INPUT_PULLUP);
  pinMode(dir_pin,  INPUT_PULLUP);
  pinMode(led_pin,  OUTPUT);
  pinMode(mode_pin, INPUT);

  pinMode(left_mode_pin,    INPUT_PULLUP);
  pinMode(right_mode_pin,  INPUT_PULLUP);

#ifdef BUZZER_ON
  pinMode(BuzzerPin, OUTPUT);
#endif

//set timer1 interrupt at 1kHz
cli();//stop interrupts
TCCR1A = 0;// set entire TCCR1A register to 0
TCCR1B = 0;// same for TCCR1B
TCNT1  = 0;//initialize counter value to 0
// set timer count for 1khz increments
OCR1A = 1999;// = (16*10^6) / (1000*8) - 1
//had to use 16 bit timer1 for this bc 1999>255, but could switch to timers 0 or 2 with larger prescaler
// turn on CTC mode
TCCR1B |= (1 << WGM12);
// Set CS11 bit for 8 prescaler
TCCR1B |= (1 << CS11);  
// enable timer compare interrupt
TIMSK1 |= (1 << OCIE1A);
sei();//allow interrupts

  MotorInit();

  // Status LED init set to OFF
  MyLED(OFF);

#ifdef OLED_DISPLAY_ON
  OledDisp.setRot180();
  OledDisp.firstPage();
  do {
    OledDisp.setRot180();
    OledDisp.setFont(u8g_font_unifont);
    OledDisp.setPrintPos(0, 10);
    OledDisp.print("OnStep Focuser");

  } while ( OledDisp.nextPage() );
#endif
  // Init interrupt for OnStep controll
//  ModeSensorValue = analogRead(mode_pin);
  attachInterrupt(digitalPinToInterrupt(step_pin), OnStepControll,FALLING  );

} //End of setup()


void loop() {


#ifdef OLED_DISPLAY_ON
  CurrentDisp = CurrentStep * StepsPerMicrometer / 5 ;
  if  ( LastDisp !=  CurrentDisp) {
    LastDisp =  CurrentDisp;

    OledDisp.firstPage();
    do {
      OledDisp.setFont(u8g_font_unifont);
      OledDisp.setPrintPos(10, 10);
      if (NowMode == NowOnStepMode) {
        OledDisp.print("OnStep Mode");
      }
      else if ( NowMode == NowStandyMode ){
        OledDisp.print("Standby Mode");
      }
      else  {
        OledDisp.print("Manual Mode");
      }
      OledDisp.setPrintPos(40, 40);
      OledDisp.print(CurrentDisp);

    } while ( OledDisp.nextPage() );
#
  }
#endif


//  ModeSensorValue = analogRead(mode_pin);


} // End of loop()



