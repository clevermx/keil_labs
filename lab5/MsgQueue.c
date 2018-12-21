#include "cmsis_os.h"
#include "stm32f2xx.h"                  // Device header
#include <stdlib.h>
extern void LED_On (int ODRnum); 
extern void LED_Off (int ODRnum);
extern void delay();

void Thread_MsgQueueSet (void const *argument);                   // thread function 1
void Thread_MsgQueueGet (void const *argument);                   // thread function 2
extern osThreadId* tid_arr;  											// thread id
extern  int thread_count;
osThreadId tid_Thread_MsgQueueGet;                                // thread id 2
osThreadDef (Thread_MsgQueueSet, osPriorityNormal, 1, 0);         // thread object 1
osThreadDef (Thread_MsgQueueGet, osPriorityNormal, 1, 0);         // thread object 2

#define MSGQUEUE_OBJECTS      16                                // number of Message Queue Objects
osMessageQId mid_MsgQueue;                                      // message queue id
osMessageQDef (MsgQueue, MSGQUEUE_OBJECTS, MSGQUEUE_OBJ_t);     // message queue object


union{
  uint32_t v;
  void *p;
  int32_t signals;
} value;

osMessageQId Q_LED;
osMessageQDef (Q_LED, 0x16, unsigned char);
osEvent result;


int Init_MsgQueue (void) {
	int i =0;
	int* arg;
 
  
  mid_MsgQueue = osMessageCreate (osMessageQ(MsgQueue), NULL);  // create msg queue
  if (!mid_MsgQueue) {
    ; // Message Queue object not created, handle failure
  }
 
	tid_arr = (osThreadId*) malloc((thread_count +1) * sizeof(osThreadId));   
	arg=(int*) malloc(sizeof(int));
	arg[0]=0;  
  for( i= 0; i<thread_count;i++){   
	arg=(int*) malloc(sizeof(int));
	arg[0]=i;		
	tid_arr[i] = osThreadCreate (osThread(Thread_MsgQueueSet), arg);
	}	
	
  tid_Thread_MsgQueueGet = osThreadCreate (osThread(Thread_MsgQueueGet), NULL);
  if (!tid_Thread_MsgQueueGet) return(-1);
  
  return(0);
}
void Thread_MsgQueueSet (void const *argument) {
	int thr_num=*((int* )argument);	
 	int led_n =thr_num;
	int led_cur=6+thr_num;
  while (1) {
     LED_Off(led_cur);                                     // Mem Block was available		
if (led_cur==6) {
	osMessagePut (mid_MsgQueue, 6, osWaitForever); // Send Message		
}	else {
 if (led_cur==7) {
	osMessagePut (mid_MsgQueue, 7, osWaitForever); // Send Message		
}	else{
	osMessagePut (mid_MsgQueue, 8, osWaitForever); // Send Message		
}	 
    osThreadYield ();                                           // suspend thread
  }
}
}

void Thread_MsgQueueGet (void const *argument) {
  while (1) {
		result = osMessageGet(mid_MsgQueue, osWaitForever);				//wait for a message to arrive
		LED_On(result.value.v);    
  }
}


