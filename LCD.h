#ifndef LCD_H
#define LCD_H
#define MDATA 1
#define MCMD 0
#include <stdint.h>
#include "stm32f1xx_hal.h"
typedef struct {
    GPIO_TypeDef *port_data;
    GPIO_TypeDef *port_control;
    uint16_t D0;
    uint16_t D1;
    uint16_t D2;
    uint16_t D3;
    uint16_t D4;
    uint16_t D5;
    uint16_t D6;
    uint16_t D7;
    uint16_t RS;
    uint16_t RW;
    uint16_t EN;
}LCD;
void LCD_Init(LCD* lcd);
void LCD_Write_Byte(LCD* lcd, uint8_t mode, uint8_t data);
void LCD_Write_Data(LCD* lcd, uint8_t data);
void LCD_Write_Cmd(LCD* lcd, uint8_t cmd);
void LCD_print(LCD* lcd, char* str);
void LCD_Set_Cursor(LCD* lcd, uint8_t row, uint8_t col);
#endif