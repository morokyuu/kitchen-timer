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
    count60(seconds);
}

void setup(){
    init_display();

    MsTimer2::set(100, tick);
    MsTimer2::start();
}




void loop(){

    unsigned char digits[4] = {0,0,0,0};
    digits[3] = minuites[1];
    digits[2] = minuites[0];
    digits[1] = seconds[1];
    digits[0] = seconds[0];

    set_4digit(digits);
    normal_mode();

}

