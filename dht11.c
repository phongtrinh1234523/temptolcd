#include "dht11.h"
extern void delay_us(uint16_t time);
extern void delay_ms(uint16_t time_ms);

static uint8_t dht11WaitForEdge(dht11* dht, GPIO_PinState state, uint32_t timeout_us) {
  while (HAL_GPIO_ReadPin(dht->GPIOx, dht->pin) != state) {
    if (timeout_us-- == 0) {
      return 0;
    }
    delay_us(1);
  }
  return 1;
}

void setToOutput(dht11* dht) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = dht->pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(dht->GPIOx, &GPIO_InitStruct);
}

void setToInput(dht11* dht) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = dht->pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(dht->GPIOx, &GPIO_InitStruct);
}

static uint8_t dht11Start(dht11* dht) {
  setToOutput(dht);
  HAL_GPIO_WritePin(dht->GPIOx, dht->pin, GPIO_PIN_RESET);
  delay_ms(20);
  HAL_GPIO_WritePin(dht->GPIOx, dht->pin, GPIO_PIN_SET);
  delay_us(30);
  setToInput(dht);

  if (!dht11WaitForEdge(dht, GPIO_PIN_RESET, 1000)) {
    return 0;
  }
  if (!dht11WaitForEdge(dht, GPIO_PIN_SET, 1000)) {
    return 0;
  }
  return 1;
}

uint8_t dht11ReadByte(dht11* dht) {
  uint8_t res = 0;
  for (int i = 0; i < 8; i ++) {
    if (!dht11WaitForEdge(dht, GPIO_PIN_RESET, 1000)) {
      return 0;
    }
    if (!dht11WaitForEdge(dht, GPIO_PIN_SET, 1000)) {
      return 0;
    }
    delay_us(40);
    if (HAL_GPIO_ReadPin(dht->GPIOx, dht->pin) == GPIO_PIN_SET) {
      res |= (1 << (7 - i));
    }
  }
  return res;
}

uint8_t dht11Read(dht11* dht) {
  for (int attempt = 0; attempt < 3; attempt++) {
    if (!dht11Start(dht)) {
      delay_ms(200);
      continue;
    }

    dht->int_Hum = dht11ReadByte(dht);
    dht->float_Hum = dht11ReadByte(dht);
    dht->int_temp = dht11ReadByte(dht);
    dht->float_temp = dht11ReadByte(dht);
    dht->checkSum = dht11ReadByte(dht);

    if (dht->checkSum == dht->int_Hum + dht->float_Hum + dht->int_temp + dht->float_temp) {
      return 1;
    }
    delay_ms(200);
  }
  return 0;
}