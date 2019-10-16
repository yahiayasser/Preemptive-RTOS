/*
 * RTOS_interface.h
 *
 *  Created on: 14 Sep 2019
 *      Author: Yahia
 */

#ifndef RTOS_INTERFACE_H_
#define RTOS_INTERFACE_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* 				Task Control Block (TCB)				 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * */
typedef struct{
	void(*task_name)(void);		/* This field contains the address of the first line of the task */
	uint32 period;				/* This field contains the periodic time of the task
	 	 	 	 	 	 	 	   updating this value during run time may cause undefined behaviour
	 	 	 	 	 	 	 	   */
	sint32 release_time;		/* This field contains the release time of the task
	 	 	 	 	 	 	 	   This value is updated automatically by the OS based on each task's priority*/
	uint8 PendOn;				/* This field indicates what the task is pending on and contains one of these values
	 	 	 	 	 	 	 	   OSTaskPendOnNothing
	 	 	 	 	 	 	 	   OSTaskPendOnMutex
	 	 	 	 	 	 	 	   OSTaskPendOnEvent
	 	 	 	 	 	 	 	   This value must be OSTaskPendOnNothing if the task is not in the pending state
	 	 	 	 	 	 	 	   */
	uint8 priority;				/* This field contains the priority of the task
	 	 	 	 	 	 	 	   Priority must be unique for each task
	 	 	 	 	 	 	 	   Priority must not exceed the value of MaxTaskNum-1 in RTOS_config.h
	 	 	 	 	 	 	 	   */
	uint8 state;				/* This field indicates the current state of a task and contains one of these values
	 	 	 	 	 	 	 	   OSTaskStateReady
	 	 	 	 	 	 	 	   OSTaskStateBlocked
	 	 	 	 	 	 	 	   OSTaskStateRunning
	 	 	 	 	 	 	 	   OSTaskStatePending
	 	 	 	 	 	 	 	   */
	uint8 Blocker;				/* This field indicates the ID of the Blocker
	 	 	 	 	 	 	 	   If the task is not in the blocked state this value must be BLOCKER_NoBlocker
	 	 	 	 	 	 	 	   This field contains one of these values
	 	 	 	 	 	 	 	   BLOCKER_USER
	 	 	 	 	 	 	 	   BLOCKER_MUTEX
	 	 	 	 	 	 	 	   BLOCKER_NoBlocker
	 	 	 	 	 	 	 	   */
}TCB;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* 			 		Mutex Struscture 					 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * */
typedef struct{
	uint8 MutID;				/* This field contains mutex id
	 	 	 	 	 	 	 	   MutID must be unique for each mutex
	 	 	 	 	 	 	 	   MutID must not exceed the value of NumOfMutex in RTOS_config.h
	 	 	 	 	 	 	 	   */
	uint8 MutStatus;			/* This field indicates the current state of a mutex and contains one of these values
	 	 	 	 	 	 	 	   MutReady
	 	 	 	 	 	 	 	   MutTaken
	 	 	 	 	 	 	 	   */
	uint8 ServiceID;			/* This field contains the Service Id of the mutex service
	 	 	 	 	 	 	 	   This field is updated by the OS
	 	 	 	 	 	 	 	   */
	void(*MutTask)(void);		/* This field contains the address of the task that takes the task*/
}MutexStruct;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* 			 	    	Task states 					 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef OSTaskStateReady
#define OSTaskStateReady 0
#endif
#ifndef OSTaskStateBlocked
#define OSTaskStateBlocked 1
#endif
#ifndef OSTaskStateRunning
#define OSTaskStateRunning 2
#endif
#ifndef OSTaskStatePending
#define OSTaskStatePending 3
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* 			 	    	Blocker states 					 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef BLOCKER_USER
#define BLOCKER_USER 0
#endif
#ifndef BLOCKER_MUTEX
#define BLOCKER_MUTEX 1
#endif
#ifndef BLOCKER_NoBlocker
#define BLOCKER_NoBlocker 0xFF
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* 			 	    	Pend on states 					 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef OSTaskPendOnNothing
#define OSTaskPendOnNothing 0
#endif
#ifndef OSTaskPendOnMutex
#define OSTaskPendOnMutex 1
#endif
#ifndef OSTaskPendOnEvent
#define OSTaskPendOnEvent 2
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* 			 	    	Priority values					 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define PRIORITY_0  0
#define PRIORITY_1  1
#define PRIORITY_2  2
#define PRIORITY_3  3
#define PRIORITY_4  4
#define PRIORITY_5  5
#define PRIORITY_6  6
#define PRIORITY_7  7
#define PRIORITY_8  8
#define PRIORITY_9  9
#define PRIORITY_10 10
#define PRIORITY_11 11
#define PRIORITY_12 12
#define PRIORITY_13 13
#define PRIORITY_14 14
#define PRIORITY_15 15
#define PRIORITY_16 16
#define PRIORITY_17 17
#define PRIORITY_18 18
#define PRIORITY_19 19
#define PRIORITY_20 20
#define PRIORITY_21 21
#define PRIORITY_22 22
#define PRIORITY_23 23
#define PRIORITY_24 24
#define PRIORITY_25 25
#define PRIORITY_26 26
#define PRIORITY_27 27
#define PRIORITY_28 28
#define PRIORITY_29 29
#define PRIORITY_30 30
#define PRIORITY_31 31
#define PRIORITY_32 32
#define PRIORITY_33 33
#define PRIORITY_34 34
#define PRIORITY_35 35

/*	RTOS_Init
 * Function Description: Initialise OS
 * Parameters(In): None.
 * Parameters(Out): None.
 * Return Value: None.
 * Sync/Async: Synchronous.
 * Reentrancy: Reentrant.
 * */
void RTOS_Init(void);

/*	Start_OS
 * Function Description: starting OS
 * Parameters(In): None.
 * Parameters(Out): None.
 * Return Value: None.
 * */
void Start_OS(void);

/*	CreatePeriodicTask
 * Function Description: create periodic task
 * Parameters(In): Task_name 		 --> Pointer to task
   	   	   	   	   Task_period 		 --> Task period in ms
   	   	   	   	   Task_priority 	 --> Task_priority: integer from 0 to MaxTaskNum-1
   	   	   	   	   	   	   	   	   	   	   	   	   	you can use one value from Priority values in RTOS_interface.h file.
   	   	   	   	   Task_state 		 --> contains one of these values
										 OSTaskStateReady
										 OSTaskStateBlocked
										 OSTaskStatePending
										 using OSTaskStateRunning in this field will generate an error.
	 	 	       Task_release_time --> Task release time in ms.
 * Parameters(Out): None.
 * Return Value: None.
 * */
void CreatePeriodicTask(void(*Task_name)(void), uint32 Task_period, uint8 Task_priority, uint8 Task_state, sint32 Task_release_time);

/*	OS_DelayMS
 * Function Description: this function is used to make delay
 * Parameters(In): milliseconds --> number of milliseconds.
 * Parameters(Out): None.
 * Return Value: None.
 * */
void OS_DelayMS(uint32 milliseconds);

/*	DeleteTask
 * Function Description: this function is used to delete a task
 * Parameters(In): Task_name --> Pointer to task.
 * Parameters(Out): None.
 * Return Value: None.
 * */
void DeleteTask(void(*Task_name)(void));

/*	ChangePriority
 * Function Description: this function is used to delete a task
 * Parameters(In): Task_name --> Pointer to task.
 	 	 	 	   Prior	 --> the new priority of the task
 * Parameters(Out): None.
 * Return Value: None.
 * */
void ChangePriority(void(*Task_name)(void), uint8 Prior);

/*	BlockTask
 * Function Description: this function is used to block a task
 * Parameters(In): Task_name --> Pointer to task.
 	 	 	 	   BlockerID --> the blocker of the task
 	 	 	 	   	   	   	   	 while using this task by user this value must be USER
 * Parameters(Out): None.
 * Return Value: None.
 * */
void BlockTask(void(*Task_name)(void), uint8 BlockerID);

/*	UnblockTask
 * Function Description: this function is used to unblock a task to the ready state
 * Parameters(In): Task_name --> Pointer to task.
 * Parameters(Out): None.
 * Return Value: None.
 * */
void UnblockTask(void(*Task_name)(void));

/*	OSTaskCreatePending
 * Function Description: this function is used to put the task in the pending state
 	 	 	 	 	 	 if you created a task as pending you must use this function before starting OS.
 * Parameters(In): Task_name 	--> Pointer to task.
 	 	 	 	   PTRtoService --> Pointer to the service you want to pend on
 	 	 	 	   	   	   	   	    you can use these values
 	 	 	 	   	   	   	   	    OSTaskPendOnMutex
 	 	 	 	   	   	   	   	    OSTaskPendOnMutexEvent
 * Parameters(Out): None.
 * Return Value: None.
 * */
void OSTaskCreatePending(void(*Task_name)(void), uint8* PTRtoService);

/*	MutexCreate
 * Function Description: this function is used to create a mutex
 * Parameters(In): MutexIndex 			   --> unique value of mutex from 0 to NumOfMutex-1.
				   Pointer to MutexStruct  --> Pointer to the created mutex.
 * Parameters(Out): None.
 * Return Value: Pointer to MutexStruct  --> Pointer to the created mutex..
 * */
MutexStruct* MutexCreate(uint8 MutexIndex);

/*	MutexTake
 * Function Description: this function is used to acquire a mutex
 	 	 	 	 	 	 if the mutex is already, the task will be blocked until the task that took it release the mutex
 * Parameters(In): PtrToMut --> Pointer to mutex that you want to take.
 * Parameters(Out): None.
 * Return Value: None.
 * */
void MutexTake(MutexStruct* PtrToMut);

/*	MutexRelease
 * Function Description: this function is used to release the mutex
 	 	 	 	 	 	 the task that took the mutex is the only task that can release it
 * Parameters(In): PtrToMut --> Pointer to mutex that you want to take.
 * Parameters(Out): None.
 * Return Value: None.
 * */
void MutexRelease(MutexStruct* PtrToMut);

/*	OSSchedulerOn
 * Function Description: this function is used to turn the scheduler on
 	 	 	 	 	 	 the scheduler in on by default unless you turn it off
 * Parameters(In): None.
 * Parameters(Out): None.
 * Return Value: None.
 * */
void OSSchedulerOn(void);

/*	OSSchedulerOff
 * Function Description: this function is used to turn the scheduler off
 * Parameters(In): None.
 * Parameters(Out): None.
 * Return Value: None.
 * */
void OSSchedulerOff(void);

#endif /* RTOS_INTERFACE_H_ */
