// Config.h

#define DEBUG_ON


// Motor is 28BYJ-48 5V 
// Motor rotation  64 steps 
// Gears ratio 64:1
// Full rotation is 4096 steps

// Port Register for ULN2003 use ATMega168 Pins 10,11,12,13  
#define ULN2003DDR      DDRB       // Data Direction Register   
#define ULN2003Port     PORTB      // Data Register
#define ULN2003PortMask B00111100
#define ULN2003FirstBit 2

#define step_pin 2  // Interupt from OnStep controll 
#define dir_pin  9  // Direcition from OnSteo controll

#define led_pin 3   // Status led pin

#define mode_pin A1

// Sensor mode Zone define

#define StepDynamicLL 0
#define StepDynamicLH 309

#define Step01LL 310
#define Step01LH 390

#define StandbyLL 391
#define StandbyLH 471

#define OnStepL  472
#define OnSterH  552

#define StandbyRL 553
#define StandbyRH 633

#define Step01RL 634
#define Step01RH 714

#define StepDynamicRL 715
#define StepDynamicRH 1023




