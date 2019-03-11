
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

#define Version "2.0"

#include "Config.h"
#include "Setup.h"

int LastModeSensorValue = 0;
long int CurrentStep = 0;
long int LastDisp = -1;
long int CurrentDisp = 0;
int CurrentSpeed = 0;
int LastSpeed = -1;
int SpeedTimer = 256;
int LongInit = 0;


#ifdef LED_DISPLAY_ON
#include <TM1637Display.h>  //  https://github.com/avishorp/TM1637
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
  // set timer count for 2khz increments
 //OCR1A = 1999;      // = (16*10^6) / (1000*8) - 1
  OCR1A = 499;      // = (16*10^6) / (4000*8) - 1
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
      OledDisp.setFont(u8g_font_unifont);
      OledDisp.setPrintPos(2, 40);
      OledDisp.print("Focuser ready!");
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
#if defined(__AVR_ATmega168__) ||defined(__AVR_ATmega168P__)
   int x = analogRead(mode_pin);
   if ( x > 100 && x < 200 ) CurrentSpeed = 2;
   else if (x > 200 && x < 300) CurrentSpeed = 4;
   else if (x > 300 && x < 400) CurrentSpeed = 8;
   else if (x > 400 && x < 500) CurrentSpeed = 16;
   else if (x > 500 && x < 600) CurrentSpeed = 32;
   else if (x > 600 && x < 700) CurrentSpeed = 64;
   else if (x > 701 && x < 800) CurrentSpeed = 128;
   else if (x > 701 ) CurrentSpeed = x / 4;
   else CurrentSpeed = 1; 
#endif

#if defined(__AVR_ATmega328P__)
 CurrentSpeed =  int (exp( analogRead(mode_pin) / 180.0) + 0.5);
  
#endif
 
#ifdef OLED_DISPLAY_ON
    CurrentDisp = CurrentStep ;
  if  ( LastDisp !=  CurrentDisp  || LastSpeed != CurrentSpeed ) {
    LastDisp  = CurrentDisp;
    LastSpeed = CurrentSpeed;
    OledDisp.firstPage();
    do {
      OledDisp.setFont(u8g_font_unifont);
#ifdef ONSTEP_ON
  OledDisp.setPrintPos(10, 10);
 if (NowMode == 2 ) {
        OledDisp.print("OnStep Mode");
      }
      else 

      if ( NowMode == 1 ) {
        OledDisp.print("Manual Mode");
      }
      else  {
        OledDisp.print("Init Mode");
      }
#else
  OledDisp.setPrintPos(10, 10);
  OledDisp.print("Step :");
  OledDisp.print(CurrentDisp);
#endif            
      OledDisp.setPrintPos(10, 30);
      OledDisp.print("Micro:");   
      OledDisp.print(int(CurrentStep/StepsPerMicrometer));
      OledDisp.setPrintPos(10, 50);
      OledDisp.print("Speed:");
      OledDisp.print(CurrentSpeed);
      
    } while( OledDisp.nextPage());
  }
#endif

} // End of loop()
