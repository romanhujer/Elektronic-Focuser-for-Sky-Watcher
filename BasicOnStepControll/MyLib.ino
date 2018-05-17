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

// OnStepControll - Step/Dir Pins  Call in Interupt - Step pin (2) chage HIGH to LOW
//
void OnStepControll() {
  
   motor_step (digitalRead(dir_pin));
   MyLedChangeStatus();
   
} // End of OnStepControll()



