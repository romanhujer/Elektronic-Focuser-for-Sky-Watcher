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

// MyLEDblink
void MyLEDblink( ) {
  if (led_status) {
    MyLED(OFF);
  } else {
    MyLED(ON);
  }
} //End of MyLEDblink()

// OnStepControll - Step/Dir Pins  Call in Interupt - Dir pin (2) chage LOW to HIGH
//
void OnStepControll() {
  if OnStepMode( ModeSensorValue ) motor_step (!digitalRead(dir_pin));
} // End of OnStepControll()


// ManualControll
void ManualControll( int Value, boolean Dir ) {
  motor_step(Dir);
  delay( int(exp( Value/ManualSpeedCurve) ) );
} // End  of ManualControll()






