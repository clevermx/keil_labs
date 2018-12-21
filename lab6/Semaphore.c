#include <stdlib.h>
#include "cmsis_os.h"                                           // CMSIS RTOS header file

/*----------------------------------------------------------------------------
 *      Semaphore creation & usage
 *---------------------------------------------------------------------------*/
 
extern osThreadId* tid_arr;  											// thread id
extern  int thread_count;
extern void FuncA (int cnt); 
extern void FuncB_new (int LEDn);
int flag=0; 
void Thread_Semaphore (void const *argument);                   // thread function
osThreadId tid_Thread_Semaphore;                                // thread id
osThreadDef (Thread_Semaphore, osPriorityNormal, 1, 0);         // thread object

osSemaphoreId sid_write_count;                             // semaphore id
osSemaphoreDef (count_sem);                               // semaphore object


int Init_Semaphore (void) {

  int i =0;
	int* arg;
	tid_arr = (osThreadId*) malloc((thread_count +1) * sizeof(osThreadId));   
	arg=(int*) malloc(sizeof(int));
	arg[0]=0;  
  for( i= 1; i<=thread_count;i++){   
		arg=(int*) malloc(sizeof(int));
		arg[0]=i;		
		tid_arr[i] = osThreadCreate (osThread(Thread_Semaphore), arg);
	}	
	sid_write_count = osSemaphoreCreate (osSemaphore(count_sem), 1);
   return(0);
}

void Thread_Semaphore (void const *argument) {
int thr_num=*((int* )argument);	
	int i;
 
  while (1) { 
		FuncA(thr_num);
		if (flag != (flag | (1<<thr_num)) ){
    osSemaphoreWait (sid_write_count, osWaitForever);           
		flag|=1<<thr_num;
		if (flag == 0xE) {
			flag=0;
			for(i=0;i<=thread_count;i++){
				osSignalSet(tid_arr[i],0x01);
			}	
		}
		osSemaphoreRelease (sid_write_count);              // Return a token back to a semaphore
	}
		osSignalWait(0x01,osWaitForever);
		osSignalClear(tid_arr[thr_num],0x01);
		FuncB_new(thr_num);
       
	}
  
}
