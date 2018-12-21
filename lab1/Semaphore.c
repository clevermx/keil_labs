#include <stdlib.h>
#include "cmsis_os.h"                                           // CMSIS RTOS header file

/*----------------------------------------------------------------------------
 *      Semaphore creation & usage
 *---------------------------------------------------------------------------*/
 
 
 
extern void Print_LCD(char* message);
extern osThreadId* tid_arr;  											// thread id
extern  int thread_count;


void Thread_Semaphore (void const *argument);                   // thread function
osThreadDef (Thread_Semaphore, osPriorityNormal, 1, 0);         // thread object
osSemaphoreId sid_Thread_Semaphore;                             // semaphore id
osSemaphoreDef (mydef);                               // semaphore object


int Init_Semaphore (void) {
	int i =0;
	int* arg;
	tid_arr = (osThreadId*) malloc((thread_count +1) * sizeof(osThreadId));   
	arg=(int*) malloc(sizeof(int));
	arg[0]=0;  
  for( i= 0; i<thread_count;i++){   
	arg=(int*) malloc(sizeof(int));
	arg[0]=i;		
	tid_arr[i] = osThreadCreate (osThread(Thread_Semaphore), arg);
	}	
	sid_Thread_Semaphore = osSemaphoreCreate (osSemaphore(mydef), 1);
   return(0);
}

void Thread_Semaphore (void const *argument) { 
	int thr_num=*((int* )argument);	
	char* buff;
 buff= (char*) malloc(sizeof(char) * 5);
	buff[0]='t';
	buff[1]='h';
	buff[2]='r';
	buff[3]= '0' + thr_num;
	buff[4]='\0';
  while (1) { 
    osSemaphoreWait (sid_Thread_Semaphore, osWaitForever);           // wait 10 mSec
		osDelay(1000);	
		Print_LCD(buff);
		osSemaphoreRelease (sid_Thread_Semaphore);              // Return a token back to a semaphore
       
	}
  
}
