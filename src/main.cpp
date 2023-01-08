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

void setup(){
    init_display();

    set_nodecode();

    //debug
    while(true){
        unsigned char digits[4];

        digits[0] = SEGMENT[0];
        digits[1] = SEGMENT[1];
        digits[2] = SEGMENT[2];
        digits[3] = SEGMENT[3];
        set_4digit(digits);
        delay(500);
    }

//    MsTimer2::set(10, tick);
//    MsTimer2::start();
}




void loop(){

    unsigned char digits[4] = {0,0,0,0};
    digits[3] = minuites[1];
    digits[2] = minuites[0];
    digits[1] = seconds[1];
    digits[0] = seconds[0];

    //set_4digit(digits);
    set_4digit_dp(digits,2);
    normal_mode();

}

