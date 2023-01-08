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

//decode enable is default
void load_decode_and_digit_setting(bool decodeEnable,digit_act_t digit_enb){
    unsigned char dec = 0x00; //nodecode
    if(decodeEnable){
        dec = 0x10;
    }
    dec = dec | digit_enb;
    
    serialin_t data = {
        .HB = 0x21,
        .LB = dec
    };
    byteout(data);
    load();
}

typedef enum{
    REG0 = 0x22,
    REG1 = 0x23,
    REG2 = 0x24,
    REG3 = 0x25,
}register_t;

void load_data_register(register_t reg, unsigned char digit){
    serialin_t data = {
        .HB = reg,
        .LB = digit 
    };
    byteout(data);
    load();
}

void normal_mode(void){
    serialin_t data = {
        .HB = 0x10,
        .LB = 0x00
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

    load_decode_and_digit_setting(true,DIG_ALL);
    load_duty_register(0x02);

}


void loop(){

    load_data_register(REG0,0x00);
    load_data_register(REG1,0x0a);
    load_data_register(REG2,0x0b);
    load_data_register(REG3,0x0c);
    normal_mode();
    delay(500);

    load_data_register(REG0,0x10);
    load_data_register(REG1,0x1a);
    load_data_register(REG2,0x1b);
    load_data_register(REG3,0x1c);
    normal_mode();
    delay(500);
}

