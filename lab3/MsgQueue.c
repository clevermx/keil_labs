#include <stdlib.h>
#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include "stm32f2xx.h"         
/*----------------------------------------------------------------------------
 *      Message Queue creation & usage
 *---------------------------------------------------------------------------*/
 
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

typedef struct {                                                // object data type
  uint8_t Buf[32];
  uint8_t Idx;
} MEM_BLOCK_t;

typedef struct {                                                // object data type
  uint8_t Buf[32];
  uint8_t Idx;
} MSGQUEUE_OBJ_t;

osPoolId mpid_MemPool2;                                         // memory pool id
osPoolDef (MemPool2, MSGQUEUE_OBJECTS, MEM_BLOCK_t);            // memory pool object
  
osMessageQId mid_MsgQueue;                                      // message queue id
osMessageQDef (MsgQueue, MSGQUEUE_OBJECTS, MSGQUEUE_OBJ_t);     // message queue object


int Init_MsgQueue (void) {
	int i =0;
	int* arg;
  mpid_MemPool2 = osPoolCreate (osPool (MemPool2));             // create Mem Pool
  if (!mpid_MemPool2) {
    ; // MemPool object not created, handle failure
  }
  
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
  MEM_BLOCK_t *pMsg = 0;
	int led_n =thr_num;
	int led_cur=6+thr_num;
  while (1) {
    ; // Insert thread code here...
    pMsg = (MEM_BLOCK_t *)osPoolCAlloc (mpid_MemPool2);         // get Mem Block
    if (pMsg) {                                                 // Mem Block was available		
      LED_Off(led_cur);
			pMsg->Buf[0] = led_cur;                                      // do some work...
      pMsg->Idx    = thr_num;
      osMessagePut (mid_MsgQueue, (uint32_t)pMsg, osWaitForever); // Send Message		
			
    }

    osThreadYield ();                                           // suspend thread
  }
}

void Thread_MsgQueueGet (void const *argument) {
  osEvent      evt;
  MEM_BLOCK_t *pMsg = 0;

  while (1) {
    ; // Insert thread code here...
    evt = osMessageGet (mid_MsgQueue, osWaitForever);           // wait for message
    if (evt.status == osEventMessage) {
      pMsg = evt.value.p;
      if (pMsg) {
				LED_On(pMsg->Buf[0]);  
        ; // process data
        osPoolFree (mpid_MemPool2, pMsg);                       // free memory allocated for message
      }
    }
  }
}
