// MyMotorDrive  
//
// Motor is 28BYJ-48 5V
// Motor rotation  64 steps
// Gears ratio 64:1
// Full rotation is 4096 steps
// and ULN2003 chip

#ifndef REVERSE_DIR_ON
  #define REVERSE_DIR  false
#else
  #define REVERSE_DIR  true
#endif


// MotorInit - Set ULN2003 port for Outpout
void MotorInit() 
{
   ULN2003DDR = ULN2003DDR | ULN2003PortMask;
} // End of MotorInit()

// SetULN2003 Set Motor Step
void SetULN2003( byte Step)
{
  ULN2003Port = ( ULN2003Port |  ULN2003PortMask ) ^ ( ~(Step << ULN2003FirstBit));

} // End of SetULN2003


// Steper motor driver
void motor_step( boolean  direction ) {
  if ( direction ^ REVERSE_DIR ) {
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

