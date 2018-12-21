//#include "stm32f2xx.h"
//#include <cmsis_os.h>
//#include <stdlib.h>
//int thread_count=8;
//osThreadId* tid_arr;  
//void delay(int n)
//{
//	unsigned int i;
//	for ( i = 0; i < n*40000; i++) {}
//}		

//void LEDfuncG(int ODRnum)
//{
//	GPIOG->ODR |= 1 << ODRnum;
//	delay(ODRnum);
//	
//	GPIOG->ODR &= ~1 << ODRnum;
//	delay(ODRnum);
//}
//void LEDfuncH(int ODRnum)
//{
//	GPIOH->ODR |= 1 << ODRnum;
//	delay(ODRnum);
//	
//	GPIOH->ODR &= ~1 << ODRnum;
//	delay(ODRnum);
//}
//void LEDfuncI(int ODRnum)
//{
//	GPIOI->ODR |= 1 << ODRnum;
//	delay(ODRnum);
//	
//	GPIOI->ODR &= ~1 << ODRnum;
//	delay(ODRnum);
//}

//osMutexId mutex;
//osMutexDef (mutex);

//void threadG (void const *argument) {
//	unsigned int i;
//	for (;;) {
//		osMutexWait(mutex, osWaitForever);
//		for( i = 0; i < 5; i++)
//			LEDfuncG((int) argument);
//		
//		osMutexRelease(mutex);
//	}
//}
//void threadH (void const *argument) {
//	unsigned int i;
//	for (;;) {
//		osMutexWait(mutex, osWaitForever);
//		for( i = 0; i < 5; i++)
//			LEDfuncH((int) argument);
//		
//		osMutexRelease(mutex);
//	}
//}
//void threadI (void const *argument) {
//	unsigned int i;
//	for (;;) {
//		osMutexWait(mutex, osWaitForever);
//		for( i = 0; i < 5; i++)
//			LEDfuncI((int) argument);
//		
//		osMutexRelease(mutex);
//	}
//}
//osThreadDef (threadG, osPriorityNormal, 5, 0);
//osThreadDef (threadH, osPriorityNormal, 5, 0);
//osThreadDef (threadI, osPriorityNormal, 5, 0);


//int main ()
//{
//	osKernelInitialize();
//	
//	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN | RCC_AHB1ENR_GPIOHEN | RCC_AHB1ENR_GPIOIEN;
//	GPIOG->MODER = 0;
//	GPIOH->MODER = 0;
//	GPIOI->MODER = 0;
//	GPIOG->MODER = GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0;
//	GPIOH->MODER = GPIO_MODER_MODER2_0 |GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0;
//	GPIOI->MODER = GPIO_MODER_MODER10_0 ;
//	tid_arr = (osThreadId*) malloc((thread_count +1) * sizeof(osThreadId));   
//	tid_arr[0] = osThreadCreate (osThread(threadH), (void*)3);
//	tid_arr[1] = osThreadCreate (osThread(threadH), (void*)6);
//	tid_arr[2] = osThreadCreate (osThread(threadH), (void*)7);
//	tid_arr[3] = osThreadCreate (osThread(threadI), (void*)10);
//	tid_arr[4] = osThreadCreate (osThread(threadG), (void*)6);
//	tid_arr[5] = osThreadCreate (osThread(threadG), (void*)7);
//	tid_arr[6] = osThreadCreate (osThread(threadG), (void*)8);
//	tid_arr[7] = osThreadCreate (osThread(threadH), (void*)2);
//	mutex = osMutexCreate(osMutex(mutex));

//	
//	osKernelStart();
//}
