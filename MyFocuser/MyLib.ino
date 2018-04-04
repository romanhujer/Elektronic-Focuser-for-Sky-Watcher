// MyLib.ino

// MyLED
void MyLED ( boolean mode )
{
  if (mode)  {
    digitalWrite(led_pin, HIGH); led_status = ON ;
  }
  else {
    digitalWrite(led_pin, LOW); led_status = OFF;
  }
} //End Of MyLED()

//
//
void MyLEDblink( )
{

if (led_status) { MyLED(OFF); } else {  MyLED(ON); }

} //End of MyLEDblink()

