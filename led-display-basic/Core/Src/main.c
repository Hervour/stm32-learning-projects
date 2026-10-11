/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "led.h"
#include "tftDisp.h"
#include "zg.h"
#include "timeAPP.h"
#include "key.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define NAME_X             700U
#define NAME_Y              20U
#define STUDENT_ID_X       700U
#define STUDENT_ID_Y        50U
#define BINARY_X           750U
#define BINARY_Y            80U
#define CIRCLE_X           800U
#define CIRCLE_Y0          150U
#define CIRCLE_RADIUS       25U
#define CIRCLE_SPACING      65U
#define PHOTO_X            300U
#define PHOTO_Y            100U
#define DISPLAY_DELAY_MS  1000U
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static const uint8_t led_sequence[] =
{
  
  0x01U, 0x02U, 0x04U, 0x08U, 0x04U, 0x02U, 0x01U,
  0x03U, 0x07U, 0x0FU, 0x0EU, 0x0CU, 0x08U
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */
static void LED_ShowCode(uint8_t code);
static void LCD_ShowCode(uint8_t code);
static void Show_Code(uint8_t code);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  uint32_t sequence_index = 0U;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FSMC_Init();
  MX_SPI2_Init();
  MX_TIM7_Init();
  MX_TIM2_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */
  LED_GPIO_Init();
  W25Q64_CS_GPIO_Init();
  LCD_Initialize();

  Lcd_Clear_All(BLUE_4_4);

  /*Gui_Draw_Asc24_Hzk24(NAME_X, NAME_Y, WHITE_4_4, BLUE_4_4,"������");
  Gui_Draw_Asc24_Hzk24(STUDENT_ID_X, STUDENT_ID_Y, WHITE_4_4, BLUE_4_4,
                       (uint8_t *)"24121980");
  LCD_Draw_Picture(PHOTO_X, PHOTO_Y, gImage_zg);

  Show_Code(0x00U); */
  
  
  HAL_TIM_Base_Start_IT(&htim7);
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);//TIM2计数并且比较产生脉冲信号
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /*for (sequence_index = 0U;
         sequence_index < (sizeof(led_sequence) / sizeof(led_sequence[0]));
         sequence_index++)
    {
      Show_Code(led_sequence[sequence_index]);
      HAL_Delay(DISPLAY_DELAY_MS);
    } */
    
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* TIM7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM7_IRQn, 7, 0);
  HAL_NVIC_EnableIRQ(TIM7_IRQn);
}

/* USER CODE BEGIN 4 */
static void LED_ShowCode(uint8_t code)
{
  uniBit8Byte led_code;

  led_code.abyte = code;

  if (led_code.bits8.b0 != 0U) { LED1_ON(); } else { LED1_OFF(); }
  if (led_code.bits8.b1 != 0U) { LED2_ON(); } else { LED2_OFF(); }
  if (led_code.bits8.b2 != 0U) { LED3_ON(); } else { LED3_OFF(); }
  if (led_code.bits8.b3 != 0U) { LED4_ON(); } else { LED4_OFF(); }
}

static void LCD_ShowCode(uint8_t code)
{
  uint8_t binary[5];
  uniBit8Byte led_code;

  led_code.abyte = code;

  binary[0] = led_code.bits8.b3 ? '1' : '0';
  binary[1] = led_code.bits8.b2 ? '1' : '0';
  binary[2] = led_code.bits8.b1 ? '1' : '0';
  binary[3] = led_code.bits8.b0 ? '1' : '0';
  binary[4] = '\0';

  Gui_Draw_Asc24_Hzk24(BINARY_X, BINARY_Y,
                       WHITE_4_4, BLUE_4_4, binary);

  Gui_Circle_Fill(CIRCLE_X, CIRCLE_Y0,
                  CIRCLE_RADIUS,
                  led_code.bits8.b0 ? RED_4_4 : WHITE_4_4);
  Gui_Circle_Fill(CIRCLE_X, CIRCLE_Y0 + CIRCLE_SPACING,
                  CIRCLE_RADIUS,
                  led_code.bits8.b1 ? RED_4_4 : WHITE_4_4);
  Gui_Circle_Fill(CIRCLE_X, CIRCLE_Y0 + 2U * CIRCLE_SPACING,
                  CIRCLE_RADIUS,
                  led_code.bits8.b2 ? RED_4_4 : WHITE_4_4);
  Gui_Circle_Fill(CIRCLE_X, CIRCLE_Y0 + 3U * CIRCLE_SPACING,
                  CIRCLE_RADIUS,
                  led_code.bits8.b3 ? RED_4_4 : WHITE_4_4);
}

static void Show_Code(uint8_t code)
{
  LED_ShowCode(code);
  LCD_ShowCode(code);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
