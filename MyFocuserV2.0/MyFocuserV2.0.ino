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
#define Version "2.0"

#include "Config.h"
#include "Setup.h"

int LastModeSensorValue = 0;
long int CurrentStep = 0;
long int LastDisp = -1;
long int CurrentDisp = 0;
int CurrentSpeed = 0;
int LastSpeed = -1;
int SpeedTimer = 290;
int LongInit = 0;


//#include "MyMotorDrive.h"
//#include "MyLib.h"

#ifdef LED_DISPLAY_ON
#include <TM1637Display.h>  //https://github.com/avishorp/TM1637
TM1637Display LedDisp(LedDispClk, LedDispDio);
#endif

#ifdef OLED_DISPLAY_ON
#include <U8glib.h>
U8GLIB_SSD1306_128X64 OledDisp(U8G_I2C_OPT_NONE);
#endif




void setup() {

#ifdef ONSTEP_ON
  pinMode(step_pin, INPUT_PULLUP);
  pinMode(dir_pin,  INPUT_PULLUP);
#endif

  pinMode(led_pin,  OUTPUT);
  pinMode(mode_pin, INPUT);

  pinMode(left_mode_pin,    INPUT_PULLUP);
  pinMode(right_mode_pin,  INPUT_PULLUP);

#ifdef BUZZER_ON
  pinMode(BuzzerPin, OUTPUT);
#endif
  MotorInit();


  //set timer1 interrupt at 1kHz
  cli();             //stop interrupts
  TCCR1A = 0;        // set entire TCCR1A register to 0
  TCCR1B = 0;        // same for TCCR1B
  TCNT1  = 0;        //initialize counter value to 0
  // set timer count for 1khz increments
  OCR1A = 1999;      // = (16*10^6) / (1000*8) - 1
  //had to use 16 bit timer1 for this bc 1999>255, but could switch to timers 0 or 2 with larger prescaler
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS11 bit for 8 prescaler
  TCCR1B |= (1 << CS11);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei();//allow interrupts

 
  // Status LED init set to OFF
  MyLED(OFF);

#ifdef OLED_DISPLAY_ON
  OledDisp.setRot180();
  OledDisp.firstPage();
  do {
    OledDisp.setRot180();
    OledDisp.setFont(u8g_font_unifont);
    OledDisp.setPrintPos(10, 20);
    OledDisp.print("W E L C O M E");
    OledDisp.setPrintPos(10, 40);
    OledDisp.print("OnStep Focuser");

  } while ( OledDisp.nextPage() );
  buzzer(500);
  delay(2000);
#endif

#ifdef ONSTEP_ON
  // Init interrupt for OnStep controll
  //  ModeSensorValue = analogRead(mode_pin);
  attachInterrupt(digitalPinToInterrupt(step_pin), OnStepControll, FALLING  );
#endif

} //End of setup()


void loop() {

 // CurrentSpeed = analogRead(mode_pin)/5+1;

 CurrentSpeed =  int (exp( analogRead(mode_pin)/180.0)+ 0.5);
 
#ifdef OLED_DISPLAY_ON
  //  CurrentDisp = CurrentStep * StepsPerMicrometer / 5 ;
  CurrentDisp = CurrentStep ;
  if  ( LastDisp !=  CurrentDisp  || LastSpeed != CurrentSpeed ) {
    LastDisp  = CurrentDisp;
    LastSpeed = CurrentSpeed;
    OledDisp.firstPage();
    do {
      OledDisp.setFont(u8g_font_unifont);
      OledDisp.setPrintPos(10, 10);
      if (NowMode == 2 ) {
        OledDisp.print("OnStep Mode");
      }
      else if ( NowMode == 1 ) {
        OledDisp.print("Manual Mode");
      }
      else  {
        OledDisp.print("Init Mode");
      }
      
      OledDisp.setPrintPos(1, 30);
      OledDisp.print("P:");
      OledDisp.setPrintPos(20, 30);
      OledDisp.print(CurrentDisp);
      OledDisp.print("/");
      OledDisp.print(CurrentDisp*Step_um/1000.);
      OledDisp.print("mm");
      if ( NowMode == 1 ) {
        OledDisp.setPrintPos(1, 50);
        OledDisp.print("Speed:");
        OledDisp.setPrintPos(50, 50);
        OledDisp.print( int( CurrentSpeed/294. *100. + 0.8));
      }
    } while ( OledDisp.nextPage() );
  }
#endif

} // End of loop()



