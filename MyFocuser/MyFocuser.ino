/*
   MyFocuer.ino

   Copyright (c) 2018 Roman Hujer

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

*/


#define Product "MyFocuserForOnStep"
#define Version "1.0"

#include "Config.h"
#include "Setup.h"


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
  attachInterrupt(digitalPinToInterrupt(step_pin), OnStepControll, HIGH);

} //End of setup()


void loop() {

  ModeSensorValue = analogRead(mode_pin);
  if OnStepMode(ModeSensorValue) {
#ifdef DEBUG_ON
    if (NowMode != NowOnStepMode) Serial.println("Now is OnStep Controll");
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
  } else if DownMode(ModeSensorValue) {
#ifdef DEBUG_ON
    if (NowMode != NowDownMode)  Serial.println("Down mode");
#endif
    NowMode = NowDownMode;
    MyLEDblink();
    
    motor_step(0);
   delay( ModeSensorValue/MinStepRate + MaxStepRate );
   

    
  } else if UpMode(ModeSensorValue) {
#ifdef DEBUG_ON
    if (NowMode != NowUpMode)   Serial.println("Up mode");
#endif
    NowMode = NowUpMode;
    MyLEDblink();

    motor_step(1);
    delay( abs(ModeSensorValue-1023)/MinStepRate + MaxStepRate );
    
        
  }

} // End of loop()


// OnStepControll - Step/Dir Pins  Call in Interupt - Dir pin (2) chage LOW to HIGH
//
void OnStepControll() {
  
  
  if OnStepMode( ModeSensorValue ) motor_step (digitalRead(dir_pin));
  
} // End of OnStepControll()


