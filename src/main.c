#include <Arduino.h>

#define DATA_PIN 2
#define CLOCK_PIN 3 
#define LOAD_PIN 4

typedef struct{
    unsigned char HB;
    unsigned char LB;
}serialin_t;


void setup(){
    pinMode(DATA_PIN,OUTPUT);
    pinMode(CLOCK_PIN,OUTPUT);
    pinMode(LOAD_PIN,OUTPUT);

    digitalWrite(DATA_PIN, LOW);
    digitalWrite(CLOCK_PIN, LOW);
    digitalWrite(LOAD_PIN, LOW);
}

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

void loop(){
    all_on_mode();
    delay(1000);

    blank_mode();
    delay(1000);
}

