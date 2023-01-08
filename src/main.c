#include <Arduino.h>
#include "tb62709.h"

void setup(){
    init_display();
}


void loop(){
    unsigned char digits[4] = {0x04,0x05,0x06,0x07};

    set_4digit(digits);
    normal_mode();
    delay(500);

    unsigned char digits2[4] = {8,9,0,1};

    set_4digit(digits2);
    normal_mode();
    delay(500);

    load_data_register(REG0,0x10);
    load_data_register(REG1,0x1a);
    load_data_register(REG2,0x1b);
    load_data_register(REG3,0x1c);
    normal_mode();
    delay(500);
}

