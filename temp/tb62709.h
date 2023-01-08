#ifndef TB62709_HEADER_FILE
#define TB62709_HEADER_FILE

#include <Arduino.h>

#define DATA_PIN 2
#define CLOCK_PIN 3 
#define LOAD_PIN 4

extern const unsigned char SEGMENT[8]; 

typedef union{
    typedef struct{
        unsigned char digit0;
        unsigned char digit1;
        unsigned char digit2;
        unsigned char digit3;
    }digits;
    unsigned char array[4];
}four_digits_t;


typedef struct{
    unsigned char HB;
    unsigned char LB;
}serialin_t;

typedef enum{
    DIG0 = 0x00,
    DIG01 = 0x01,
    DIG012 = 0x02,
    DIG_ALL = 0x03
}digit_act_t;

const unsigned char REG0 = 0x22;
//typedef enum{
//    REG0 = 0x22,
//    REG1 = 0x23,
//    REG2 = 0x24,
//    REG3 = 0x25,
//}register_t;

void load_duty_register(unsigned char duty);
void load_decode_and_digit_setting(bool decodeEnable,digit_act_t digit_enb);
void load_data_register(unsigned char reg, unsigned char digit);

void all_on_mode(void);
void blank_mode(void);
void normal_mode(void);

void init_display(void);
void set_nodecode(void);
void set_decode(void);

void set_4digit(unsigned char digits[4]);
void set_4digit_dp(unsigned char digits[4],int dp);

#endif //TB62709_HEADER_FILE
