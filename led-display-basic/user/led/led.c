#include "led.h"

void LED_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  RCC_LED1_GPIO_CLK_EN();
  RCC_LED2_GPIO_CLK_EN();
  RCC_LED3_GPIO_CLK_EN();
  RCC_LED4_GPIO_CLK_EN();
  RCC_PG10_GPIO_CLK_EN();

  LED_ALL_OFF();

  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

  GPIO_InitStruct.Pin = LED1_PINx;
  HAL_GPIO_Init(LED1_GPIOx, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LED2_PINx;
  HAL_GPIO_Init(LED2_GPIOx, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LED3_PINx;
  HAL_GPIO_Init(LED3_GPIOx, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LED4_PINx;
  HAL_GPIO_Init(LED4_GPIOx, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = PG10_PINx;
  HAL_GPIO_Init(PG10_GPIOx, &GPIO_InitStruct);
}