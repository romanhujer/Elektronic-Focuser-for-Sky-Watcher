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

#ifdef BUZZER_ON
  pinMode(BuzzerPin, OUTPUT);
#endif
  MotorInit();

  // Status LED init set to OFF
  MyLED(OFF);

#ifdef LED_DISPLAY_ON
  LedDisp.setBrightness(0x8);
  LedDisp.showNumberDec(abs(CurrentDisp), true);
#endif
  // Init interrupt for OnStep controll
  ModeSensorValue = analogRead(mode_pin);
  attachInterrupt(digitalPinToInterrupt(step_pin), OnStepControll, FALLING);

} //End of setup()


void loop() {


#ifdef LED_DISPLAY_ON
  CurrentDisp = CurrentStep * StepsPerMicrometer / 10 ;
  if  ( LastDisp !=  CurrentDisp) {
    LastDisp =  CurrentDisp;
    LedDisp.showNumberDec(abs(CurrentDisp), true);
 }
#endif

  ModeSensorValue = analogRead(mode_pin);
  if OnStepMode(ModeSensorValue) {
    if (NowMode != NowOnStepMode) {
#ifdef BUZZER_ON
      buzzer(50);
      delay(100);
      buzzer(50);
#endif
    }
    MyLED(ON);
    NowMode = NowOnStepMode;
  }
  else if StandbyMode(ModeSensorValue) {

#ifdef BUZZER_ON
    tone(BuzzerPin, BuzzerTone );
#endif
    MyLED(OFF);
    NowMode = NowStandyMode;
  } else if LeftMode(ModeSensorValue) {

#ifdef BUZZER_ON
    noTone(BuzzerPin);
#endif
    NowMode = NowLeftMode;
    MyLEDblink();
    ManualControll( ModeSensorValue - SensorMin, LEFT);

  } else if RightMode(ModeSensorValue) {

#ifdef BUZZER_ON
    noTone(BuzzerPin);
#endif
    NowMode = NowRightMode;
    MyLEDblink();
    ManualControll(abs(ModeSensorValue - SensorMax), RIGHT);
  }

} // End of loop()



