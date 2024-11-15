/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
void display7SEG(int num);
void update7SEG(int index);
void updateClockBuffer(int hour, int minute);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE BEGIN 0 */
const int MAX_LED = 4;
int index_led = 0;
int led_buffer[4] = {1, 2, 3, 4};


int timer0_counter = 0;       // Counter to track the countdown
int timer0_flag = 0;          // Flag that signals when timer has expired
int TIMER_CYCLE = 10;         // Timer cycle in ms (same as the interrupt period)

void setTimer0(int duration) {
    timer0_counter = duration / TIMER_CYCLE;  // Calculate countdown cycles
    timer0_flag = 0;                          // Reset the flag to 0
}

void timer_run() {
    if (timer0_counter > 0) {
        timer0_counter--;                     // Decrease the counter
        if (timer0_counter == 0) {
            timer0_flag = 1;                  // Set the flag when countdown completes
        }
    }
}
/* USER CODE END 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

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
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  int seven_segment_counter = 250;   // Update every 250ms for 4 segments in 1Hz cycle
  int LED_counter = 1000;            // Blink every 1000ms

  setTimer0(10);                     // Start the timer with 10ms cycle

  while (1) {
      if (timer0_flag == 1) {
          setTimer0(10);             // Reset the software timer for the next 10ms cycle

          // 7-segment display update logic
          seven_segment_counter -= TIMER_CYCLE;
          if (seven_segment_counter <= 0) {
              seven_segment_counter = 250;       // Reset counter for 1 Hz cycle across 4 segments

              if (index_led >= MAX_LED) index_led = 0;
              update7SEG(index_led++);           // Update the current segment and move to the next
          }

          // LED blink logic
          LED_counter -= TIMER_CYCLE;
          if (LED_counter <= 0) {
              LED_counter = 1000;                // Reset counter for LED blink
              HAL_GPIO_TogglePin(DOT_GPIO_Port, DOT_Pin);  // Toggle the DOT LED
          }
      }
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, DOT_Pin|LED_RED_Pin|EN0_Pin|EN1_Pin
                          |EN2_Pin|EN3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pins : DOT_Pin LED_RED_Pin EN0_Pin EN1_Pin
                           EN2_Pin EN3_Pin */
  GPIO_InitStruct.Pin = DOT_Pin|LED_RED_Pin|EN0_Pin|EN1_Pin
                          |EN2_Pin|EN3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB2 PB3
                           PB4 PB5 PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2) {  // Confirm the callback is for TIM2
        timer_run();               // Run software timer every interrupt
    }
}


void update7SEG(int index) {
    // Reset all enable pins first to turn off other displays
    HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, GPIO_PIN_SET);

    // Select the correct display based on index and show the corresponding value
    switch (index) {
        case 0:
            HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, GPIO_PIN_RESET);  // Enable first display
            display7SEG(led_buffer[0]);                                 // Display led_buffer[0]
            break;
        case 1:
            HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_RESET);  // Enable second display
            display7SEG(led_buffer[1]);                                 // Display led_buffer[1]
            break;
        case 2:
            HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, GPIO_PIN_RESET);  // Enable third display
            display7SEG(led_buffer[2]);                                 // Display led_buffer[2]
            break;
        case 3:
            HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, GPIO_PIN_RESET);  // Enable fourth display
            display7SEG(led_buffer[3]);                                 // Display led_buffer[3]
            break;
        default:
            break;
    }
}

void updateClockBuffer(int hour, int minute){
	led_buffer[0] = hour / 10;
	led_buffer[1] = hour % 10;
	led_buffer[2] = minute / 10;
	led_buffer[3] = minute % 10;
}

void display7SEG(int num)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_All, SET);

	switch(num){
	case 0:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, RESET);
		break;
	case 1:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, RESET);
		break;
	case 2:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, RESET);
		break;
	case 3:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, RESET);
		break;
	case 4:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, RESET);
		break;
	case 5:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, RESET);
		break;
	case 6:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, RESET);
		break;
	case 7:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, RESET);
		break;
	case 8:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, RESET);
		break;
	case 9:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, RESET);
		break;
	}
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

#ifdef  USE_FULL_ASSERT
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