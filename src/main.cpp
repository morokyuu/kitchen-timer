#include <Arduino.h>
#include "tb62709.h"
#include "MsTimer2.h"

#define TONE_PIN 9
#define START_BTN 7
#define SELECT_BTN 8

//return value = borrow
bool countdown60(int *count){
    *count -= 1;

    if(*count < 0){
        *(count+1) -= 1;
        if(*(count+1) < 0){
            return true;
        }
        else{
            *count = 9;
        }
    }
    return false;
}

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



volatile bool tick_flag = false;
void tick(void){
    tick_flag = true;
}

void setup(){
    pinMode(DATA_PIN,OUTPUT);
    pinMode(CLOCK_PIN,OUTPUT);
    pinMode(LOAD_PIN,OUTPUT);

    digitalWrite(DATA_PIN, LOW);
    digitalWrite(CLOCK_PIN, LOW);
    digitalWrite(LOAD_PIN, LOW);


    pinMode(START_BTN, INPUT_PULLUP);
    pinMode(SELECT_BTN,INPUT_PULLUP);


    load_duty_register(0x02);
    load_decode_and_digit_setting(true,DIG_ALL);

    MsTimer2::set(70, tick);
    MsTimer2::start();

    pinMode(TONE_PIN,OUTPUT);
}

void tone_sound(){
    tone(TONE_PIN,440,100);
    delay(100);
    tone(TONE_PIN,392,100);
    delay(100);
    digitalWrite(TONE_PIN,LOW);
}


int minuites[2] = {3,0};
int seconds[2] = {0,5};
int tick_count = 0;

enum state_t{
    MENU,
    RUN,
    TIMEOVER,
};
state_t state;

int timer_process(){
    //kitchen-timer
    if(countdown60(seconds)){
        //tone_sound();
        seconds[1] = 5;
        seconds[0] = 9;

        if(countdown60(minuites)){
            tone_sound();
            tone_sound();
            return 1;
        }
    }

//  stopwatch
//    if(count60(seconds)){
//        tone_sound();
//        if(count60(minuites)){
//            tone_sound();
//            tone_sound();
//        }
//    }
    return 0;
}


void loop(){
    unsigned char digits[4];

    while(!tick_flag);
    tick_flag = false;

    switch(state){
        case MENU:
            if(!digitalRead(START_BTN)){
                state = RUN;
            }
            break;
        case RUN:
            if(timer_process()){
                state = TIMEOVER;
            }

            digits[3] = minuites[1];
            digits[2] = minuites[0];
            digits[1] = seconds[1];
            digits[0] = seconds[0];
            set_4digit_dp(digits,2);
            normal_mode();
            break;
        default:
            break;
    }



}

