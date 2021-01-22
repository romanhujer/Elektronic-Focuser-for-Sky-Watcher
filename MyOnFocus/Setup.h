// Setup.h
// Hardware specific config for MyFocuser
// 
// Copyright (c) 2018 Roman Hujer   http://hujer.net
//
// Wring and PCB on  https://easyeda.com/hujer.roman/myfocuser-for-onstep
//
//

// Port Register for ULN2003 on ATMega168 osr ATMega368 use pins 10,11,12,13
#define ULN2003DDR      DDRB       // Data Direction Register   
#define ULN2003Port     PORTB      // Data Register
#define ULN2003PortMask B00111100  // Pins mask
#define ULN2003FirstBit 2          // First pin bit


#define led_pin 3     // Status led pin
#define BuzzerPin  4  // Buzzer Pin

// LED
#define ON  true
#define OFF false

boolean led_status = OFF;
int step_position = 0;
