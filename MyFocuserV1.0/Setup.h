// Setup.h
// Hardware specific config for MyFocuser
// 
// Copyright (c) 2018 Roman Hujer   http://hujer.net
//
// Wring and PCB on  https://easyeda.com/hujer.roman/myfocuser-for-onstep
//
//

// Port Register for ULN2003 on ATMega168 use pins 10,11,12,13
#define ULN2003DDR      DDRB       // Data Direction Register   
#define ULN2003Port     PORTB      // Data Register
#define ULN2003PortMask B00111100  // Pins mask
#define ULN2003FirstBit 2          // First pin bit

#define step_pin 2  // Interupt from OnStep controll 
#define dir_pin  9  // Direcition from OnSteo controll

#define led_pin 3       // Status led pin
#define Mode1_pin 5     // Mode1 button pin 
#define Mode2_pin 6     // Mode2 button pin 
#define Limit_pin A6     // Limit pin for auto calibration


#define LedDispClk A5	// TM1637 CLK
#define LedDispDio A4	// TM1637 DIO 

#define BuzzerPin  4   // Buzzer Pin
#define BuzzerTone 443  // Buzzer tone in Hz 

#define mode_pin A1  // Sensor (Poteciometr) Pin

// LED
#define ON  true
#define OFF false

// Motor Direction
#define LEFT  false
#define RIGHT true

// Sensor Zone define
#define SensorMin 0
#define SensorCenter 512
#define SensorMax 1023

#define StepsLeftL 0
#define StepsLeftR 470

#define StandbyLL 474
#define StandbyLR 480

#define OnStepL  482
#define OnStepR  532

#define StandbyRL 530
#define StandbyRR 560

#define StepsRightL 564
#define StepsRightR 1023


boolean led_status = OFF;
int step_position = 0;
int ModeSensorValue = SensorCenter;
byte NowMode = 0;

#define  NowOnStepMode 1
#define  NowStandyMode 2
#define  NowLeftMode   3
#define  NowRightMode  4

// MACROS for Senzor zone select
//
// OnStep Mode
#define OnStepMode(SensorValue) (((SensorValue) >= OnStepL) && ((SensorValue) <= OnStepR))

// Standby Mode
#define StandbyMode(SensorValue) ((((SensorValue) >= StandbyLL ) && ((SensorValue) <= StandbyLR)) || (((SensorValue) >= StandbyRL) && ((SensorValue) <= StandbyRR)))

// Left Mode
#define LeftMode(SensorValue) (((SensorValue) >= StepsLeftL) && ((SensorValue) <= StepsLeftR))

// Right Mode
#define RightMode(SensorValue) (((SensorValue) >= StepsRightL) && ((SensorValue) <= StepsRightR))


