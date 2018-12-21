#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include <stdlib.h>
#include <string.h>
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
extern void send_str(char * string);
void work_thread (void const *argument);                             // thread function
void master_thread (void const *argument);   
extern int thread_count ;
extern osThreadId* tid_arr;
 osThreadDef (master_thread, osPriorityNormal, 1, 0); 
 osThreadDef (work_thread, osPriorityNormal, 1, 0);  

int Init_Thread (void) {
	int i =0;
	int* arg;
	tid_arr = (osThreadId*) malloc((thread_count +1) * sizeof(osThreadId));
	    
  arg=(int*) malloc(sizeof(int));
	arg[0]=0;
		tid_arr[0] = osThreadCreate (osThread(master_thread),arg);
		if (!tid_arr[i]) return(-1);
	for( i= 1; i<thread_count;i++){   
		arg=(int*) malloc(sizeof(int));
		arg[0]=i;		
		tid_arr[i] = osThreadCreate (osThread(work_thread), arg);
		if (!tid_arr[i]) return(-1);
	}	
  osSignalSet(tid_arr[0],0x01);
  return(0);
}



//


void work_thread(void const *argument){
	int thr_num=*((int* )argument);		
	char* buff;
 buff= (char*) malloc(sizeof(char) * 5);
	buff[0]='t';
	buff[1]='h';
	buff[2]='r';
	buff[3]= '0' + thr_num;
	buff[4]='\0';
	while (1) {
		osSignalWait(0x01,osWaitForever);
		osSignalClear(tid_arr[thr_num],0x01);
		osDelay(1000);
	
		send_str(buff);
		osSignalSet(tid_arr[0],thr_num);
    osThreadYield ();                                         
  }                                         
  
}
void master_thread(void const *argument){
	int thr_num ;	
	int next_thr;
	char * buff ="thr";

	osEvent ev;
  
	thr_num = *((int* )argument);
	while (1) {
		ev= osSignalWait(0x00,osWaitForever);
		osSignalClear(tid_arr[thr_num],0x01);
		osDelay(1000);		
		send_str("master" );
		
		if (ev.value.signals == thread_count-1) {
			next_thr= 1;
		}
		else {
			next_thr= ev.value.signals+1;
		}
		osSignalSet(tid_arr[next_thr],0x01);
    osThreadYield ();                                           
  }
}


