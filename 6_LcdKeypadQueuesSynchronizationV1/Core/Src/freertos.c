/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "LCD_interface.h"
#include "KEYPAD_interface.h"
#include "queue.h"

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
/* USER CODE BEGIN Variables */

uint8_t keypadVal = NOTPRESSED;

/* USER CODE END Variables */
/* Definitions for task1 */
osThreadId_t task1Handle;
const osThreadAttr_t task1_attributes = {
  .name = "task1",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for task2 */
osThreadId_t task2Handle;
const osThreadAttr_t task2_attributes = {
  .name = "task2",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for myQueue01 */
osMessageQueueId_t myQueue01Handle;
const osMessageQueueAttr_t myQueue01_attributes = {
  .name = "myQueue01"
};
/* Definitions for myBinarySem01 */
osSemaphoreId_t myBinarySem01Handle;
const osSemaphoreAttr_t myBinarySem01_attributes = {
  .name = "myBinarySem01"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */


/* USER CODE END FunctionPrototypes */

void vLcdTask(void *argument);
void vKeypadTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of myBinarySem01 */
  myBinarySem01Handle = osSemaphoreNew(1, 0, &myBinarySem01_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of myQueue01 */
  myQueue01Handle = osMessageQueueNew (5, sizeof(uint8_t), &myQueue01_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */

  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of task1 */
  task1Handle = osThreadNew(vLcdTask, NULL, &task1_attributes);

  /* creation of task2 */
  task2Handle = osThreadNew(vKeypadTask, NULL, &task2_attributes);

  /* USER CODE BEGIN RTOS_THREADS */



  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_vLcdTask */
/**
  * @brief  Function implementing the task1 thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_vLcdTask */
void vLcdTask(void *argument)
{
	/* USER CODE BEGIN vLcdTask */
	BaseType_t RetVal = pdTRUE;
	uint8_t message = 0;
	/* Infinite loop */
	for(;;)
	{
		RetVal= xQueueReceive(myQueue01Handle, &message, HAL_MAX_DELAY);
		lcd_4bit_send_char_data_pos(&lcd_1, 1, 14, message);
		osSemaphoreRelease(myBinarySem01Handle);
	}
	/* USER CODE END vLcdTask */
}

/* USER CODE BEGIN Header_vKeypadTask */
/**
* @brief Function implementing the task2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_vKeypadTask */
void vKeypadTask(void *argument)
{
	/* USER CODE BEGIN vKeypadTask */
	BaseType_t RetVal = pdTRUE;
	/* Infinite loop */
	for(;;)
	{
		keypadVal = NOTPRESSED;
		keypadVal = read_keypad(&keypad_1);
		if(keypadVal != NOTPRESSED)
		{
			LCD_Clear(&lcd_1);
			if(keypadVal<'1' || keypadVal>'9') lcd_4bit_send_string_pos(&lcd_1, 1, 1, "Direction => ");
			else lcd_4bit_send_string_pos(&lcd_1, 1, 1, "Character => ");
			RetVal = xQueueSendToFront(myQueue01Handle, (void *)&keypadVal, HAL_MAX_DELAY);
			osSemaphoreAcquire(myBinarySem01Handle, HAL_MAX_DELAY);
		}
	}
	/* USER CODE END vKeypadTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */



/* Task to be created. */


/* USER CODE END Application */

