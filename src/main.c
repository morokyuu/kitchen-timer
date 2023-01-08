#include <Arduino.h>

#define DATA_PIN 2
#define CLOCK_PIN 3 
#define LOAD_PIN 4

typedef struct{
    unsigned char HB;
    unsigned char LB;
}serialin_t;



void bitout(int val){
    digitalWrite(CLOCK_PIN, LOW);
    digitalWrite(DATA_PIN, val);
    digitalWrite(CLOCK_PIN, HIGH);
}

void byteout(serialin_t data){
    for(int i=0;i<8;i++){
        bitout(data.HB & 1<<(7-i));
    }
    for(int i=0;i<8;i++){
        bitout(data.LB & 1<<(7-i));
    }
}

void load(void){
    digitalWrite(LOAD_PIN, HIGH);
    digitalWrite(LOAD_PIN, LOW);
}

void all_on_mode(void){
    serialin_t data = {
        .HB = 0x30,
        .LB = 0x00
    };
    byteout(data);
    load();
}

void blank_mode(void){
    serialin_t data = {
        .HB = 0x00,
        .LB = 0x00
    };
    byteout(data);
    load();
}

void load_duty_register(unsigned char duty){
    serialin_t data = {
        .HB = 0x20,
        .LB = duty
    };
    byteout(data);
    load();
}

typedef enum{
    DIG0 = 0x00,
    DIG01 = 0x01,
    DIG012 = 0x02,
    DIG_ALL = 0x03
}digit_act_t;

void load_digit_activate_register(digit_act_t act){
    serialin_t data = {
        .HB = 0x21,
        .LB = act 
    };
    byteout(data);
    load();
}


void setup(){
    pinMode(DATA_PIN,OUTPUT);
    pinMode(CLOCK_PIN,OUTPUT);
    pinMode(LOAD_PIN,OUTPUT);

    digitalWrite(DATA_PIN, LOW);
    digitalWrite(CLOCK_PIN, LOW);
    digitalWrite(LOAD_PIN, LOW);

    load_digit_activate_register(DIG01);
}

void loop(){
    load_duty_register(0x0F);
    all_on_mode();
    delay(80);

    load_duty_register(0x07);
    all_on_mode();
    delay(80);

    load_duty_register(0x01);
    all_on_mode();
    delay(80);

    blank_mode();
    delay(80);

    load_duty_register(0x07);
    all_on_mode();
    delay(80);

}

