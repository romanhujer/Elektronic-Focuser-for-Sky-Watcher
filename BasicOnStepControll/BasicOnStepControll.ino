/*
   BasicOnStepControll.ino    Only OnStepControll - test code

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

#define Product "BasicOnStepControll"
#define Version "1.0"

#include "Config.h"
#include "Setup.h"

void setup() {

  pinMode(step_pin, INPUT_PULLUP);
  pinMode(dir_pin,  INPUT_PULLUP);
  pinMode(led_pin,  OUTPUT);
  MotorInit();

  // Status LED init set to OFF
  MyLED(OFF);
  // Step_pin signal HIGH to LOW  Attach Interrupt to subroutine OnStepControll
  attachInterrupt(digitalPinToInterrupt(step_pin), OnStepControll, FALLING );

} //End of setup()

void loop() {

} // End of loop()



