#include <cmsis_os.h>
#include "stm32f2xx.h"
extern int Init_Semaphore(void);
int thread_count=3;
osThreadId* tid_arr;  		
void delay ()
{
	unsigned int i;
	for (i = 0; i < 100000; i++) {}
}

/* wait for cnt turns */
void FuncA (int cnt)
{
	int i;
	for (i=0; i<cnt; i++) delay();
}

/* LED function */
void FuncB_new (int LEDn)
{	
	
		GPIOG->ODR |= 1<<(5+LEDn);
		delay();
		
		GPIOG->ODR &= ~1<<(5+LEDn);
		delay();
	
}
void FuncB (int LEDn)
{	
		GPIOG->ODR |= 1<<LEDn;
		delay();
		
		GPIOG->ODR &= ~1<<LEDn;
		delay();
}

osSemaphoreId arrived1, arrived2;
osSemaphoreDef(arrived1);
osSemaphoreDef(arrived2);

/* waits 5 turns, then LED func */
void thread1 (void const *argument)
{	
	while (1)
	{	
		FuncA(5);
	  osSemaphoreRelease(arrived1);
	  osSemaphoreWait(arrived2, osWaitForever);
		FuncB(6);
	}
}

/* waits 10 turns, then LED function */
void thread2 (void const *argument)
{
	while (1)
	{	
		FuncA(10);
	  osSemaphoreRelease(arrived2);
	  osSemaphoreWait(arrived1, osWaitForever);
		FuncB(7);
	}
}

osThreadDef (thread1, osPriorityNormal, 1, 0);
osThreadDef (thread2, osPriorityNormal, 1, 0);

void InitializeLED(){
//LEDS
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
	GPIOG->MODER = 0;
	GPIOG->MODER |= GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0;
	//
}
int main () 
{
//	arrived1 = osSemaphoreCreate(osSemaphore(arrived1), 0);
//	arrived2 = osSemaphoreCreate(osSemaphore(arrived2), 0);
	InitializeLED();
	osKernelInitialize ();
	Init_Semaphore();
//	osThreadCreate (osThread(thread1), NULL);
//	osThreadCreate (osThread(thread2), NULL);
	
	osKernelStart ();  
}


