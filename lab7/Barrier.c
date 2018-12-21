#include "stm32f2xx.h"
#include <cmsis_os.h>
#include <stdlib.h>
int thread_count=8;
osThreadId* tid_arr; 
int on_diods=8;
void delay (int n)
{
	unsigned int i;
	for (i=0; i< n * 50000; i++) {}
}

void LEDfunc (int* arg)
{
	if (arg[0]==0) {
		GPIOH->ODR |= 1 << (arg[1] );
		delay(4);
		GPIOH->ODR &= ~1 << (arg[1]);
		
	}	
	if (arg[0]==1) {
		GPIOG->ODR |= 1 << (arg[1]);
		delay(4);
		GPIOG->ODR &= ~1 << (arg[1]);
		
	}	
		if (arg[0]==2) {
		GPIOI->ODR |= 1 << (arg[1]);
		delay(4);
		GPIOI->ODR &= ~1 << (arg[1]);
		
	}	
}
void LEDfuncON (int* arg)
{
	if (arg[0]==0) {
		GPIOH->ODR |= 1 << (arg[1] );	
		
	}	
	if (arg[0]==1) {
		GPIOG->ODR |= 1 << (arg[1]);		
		
	}	
		if (arg[0]==2) {
		GPIOI->ODR |= 1 << (arg[1]);		
		
	}	
}
void LEDfuncOF (int* arg)
{
	if (arg[0]==0) {
			GPIOH->ODR &= ~1 << (arg[1]);
		
	}	
	if (arg[0]==1) {
			GPIOG->ODR &= ~1 << (arg[1]);
		
	}	
		if (arg[0]==2) {
	GPIOI->ODR &= ~1 << (arg[1]);
		
	}	
}
osSemaphoreId  mutex;

osSemaphoreDef(mutex);

unsigned int count = 0;
	
void thread(void const *argument)
{	
	int* arg= (int*) argument;
	while(1)
	{
		osSignalWait(0x01,osWaitForever);	
		if (arg[2]!= (on_diods-1)){				
		LEDfunc((int*)argument);
			
		osSignalSet(tid_arr[arg[2]+1],0x01);
		}
		else{
			LEDfuncON(arg);
			osSemaphoreWait(mutex, 0xffff);  // allow only one task to increment the counter and access this turnstile
			on_diods--;
			if (on_diods == 0){
			 on_diods=thread_count;
				
			 delay(4);
				GPIOI->ODR = 0;
				GPIOH->ODR = 0;
				GPIOG->ODR = 0;
					
			}
			
				osSignalSet(tid_arr[0],0x01);
				
			osSemaphoreRelease(mutex);
		}				
	}
				
	
}

osThreadDef (thread, osPriorityNormal, 5, 0);


int main ()
{
	int **mas_arg;

	int i =0;
	osKernelInitialize ();
	
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN | RCC_AHB1ENR_GPIOHEN| RCC_AHB1ENR_GPIOIEN;
	GPIOG->MODER = 0;
	GPIOH->MODER = 0;
	GPIOI->MODER = 0;
	GPIOG->MODER = GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0;
	GPIOH->MODER = GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0|GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER10_0;
	GPIOI->MODER = GPIO_MODER_MODER10_0 ;
	
tid_arr = (osThreadId*) malloc((thread_count +1) * sizeof(osThreadId));   
	mutex = osSemaphoreCreate(osSemaphore(mutex), 1);
	mas_arg= (int**) malloc((8) * sizeof(int*));
	for(  i=0; i<thread_count; i++){
	 mas_arg[i] = (int*) malloc((3) * sizeof(int));
	}
	mas_arg[0][0]= 0;
	mas_arg[0][1]= 3;
	mas_arg[0][2]= 0;
	mas_arg[1][0]= 0;
	mas_arg[1][1]= 6;
	mas_arg[1][2]= 1;
	mas_arg[2][0]= 0;
	mas_arg[2][1]= 7;
	mas_arg[2][2]= 2;
	mas_arg[3][0]= 2;
	mas_arg[3][1]= 10;
	mas_arg[3][2]= 3;
	mas_arg[4][0]= 1;
	mas_arg[4][1]= 6;
	mas_arg[4][2]= 4;
	mas_arg[5][0]= 1;
	mas_arg[5][1]= 7;
	mas_arg[5][2]= 5;
	mas_arg[6][0]= 1;
	mas_arg[6][1]= 8;
	mas_arg[6][2]= 6;
	mas_arg[7][0]= 0;
	mas_arg[7][1]= 2;
  mas_arg[7][2]= 7;	
	
	for(  i=0; i<thread_count; i++){
	 tid_arr[i] = osThreadCreate (osThread(thread), (void*)mas_arg[i]);
	 osSignalClear(tid_arr[i],0x01);
	}
	
	osKernelStart ();
	osSignalSet(tid_arr[0],0x01);
}
