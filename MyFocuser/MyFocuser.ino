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

// čísla pinů pro digitální výstupy vstupy
const int  in1 =  13;
const int  in2 =  12;
const int  in3 =  11;
const int  in4 =  10;
const int  step_pin = 9;
const int  dir_pin  = 8;

int8_t step_position = 0;

int rychlost = 1;
//
int uhel = 360;

#define SERIAL_BAUD 115200
#define USE_SERIAL Serial


void setup() {

  USE_SERIAL.begin(SERIAL_BAUD);
  USE_SERIAL.println("...");

  
  // inicializace digitálních výstupů
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(step_pin, INPUT);
  pinMode(dir_pin, INPUT);
}


void rotacePoSmeru() {
  motor_step( 1 );
  delay(rychlost);
}
  
void rotaceProtiSmeru() {
  motor_step( 0 );
  delay(rychlost);
}

void motor_step( int  direction ) {

  
  if ( direction ){ 
    step_position++; 
  }
  else { 
    step_position--;
  }
   
  if (step_position < 0) step_position = 7;
  if (step_position > 7) step_position = 0;
//  USE_SERIAL.println( step_position );
  switch (step_position) {
    case 0: {
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        digitalWrite(in3, LOW);
        digitalWrite(in4, LOW);
        break;
      }
    case 1: {
        digitalWrite(in1, HIGH);
        digitalWrite(in2, HIGH);
        digitalWrite(in3, LOW);
        digitalWrite(in4, LOW);
        break;
      }
    case 2: {
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        digitalWrite(in3, LOW);
        digitalWrite(in4, LOW);
        break;
      }
    case 3: {
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        digitalWrite(in3, HIGH);
        digitalWrite(in4, LOW);
        break;
      }
    case 4: {
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        digitalWrite(in3, HIGH);
        digitalWrite(in4, LOW);
        break;
      }
    case 5: {
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        digitalWrite(in3, HIGH);
        digitalWrite(in4, HIGH);
        break;
      }
    case  6: {
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        digitalWrite(in3, LOW);
        digitalWrite(in4, HIGH);
        break;
      }
    case 7: {
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        digitalWrite(in3, LOW);
        digitalWrite(in4, HIGH);
        break;
      }
  }
}

void loop() {
/*
for(int i=0;i<(uhel*64/45)*8;i++){
    rotacePoSmeru();
  }
  // pauza po dobu 1 vteřiny
  
  for(int i=0;i<(uhel*64/45)*8;i++){
    rotaceProtiSmeru();
  }
  // pauza po dobu 1 vteřiny
  delay(10);

 */ 
  if ( digitalRead(step_pin) ) {
    motor_step (digitalRead(dir_pin));
    while ( digitalRead(step_pin)) ;  // wait for end step pulse

  }
}
