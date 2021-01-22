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



void buzzer( int _long ) {
  for (signed int _i = 0; _i < _long / 2; _i++) {
    digitalWrite(BuzzerPin, 1);
    delay(1);
    digitalWrite(BuzzerPin, 0);
    delay(1);
  }
} // end of buzzer( int _long )
