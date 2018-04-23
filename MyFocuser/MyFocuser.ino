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
#define Version "1.0a"

#include "Config.h"
#include "Setup.h"

#ifdef DEBUG_ON
int LastModeSensorValue = 0;
#endif

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

  MotorInit();

  // Status LED init set to OFF
  MyLED(OFF);

  // Init interrupt for OnStep controll
  ModeSensorValue = analogRead(mode_pin);
  attachInterrupt(digitalPinToInterrupt(step_pin), OnStepControll,HIGH );

} //End of setup()


void loop() {

  ModeSensorValue = analogRead(mode_pin);

#ifdef DEBUG_ON
  if (ModeSensorValue != LastModeSensorValue) {
    LastModeSensorValue = ModeSensorValue
                          Serial.print("Sensor value is :");
    Serial.println(ModeSensorValue);
  }
#endif

  if OnStepMode(ModeSensorValue) {

#ifdef DEBUG_ON
    if (NowMode != NowOnStepMode) Serial.println("OnStep Controll");
#endif

    MyLED(ON);
    NowMode = NowOnStepMode;
  }
  else if StandbyMode(ModeSensorValue) {

#ifdef DEBUG_ON
    if (NowMode != NowStandyMode)  Serial.println("Standy mode");
#endif

    MyLED(OFF);
    NowMode = NowStandyMode;
  } else if LeftMode(ModeSensorValue) {

#ifdef DEBUG_ON
    if (NowMode != NowLeftMode)  Serial.println("Left mode");
#endif

    NowMode = NowLeftMode;
    MyLEDblink();
    ManualControll( ModeSensorValue - SensorMin, LEFT);

  } else if RightMode(ModeSensorValue) {

#ifdef DEBUG_ON
    if (NowMode != NoRightMode)   Serial.println("Right mode");
#endif

    NowMode = NowRightMode;
    MyLEDblink();
    ManualControll(abs(ModeSensorValue - SensorMax), RIGHT);
  }

} // End of loop()



