
#include "tb62709.h"

//----------------------------------------- bottom layer
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
//----------------------------------------- middle layer

void load_duty_register(unsigned char duty){
    serialin_t data = {
        .HB = 0x20,
        .LB = duty
    };
    byteout(data);
    load();
}

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

void load_data_register(register_t reg, unsigned char digit){
    serialin_t data = {
        .HB = reg,
        .LB = digit 
    };
    byteout(data);
    load();
}

//------------------------------------------mode

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

void normal_mode(void){
    serialin_t data = {
        .HB = 0x10,
        .LB = 0x00
    };
    byteout(data);
    load();
}

//----------------------------------------------- highest layer


void init_display(void){
    pinMode(DATA_PIN,OUTPUT);
    pinMode(CLOCK_PIN,OUTPUT);
    pinMode(LOAD_PIN,OUTPUT);

    digitalWrite(DATA_PIN, LOW);
    digitalWrite(CLOCK_PIN, LOW);
    digitalWrite(LOAD_PIN, LOW);

    load_decode_and_digit_setting(true,DIG_ALL);
    load_duty_register(0x02);
}

void set_4digit(unsigned char digits[4]){
    for(int i=0;i<4;i++){
        load_data_register(REG0+(3-i),digits[i]);
    }
}


