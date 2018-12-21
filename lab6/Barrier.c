#include "stm32f2xx.h"
#include <cmsis_os.h>

void delay (int n)
{
	unsigned int i;
	for (i=0; i< n * 50000; i++) {}
}

void LEDfunc (int ODRnum)
{
	
	if (ODRnum < 4)
	{
		GPIOG->ODR |= 1 << (ODRnum + 5);
		delay(4);
	
		GPIOG->ODR &= ~1 << (ODRnum + 5);
		delay(ODRnum);
	}
	else
	{
		GPIOH->ODR |= 1 << (ODRnum - 2);
		delay(4);
	
		GPIOH->ODR &= ~1 << (ODRnum - 2);
		delay(ODRnum);
	}
}

osSemaphoreId barrier1, barrier2, mutex;
osSemaphoreDef(barrier1);
osSemaphoreDef(barrier2);
osSemaphoreDef(mutex);

unsigned int count = 0;
	
void thread(void const *argument)
{	
	while(1)
	{
		// START CYCLE - BARRIER //
		
		/*
		osSemaphoreWait(mutex, 0xffff);  // allow only one task to increment the counter and access this turnstile
		count = count + 1;
		if (count == 5)
		{
			osSemaphoreWait(barrier2, 0xffff);
			osSemaphoreRelease(barrier1);  // create a new token to allow tasks to pass through
		}
		osSemaphoreRelease(mutex);       // next task can access the turnstile now
		osSemaphoreWait(barrier1, 0xffff);
		osSemaphoreRelease(barrier1);
		*/
				
		LEDfunc((int)argument);
		
		// END CYCLE - BARRIER //
		
		/*
		osSemaphoreWait(mutex, 0xffff);
		count = count - 1;
		if (count == 0)
		{
			osSemaphoreWait(barrier1, 0xffff);
			osSemaphoreRelease(barrier2);
		}
		osSemaphoreRelease(mutex);
		osSemaphoreWait(barrier2, 0xffff);
		osSemaphoreRelease(barrier2);
		*/
	}		
}

osThreadDef (thread, osPriorityNormal, 5, 0);
osThreadId thr1, thr2, thr3, thr4, thr5;

int main ()
{
	osKernelInitialize ();
	
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN | RCC_AHB1ENR_GPIOHEN;
	GPIOG->MODER = 0;
	GPIOG->MODER |= GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0;
	GPIOH->MODER = 0;
	GPIOH->MODER |= GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0;
	
	barrier1 = osSemaphoreCreate(osSemaphore(barrier1), 0);
	barrier2 = osSemaphoreCreate(osSemaphore(barrier2), 1);
	mutex = osSemaphoreCreate(osSemaphore(mutex), 1);
	
	thr1 = osThreadCreate(osThread(thread), (void*) 1);
	thr2 = osThreadCreate(osThread(thread), (void*) 2);
	thr3 = osThreadCreate(osThread(thread), (void*) 3);
	thr4 = osThreadCreate(osThread(thread), (void*) 4);
	thr5 = osThreadCreate(osThread(thread), (void*) 5);
	
	osKernelStart ();
}
