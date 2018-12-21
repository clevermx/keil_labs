#include <stdlib.h>
#include "cmsis_os.h"                                           // CMSIS RTOS header file

/*----------------------------------------------------------------------------
 *      Mutex creation & usage
 *---------------------------------------------------------------------------*/
extern void Print_LCD(char* message);
void Thread_Mutex (void const *argument);  // thread function
void mutex_master_thr (void const *argument);
extern osThreadId* tid_arr;  											// thread id
extern  int thread_count;
osThreadDef (Thread_Mutex, osPriorityNormal, 1, 0);             // thread object
osThreadDef (mutex_master_thr, osPriorityNormal, 1, 0); 



osMutexId* mid_Thread_Mutex;                                     // mutex id
osMutexDef (SampleMutex);                                      // mutex name definition

int* flag;

int Init_Mutex (void) {
	int i =0;
	int* arg;
	mid_Thread_Mutex = (osMutexId*) malloc((thread_count +1) * sizeof(osMutexId));   
	tid_arr = (osThreadId*) malloc((thread_count +1) * sizeof(osThreadId));   
	arg=(int*) malloc(sizeof(int));
	arg[0]=0;
	mid_Thread_Mutex[0]=  osMutexCreate (osMutex (SampleMutex));
	if (mid_Thread_Mutex[0]== NULL ){
		osDelay(1000);
	}
	tid_arr[0] = osThreadCreate (osThread(mutex_master_thr),arg);
	if (!tid_arr[i]) return(-1);
	for( i= 1; i<thread_count;i++){ 	 
	mid_Thread_Mutex[i]=  osMutexCreate (osMutex(SampleMutex));	
	
		
		
	}	
	
  return(0);
}

void Thread_Mutex (void const *argument) {
  osStatus status;
	int thr_num=*((int* )argument);	
	char* buff;
 buff= (char*) malloc(sizeof(char) * 5);
	buff[0]='t';
	buff[1]='h';
	buff[2]='r';
	buff[3]= '0' + thr_num;
	buff[4]='\0';
  while (1) {
    ; // Insert thread code here...
	while(!(flag[0] == 0)){		
		}
    status = osMutexWait (mid_Thread_Mutex[0], NULL);
    switch (status) {
      case osOK:
				osDelay(1000);	
				Print_LCD(buff);
				flag[0]=thr_num;
        osMutexRelease (mid_Thread_Mutex[0]);
        break;
      case osErrorTimeoutResource:
        break;
      case osErrorResource:
        break;
      case osErrorParameter:
        break;
      case osErrorISR:
        break;
      default:
        break;
    }
    osThreadYield ();                                           // suspend thread
  }
}
void mutex_master_thr (void const *argument) {
  osStatus status;
	int* arg;
	int i = 0;
	flag = (int*) malloc(sizeof(int));
	
	status = osMutexWait (mid_Thread_Mutex[0], NULL);
	
	for( i= 1; i<thread_count;i++){   
	arg=(int*) malloc(sizeof(int));
	arg[0]=i;		
	tid_arr[i] = osThreadCreate (osThread(Thread_Mutex), arg);

	}	
  while (1) {
    ; // Insert thread code here...
	for(i=1; i<thread_count;i++){
		osDelay(1000);	
		Print_LCD("master");
		flag[0]=0;
		osMutexRelease (mid_Thread_Mutex[0]);	
	while(flag[0]==0){		
	}
    status = osMutexWait (mid_Thread_Mutex[0], NULL);
		flag[0]=0;
	}   
  }
}
