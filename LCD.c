#include "LCD.h"
extern void delay_ms(uint16_t time_ms);
void LCD_Write_Byte(LCD* lcd,uint8_t mode, uint8_t data){
  if (mode == 0) {
    // cmd
    HAL_GPIO_WritePin(lcd->port_control, lcd->RS, 0); // rs = 0
  }
  else if (mode == 1) {
    // data
    HAL_GPIO_WritePin(lcd->port_control, lcd->RS, 1); // rs = 1
  }
  HAL_GPIO_WritePin(lcd->port_data, lcd->D0, (data & 0x01) ? 1 : 0); // d0
  HAL_GPIO_WritePin(lcd->port_data, lcd->D1, (data & 0x02) ? 1 : 0); // d1
  HAL_GPIO_WritePin(lcd->port_data, lcd->D2, (data & 0x04) ? 1 : 0); // d2
  HAL_GPIO_WritePin(lcd->port_data, lcd->D3, (data & 0x08) ? 1 : 0); // d3
  HAL_GPIO_WritePin(lcd->port_data, lcd->D4, (data & 0x10) ? 1 : 0); // d4
  HAL_GPIO_WritePin(lcd->port_data, lcd->D5, (data & 0x20) ? 1 : 0); // d5
  HAL_GPIO_WritePin(lcd->port_data, lcd->D6, (data & 0x40) ? 1 : 0); // d6
  HAL_GPIO_WritePin(lcd->port_data, lcd->D7, (data & 0x80) ? 1 : 0); // d7
  HAL_GPIO_WritePin(lcd->port_control, lcd->EN, 1); // en = 1
  delay_ms(1);
  HAL_GPIO_WritePin(lcd->port_control, lcd->EN, 0); // en = 0
  delay_ms(1);
}
void LCD_Write_Data(LCD* lcd, uint8_t data) {
  LCD_Write_Byte(lcd, 1, data);
}
void LCD_Write_Cmd(LCD* lcd, uint8_t cmd) {
  LCD_Write_Byte(lcd, 0, cmd);
}
void LCD_Init(LCD* lcd) {
  HAL_GPIO_WritePin(lcd->port_control, lcd->RW, 0); // rw = 0
  delay_ms(50); // chờ lcd khởi động
  LCD_Write_Cmd(lcd, 0x38); // chế đọn 8 bit 2 dòng font 5x8
  delay_ms(5);
  LCD_Write_Cmd(lcd, 0x0C); // bật display, tắt con trỏ
  delay_ms(5);
  LCD_Write_Cmd(lcd, 0x01); // xóa màn hình
  delay_ms(5);
  LCD_Write_Cmd(lcd, 0x06); // tự động tăng con trỏ sau khi ghi dữ liệu
}
void LCD_print(LCD* lcd, char* str) {
  while(*str) {
    LCD_Write_Data(lcd, *str ++);
  }
}
void LCD_Set_Cursor(LCD* lcd, uint8_t row, uint8_t col) {
  uint8_t address = (row == 0) ? col : (0x40 + col);
  LCD_Write_Cmd(lcd, 0x80 | address);
}