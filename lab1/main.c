/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions


#define osObjectsPublic  
#include "osObjects.h"                      
#include "cmsis_os.h"
#include "Board_GLCD.h"
#include "GLCD_Config.h"
#include "stm32f2xx_hal.h"
#include <cstring>

int thread_count = 11;
extern    GLCD_FONT GLCD_Font_6x8;
extern    GLCD_FONT GLCD_Font_16x24;
extern int Init_Thread(void);
extern int Init_Mutex(void);
extern int Init_Semaphore(void);
#ifdef RTE_CMSIS_RTOS_RTX
extern   uint32_t   os_time;
uint32_t HAL_GetTick (void) {
  return os_time;
}
#endif

/*method declaration*/
void Print_LCD(char* message);

/*----------------------------------------------------------------------------
 * SystemClock_Config: System Clock Configuration
 *----------------------------------------------------------------------------*/
static void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM       = 25U;
  RCC_OscInitStruct.PLL.PLLN       = 240U;
  RCC_OscInitStruct.PLL.PLLP       = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ       = 5U;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
                                     RCC_CLOCKTYPE_PCLK1  | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);
}





int count_strings=0;
int i=0;
void Print_LCD(char* message)
{	
	
	if (count_strings > 9)
	{
		count_strings=0;
		GLCD_ClearScreen();
	}
	for (i=0; i<100;i++){
		GLCD_DrawString(0U, count_strings*24U, message);
	}
	
	count_strings++;
}


/*
 * main: initialize and start the system
 */
int main (void) {
  osKernelInitialize (); 
  HAL_Init();                           // Initialize the HAL Library
  SystemClock_Config();                 // Configure the System Clock
  SystemCoreClockUpdate();              // Update system clock

  GLCD_Initialize         ();
  GLCD_SetBackgroundColor (GLCD_COLOR_BLACK);
  GLCD_SetForegroundColor (GLCD_COLOR_CYAN);
  GLCD_ClearScreen        ();
  GLCD_SetFont            (&GLCD_Font_16x24);
	//	Init_Thread();
	//	Init_Mutex();
	
	Init_Semaphore();
	osKernelStart ();  

}
