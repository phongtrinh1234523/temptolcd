#ifndef dht11_h
#define dht11_h
#include "main.h"
typedef struct {
    /* data */
    GPIO_TypeDef* GPIOx;
    uint16_t pin;
    uint8_t int_Hum;
    uint8_t float_Hum;
    uint8_t int_temp;
    uint8_t float_temp;
    uint8_t checkSum;
}dht11;
uint8_t dht11Read(dht11* dht);
void handle(dht11* dht);
#endif
