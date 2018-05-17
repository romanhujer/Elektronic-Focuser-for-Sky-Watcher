// MyLib.ino
// Librarry function for MyFocuser
// 
// Copyright (c) 2018 Roman Hujer   http://hujer.net
//

// MyLED
void MyLED ( boolean mode ) {
  if (mode)  {
    digitalWrite(led_pin, HIGH); led_status = ON ;
  }
  else {
    digitalWrite(led_pin, LOW); led_status = OFF;
  }
} //End Of MyLED()

// MyLedChangeStatus
void MyLedChangeStatus( ) {
  if (led_status) {
    MyLED(OFF);
  } else {
    MyLED(ON);
  }
} //End of MyLEDblink()



// OnStepControll - Step/Dir Pins  Call in Interupt - Dir pin (2) chage LOW to HIGH
//
void OnStepControll() {
    MyLedChangeStatus();
    motor_step (digitalRead(dir_pin));
} // End of OnStepControll()



//TIMER1  handle Interrupt at freq of 1kHz 
ISR(TIMER1_COMPA_vect) {
boolean left,  right;

 left  = !digitalRead(left_mode_pin);
 right = !digitalRead(right_mode_pin);

 if ( left &&  right) {
     NowMode = 0;
 }
 else if (left ) {
  
   NowMode = 4;
   SpeedTimer -= CurrentSpeed;
   if (SpeedTimer < 1) {
     SpeedTimer = 205; 
     MyLedChangeStatus();
     motor_step(HIGH);
   } 
 } else if ( right ) {
   NowMode = 3;
   SpeedTimer -= CurrentSpeed;
   if (SpeedTimer < 1) {
     SpeedTimer = 205; 
     MyLedChangeStatus();
     motor_step(LOW);
   } 
 }


}  // end of ISR  for TIMER1


#ifdef BUZZER_ON 
void buzzer( int _long ) {
  tone(BuzzerPin, BuzzerTone );
  delay(_long);
  noTone(BuzzerPin);
} //
#endif



