#include <Arduino.h>
#include "tb62709.h"
#include "MsTimer2.h"



//return value = carriage
bool count60(int *count){
    *count += 1;

    if(*count > 9){
        *(count+1) += 1;
        *count = 0;
    }

    if(*(count+1) > 5){
        *(count+1) = 0;
        *count = 0;
        return true;
    }
    return false;

}


int minuites[2];
int seconds[2];
int tick_count = 0;

void tick(void){
    if(count60(seconds)){
        count60(minuites);
    }
}

four_digits_t loopAnim[] = {
    { SEGMENT[2], 0x00, 0x00, 0x00 },
    { SEGMENT[1], 0x00, 0x00, 0x00 },
    { SEGMENT[0], 0x00, 0x00, 0x00 },
    { 0x00, SEGMENT[0], 0x00, 0x00 },
    { 0x00, 0x00, SEGMENT[0], 0x00 },
    { 0x00, 0x00, 0x00, SEGMENT[0] },
    { 0x00, 0x00, 0x00, SEGMENT[5] },
    { 0x00, 0x00, 0x00, SEGMENT[4] },
    { 0x00, 0x00, 0x00, SEGMENT[3] },
    { 0x00, 0x00, SEGMENT[3], 0x00 },
    { 0x00, SEGMENT[3], 0x00, 0x00 },
    { SEGMENT[3], 0x00, 0x00, 0x00 },
};

void do_anim(){
    load_decode_and_digit_setting(false,DIG_ALL);

    four_digits_t d;
    for(int i=0;i<sizeof(loopAnim)/sizeof(loopAnim[0]);i++){
        d = loopAnim[i];
        set_4digit(d.array);
        delay(50);
    }

    load_decode_and_digit_setting(true,DIG_ALL);
}

void setup(){
    pinMode(DATA_PIN,OUTPUT);
    pinMode(CLOCK_PIN,OUTPUT);
    pinMode(LOAD_PIN,OUTPUT);

    digitalWrite(DATA_PIN, LOW);
    digitalWrite(CLOCK_PIN, LOW);
    digitalWrite(LOAD_PIN, LOW);

    load_duty_register(0x02);
    load_decode_and_digit_setting(true,DIG_ALL);

    MsTimer2::set(1000, tick);
    MsTimer2::start();
}




void loop(){

    unsigned char digits[4] = {0,0,0,0};
    digits[3] = minuites[1];
    digits[2] = minuites[0];
    digits[1] = seconds[1];
    digits[0] = seconds[0];

    set_4digit_dp(digits,2);
    normal_mode();

}

