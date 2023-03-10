
#include "tb62709.h"


const unsigned char SEGMENT[] = {
    0x01,// a
    0x02,// b
    0x04,// c
    0x08,// d
    0x10,// e
    0x20,// f
    0x40,// g
    0x80,// dp
};

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

void load_data_register(unsigned char reg, unsigned char digit){
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

void standby_mode(bool dataclear_enb){
    unsigned char dataclear = 0x00;
    if(dataclear_enb){
        dataclear |= 0x01;
    }
    serialin_t data = {
        .HB = 0x40,
        .LB = dataclear
    };
    byteout(data);
    load();
}

//----------------------------------------------- highest layer

void set_4digit(unsigned char digits[4]){
    for(unsigned char i=0;i<4;i++){
        load_data_register((unsigned char)(REG0+i),digits[i]);
    }
}

void set_4digit_dp(unsigned char digits[4],int dp){
    unsigned char dps[4];
    dps[dp] = 0x40;
    load_data_register(REG0 + 0,(digits[0] | dps[0]));
    load_data_register(REG0 + 1,(digits[1] | dps[1]));
    load_data_register(REG0 + 2,(digits[2] | dps[2]));
    load_data_register(REG0 + 3,(digits[3] | dps[3]));
}

