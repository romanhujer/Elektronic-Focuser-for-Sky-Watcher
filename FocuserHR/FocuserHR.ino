/*

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


#define Product "FocuserHR"
#define Version "1.0"

#include "Config.h"
#include "Setup.h"

int8_t led_status = LOW;
int8_t step_position = 0;
long ModeSensorValue = 512;


#define SERIAL_BAUD 115200



void setup() {

#ifdef DEBUG_ON
  Serial.begin(SERIAL_BAUD);
  for (uint8_t t = 10; t > 0; t--) {
    delay(100);
    Serial.print(".");
  }
  Serial.println(".");
  Serial.println("Debug mode ON ");
#endif

 
 // Set ULN2003 port to Outpout
  ULN2003DDR = ULN2003DDR | ULN2003PortMask; 

  
  pinMode(step_pin, INPUT_PULLUP);
  pinMode(dir_pin, INPUT_PULLUP);

  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, HIGH);
  led_status = LOW;

  pinMode(mode_pin, INPUT);
  
  attachInterrupt(digitalPinToInterrupt(step_pin), OnStepControll, HIGH);

} //End of setup()



void loop() {
  ModeSensorValue = analogRead(mode_pin);
  if (ModeSensorValue >= 473 && ModeSensorValue <= 551) {
    digitalWrite(led_pin, HIGH); led_status = LOW;
#ifdef DEBUG_ON
    Serial.println("Start of OnStep Controll");
#endif
    //    old_test() ;

  }
  if (ModeSensorValue < 473 || ModeSensorValue > 551) {
    digitalWrite(led_pin, LOW); led_status = HIGH;
#ifdef DEBUG_ON
    Serial.println("STOP OnStep Controll");
#endif

 }

} // End of loop()

//
// OnStepControll - Step/Dir Pins
//
void OnStepControll() {
  ModeSensorValue = analogRead(mode_pin);
  // Test for OnStep  ON
  if ( ModeSensorValue >= 473 && ModeSensorValue <= 551 ) {
  
    motor_step (digitalRead(dir_pin));
  }
} // End of OnStepControll()


// SetULN2003 Set Motor Step 
void SetULN2003( byte Step)
{
   ULN2003Port = ( ULN2003Port |  ULN2003PortMask ) ^ ( ~(Step << ULN2003FirstBit));
   
} // End of SetULN2003


void motor_step( int  direction ) {
  if ( direction ) {
    step_position++;
  }
  else {
    step_position--;
  }
  if (step_position < 0) step_position = 7;
  if (step_position > 7) step_position = 0;
  
  switch (step_position) {
    case 0: {    
        SetULN2003( B0001 );
        break;
      }
    case 1: {
        SetULN2003( B0011 );
        break;
      }
    case 2: {
        SetULN2003( B0010 );
        break;
      }
    case 3: {
        SetULN2003( B0110 );
        break;
      }
    case 4: {
        SetULN2003( B0100 );
        break;
      }
    case 5: {
        SetULN2003( B1100 );
        break;
      }
    case  6: {
        SetULN2003( B1000 );
        break;
      }
    case 7: {
        SetULN2003( B1001 );
        break;
      }
  }
}

void old_test() {
  int rychlost = 1;
  int uhel = 360;

  for (int i = 0; i < (uhel * 64 / 45) * 8; i++) {
    motor_step( 1 );
    delay(rychlost);

  }
  delay(1000);
  for (int i = 0; i < (uhel * 64 / 45) * 8; i++) {
    motor_step( 0 );
    delay(rychlost);
  }

  if ( led_status == HIGH ) {
    digitalWrite(led_pin, HIGH);
    led_status = LOW;
  }
  else {
    digitalWrite(led_pin, LOW); led_status = HIGH;

  }

}
