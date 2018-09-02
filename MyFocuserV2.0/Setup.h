// Setup.h
// Hardware specific config for MyFocuser
//
// Copyright (c) 2018 Roman Hujer   http://hujer.net
//
// Wring and PCB on  https://easyeda.com/hujer.roman/myfocuser-for-onstep
//
//

// Port Register for ULN2003 on ATMega168/368 use pins 10,11,12,13

#ifndef NEMA_ON

#define ULN2003DDR      DDRB       // Data Direction Register   
#define ULN2003Port     PORTB      // Data Register
#define ULN2003PortMask B00111100  // Pins mask
#define ULN2003FirstBit 2          // First pin bit

#endif


#ifdef ONSTEP_ON

#define step_pin 2  // Interupt from OnStep controll 
#define dir_pin  9  // Direcition from OnStep controll

#endif


#ifdef NEMA_ON

#define driver_step 8             // connected to stepper driver step pin (required)
#define driver_dir  9             // connected to stepper driver dir pin  (required)

#endif



#define led_pin 3       // Status led pin
#define left_mode_pin 6     // UP Mode button pin 
#define right_mode_pin 5 // DOWN Mode button pin 

#define LedDispClk A5	// CLK  (SCL)
#define LedDispDio A4	// DIO  (SDA)

#define BuzzerPin  4    // Buzzer Pin
#define BuzzerTone 443  // Buzzer tone in Hz 

#define mode_pin A1  // Sensor (Poteciometr) Pin

// LED
#define ON  true
#define OFF false

// Motor Direction
#define LEFT  false
#define RIGHT true

boolean led_status = OFF;
int step_position = 0;

byte NowMode = 1;

#define  NowInitMode 0
#define  NowOnStepMode 1
#define  NowStandyMode 2
#define  NowLeftMode   3
#define  NowRightMode  4



