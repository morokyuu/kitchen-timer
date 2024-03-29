#include <Arduino.h>
#include "tb62709.h"
#include "MsTimer2.h"

#define TONE_PIN   4
#define SELECT_BTN 3
#define START_BTN  2

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

    //5s late
    MsTimer2::set(92, tick);
    MsTimer2::start();

    pinMode(TONE_PIN,OUTPUT);
}

void tone_sound(){
    tone(TONE_PIN,440,100);
    delay(20);
    tone(TONE_PIN,392,100);
    delay(20);
    digitalWrite(TONE_PIN,LOW);
}


union timerset_t{
    int array[4];
    struct {
        int minuites[2];
        int seconds[2];
    }keta;
};
timerset_t sett = {1,0,0,0};
timerset_t time;


enum state_t{
    MENU,
    RUN,
    TIMEOVER,
};
state_t state;


int tick_count = 0;
bool divider(int *count){
    (*count)++;
    if(*count > 10){
        *count = 0;
        return true;
    }
    return false;
}

int timer_process(){
    if(!divider(&tick_count)){
        return 0;
    }

    //kitchen-timer
    if(countdown60(time.keta.seconds)){
        time.keta.seconds[1] = 5;
        time.keta.seconds[0] = 9;
        if(countdown60(time.keta.minuites)){
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

void alarm(){
    static int blink_cn = 0;
    const int brightness[6] = {0,2,4,7,4,2};

    for(int i=0;i<6;i++){
        load_duty_register(i);
        tone_sound();
    }
}

const timerset_t sett_arry[] = {
    {1,0,0,0}, //1min
    {3,0,0,0}, //3min
    {5,0,0,0}, //5min
    {7,0,0,0}, //7min
    {0,1,0,0}, //10min
    {2,1,0,0}, //12min
    {5,1,0,0}, //15min
    {0,2,0,0}, //20min
    {5,2,0,0}, //25min
    {0,3,0,0}, //30min
};

void select_time(timerset_t *sett){
    static int round = 0;

    if(!digitalRead(SELECT_BTN)){
        round++;
        if(round > sizeof(sett_arry)/sizeof(sett_arry[0])-1){
            round = 0;
        }
        delay(300);
    }
    
    *sett = sett_arry[round];

    unsigned char digits[4];
    digits[3] = (*sett).keta.minuites[1];
    digits[2] = (*sett).keta.minuites[0];
    digits[1] = (*sett).keta.seconds[1];
    digits[0] = (*sett).keta.seconds[0];
    set_4digit_dp(digits,2);
    normal_mode();
}

void loop(){

    while(!tick_flag);
    tick_flag = false;

    switch(state){
        case MENU:
            select_time(&sett);

            if(!digitalRead(START_BTN)){
                time = sett;
                state = RUN;
            }
            break;
        case RUN:
            if(!digitalRead(SELECT_BTN)){
                //reset
                delay(300);
                state = MENU;
            }
            else if(timer_process()){
                state = TIMEOVER;
            }
            else{
                unsigned char digits[4];
                digits[3] = time.keta.minuites[1];
                digits[2] = time.keta.minuites[0];
                digits[1] = time.keta.seconds[1];
                digits[0] = time.keta.seconds[0];
                set_4digit_dp(digits,2);
                normal_mode();
            }
            break;
        case TIMEOVER:
            if(!digitalRead(START_BTN) ||
               !digitalRead(SELECT_BTN))
            {
                load_duty_register(0x02);
                state = MENU;
            }
            else{
                alarm();
            }
            break;
        default:
            break;
    }
}

