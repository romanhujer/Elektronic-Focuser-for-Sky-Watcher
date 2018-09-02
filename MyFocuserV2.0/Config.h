// Config.h 
// User specific configuration for MyFocuser
// 
// Copyright (c) 2018 Roman Hujer   http://hujer.net
//


#define ManualSpeedCurve  90.0 //  Corection speed curve in manual mode (try from 60 to 90) default 75.0 
#define REVERSE_DIR_OFF        // _ON  is reverse the direction of focuser movement default _OFF

#define NEMA_ON            // NEMA_ON  Use NEMA17 steper motor or NEMA_OFF use 28BYJ-48 steper motor
#define ONSTEP_OFF         // _ON OnStep dir step cotroll _OFF only Handled controler

#define DEBUG_OFF          // Turn _ON is DEBUG mode default _OFF
#define SERIAL_BAUD 115200 // Serial port bauts for DEBUG mode default 115200

#define OLD_MODE_OFF      

#define LED_DISPLAY_OFF    // TM1637  LED Dispaly
#define OLED_DISPLAY_ON    // OLED Dispaly

#define BUZZER_ON	   // Buzzer


#define StepsPerMicrometer  0.6  // Steps Per Micrometer 
#define Step_um   1.2258  //  38000 um  is about 31000 steps

