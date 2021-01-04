// MyMotorDrive.ino  
// Motor Driver for MyFocuser 
//
// Copyright (c) 2018 Roman Hujer   http://hujer.net
//
//
// Motor is 28BYJ-48 5V or 12V
//
// NOTE:
// Motor rotation  64 steps
// Gears ratio 64:1
// Full rotation is 4096 steps
// ULN2003 chip
//
// NEMA17 step motor use dirver TMC2100 Step Dir kontrol




#ifndef REVERSE_DIR_ON
  #define REVERSE_DIR  false
#else
  #define REVERSE_DIR  true
#endif



// MotorInit - Set ULN2003 port for Outpout
void MotorInit() 
{

#ifndef NEMA_ON 
   ULN2003DDR = ULN2003DDR | ULN2003PortMask;
#else
    pinMode(driver_dir,  OUTPUT);
    pinMode(driver_step,  OUTPUT);
    digitalWrite(driver_dir, LOW);
    digitalWrite(driver_step,LOW);  
#endif 


} // End of MotorInit()


#ifndef NEMA_ON 
// SetULN2003 Set Motor Step
void SetULN2003( byte Step)
{
  ULN2003Port = ( ULN2003Port |  ULN2003PortMask ) ^ ( ~(Step << ULN2003FirstBit));

} // End of SetULN2003
#endif 

// Steper motor driver
void motor_step( boolean  direction ) {

   if ( direction ^ REVERSE_DIR ) {
#ifndef NEMA_ON 
    step_position++;
#endif
    CurrentStep++;
  }
  else {
#ifndef NEMA_ON     
    step_position--;
#endif    
    CurrentStep--;
  }
#ifdef NEMA_ON     
       digitalWrite(driver_dir,direction);
       digitalWrite(driver_step,HIGH);
       digitalWrite(driver_step,LOW);  
#else  
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
#endif
}

