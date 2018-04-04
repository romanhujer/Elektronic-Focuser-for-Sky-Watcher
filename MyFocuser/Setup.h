// Setup.h
//
// Hardware specific config
//
// https://easyeda.com/hujer.roman/myfocuser-for-onstep
//
//

// Port Register for ULN2003 use ATMega168 Pins 10,11,12,13
#define ULN2003DDR      DDRB       // Data Direction Register   
#define ULN2003Port     PORTB      // Data Register
#define ULN2003PortMask B00111100
#define ULN2003FirstBit 2

#define step_pin 2  // Interupt from OnStep controll 
#define dir_pin  9  // Direcition from OnSteo controll

#define led_pin 3   // Status led pin

#define mode_pin A1

#define ON true
#define OFF false

// Sensor mode Zone define

#define StepsDownL 0
#define StepsDownH 480

#define StandbyLL 471
#define StandbyLH 491

#define OnStepL  492
#define OnStepH  532

#define StandbyRL 533
#define StandbyRH 553

#define StepsUpL 554
#define StepsUpH 1023


boolean led_status = OFF;
int8_t step_position = 0;
long ModeSensorValue = 512;
byte NowMode = 0;

#define  NowOnStepMode 1
#define  NowStandyMode 2
#define  NowDownMode   3
#define  NowUpMode     4

// OnStep Mode
#define OnStepMode(SensorValue) (((SensorValue) >= OnStepL) && ((SensorValue) <= OnStepH))

// Standby Mode
#define StandbyMode(SensorValue) ((((SensorValue) >= StandbyLL ) && ((SensorValue) <= StandbyLH)) || (((SensorValue) >= StandbyRL) && ((SensorValue) <= StandbyRH)))

// Down Mode
#define DownMode(SensorValue) (((SensorValue) >= StepsDownL) && ((SensorValue) <= StepsDownH))

// Up Mode
#define UpMode(SensorValue) (((SensorValue) >= StepsUpL) && ((SensorValue) <= StepsUpH))


