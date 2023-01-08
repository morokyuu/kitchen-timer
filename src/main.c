#include <Arduino.h>
#include "tb62709.h"

void setup(){
    pinMode(DATA_PIN,OUTPUT);
    pinMode(CLOCK_PIN,OUTPUT);
    pinMode(LOAD_PIN,OUTPUT);

    digitalWrite(DATA_PIN, LOW);
    digitalWrite(CLOCK_PIN, LOW);
    digitalWrite(LOAD_PIN, LOW);

    load_decode_and_digit_setting(true,DIG_ALL);
    load_duty_register(0x02);

}


void loop(){

    load_data_register(REG0,0x00);
    load_data_register(REG1,0x01);
    load_data_register(REG2,0x02);
    load_data_register(REG3,0x43);
    normal_mode();
    delay(500);

    load_data_register(REG0,0x10);
    load_data_register(REG1,0x1a);
    load_data_register(REG2,0x1b);
    load_data_register(REG3,0x1c);
    normal_mode();
    delay(500);
}

