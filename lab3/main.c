/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
 #include <stdio.h>
 #include "stm32f2xx.h"
#include "stm32f2xx_hal.h"
#define osObjectsPublic    // define objects in main module
#include "osObjects.h"      // RTOS object definitions
extern int Init_Thread(void);
extern int Init_Mutex(void);
extern int Init_MsgQueue(void);
extern int Init_Semaphore(void);
int thread_count = 3;
osThreadId* tid_arr; 
/*
 * main: initialize and start the system
 */
 
 /*--------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *-------------------------------------------------------*/


/*----------------------------------------------------------
 * SystemClock_Config: System Clock Configuration
 *-----------------------------------------------------*/
static void SystemClock_Config (void) {
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;


  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 240;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);


  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
                                RCC_CLOCKTYPE_PCLK1  | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);
}

/*----------------------------------------------------------
 * GPIO Initialization
 *-----------------------------------------------------*/

GPIO_InitTypeDef GPIO_InitStruct;
void InitializeLEDdisplay()
{
RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
	RCC->AHB1ENR |=RCC_AHB1ENR_GPIOBEN;
RCC->AHB1ENR |= RCC_AHB1ENR_GPIOIEN;
/* GPIO base configuration */
GPIO_InitStruct.Pin = (GPIO_PIN_7|GPIO_PIN_6);
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7,GPIO_PIN_RESET);
HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6,GPIO_PIN_RESET);	
}

void InitializeLED(){
//LEDS
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
	GPIOG->MODER = 0;
	GPIOG->MODER |= GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0;
	//
}

GPIO_InitTypeDef GPIO_Init_USART;
USART_InitTypeDef USART_InitUser;
USART_HandleTypeDef USART_HandleUser;

void InitializeGPIOforUSART()
{

RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
RCC->APB2ENR |= RCC_APB2ENR_USART1EN  ; 
/* GPIO base configuration */
	
GPIO_Init_USART.Pin = (GPIO_PIN_6);
GPIO_Init_USART.Mode = GPIO_MODE_AF_PP ;
GPIO_Init_USART.Speed = GPIO_SPEED_FAST;
GPIO_Init_USART.Pull=GPIO_PULLUP;
GPIO_Init_USART.Alternate =GPIO_AF7_USART1 ;
	
HAL_GPIO_Init(GPIOB, &GPIO_Init_USART);
	
GPIO_Init_USART.Pin = (GPIO_PIN_7);
GPIO_Init_USART.Mode = GPIO_MODE_AF_PP ;
GPIO_Init_USART.Speed = GPIO_SPEED_FAST;
GPIO_Init_USART.Pull=GPIO_PULLUP;
GPIO_Init_USART.Alternate =GPIO_AF7_USART1 ;
	
HAL_GPIO_Init(GPIOB, &GPIO_Init_USART);



	USART_InitUser.BaudRate=9600;
	USART_InitUser.WordLength=USART_WORDLENGTH_8B;
	USART_InitUser.StopBits=USART_STOPBITS_1;
	USART_InitUser.Parity=USART_PARITY_NONE;
	USART_InitUser.Mode=USART_MODE_TX_RX;
	

  USART_HandleUser.Instance=USART1;
	USART_HandleUser.Init=USART_InitUser;
	HAL_USART_Init(&USART_HandleUser);
		
}
void send_to_uart(uint8_t data, uint8_t size)
{
 HAL_USART_Transmit(&USART_HandleUser, &data ,size, 1);
	
}

void send_str(char * string)
{
 uint8_t i=0;
 while(string[i]) 
{
  send_to_uart(string[i],1);
  i++;
 }
send_to_uart('\n',1);
send_to_uart('\r',1);
}
void delay ()
{
	unsigned int i;
	for (i = 0; i < 10000000; i++) {}
}
void LED_On (int ODRnum)
{
		GPIOG->ODR |= 1 << ODRnum;
		delay();
}

void LED_Off (int ODRnum)
{
		delay();
		GPIOG->ODR &= ~1 << ODRnum;
		delay();

}
 
 
 /*
int main (void) {
  osKernelInitialize ();                    // initialize CMSIS-RTOS

  // initialize peripherals here

  // create 'thread' functions that start executing,
  // example: tid_name = osThreadCreate (osThread(name), NULL);

  osKernelStart ();                         // start thread execution 
}
*/


int main(void)
{
	uint8_t i =156;
	uint8_t size = 1;
	  osKernelInitialize (); 
  HAL_Init();                           // Initialize the HAL Library
  SystemClock_Config();                 // Configure the System Clock
  SystemCoreClockUpdate();              // Update system clock
  SystemClock_Config();                 // Configure the System Clock
  SystemCoreClockUpdate();              // Update system clock	
  InitializeLED();
	//InitializeLEDdisplay();
   //InitializeGPIOforUSART();
 
	// 	Init_Thread();
	//	Init_Mutex();
	
	//Init_Semaphore();
	Init_MsgQueue();
	osKernelStart ();  
}
