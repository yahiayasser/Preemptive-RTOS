/*
 * RTOS_private.h
 *
 *  Created on: 14 Sep 2019
 *      Author: Yahia
 */

#ifndef RTOS_PRIVATE_H_
#define RTOS_PRIVATE_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* 			 		  Macro expressions					 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define NULL 0

#define LowestPriority (MaxTaskNum - 1)

#define ThereWasARunningTask (RunningTask != (LowestPriority+1))

#define TaskSwitching (NonCompleted_Task_index != 0)

#define NoTaskIsReady (RunningFlag == 0)

#define NoRunningTask (RunningTask = (LowestPriority+1))

#ifndef OSTaskStateNoTask
#define OSTaskStateNoTask 4
#endif

#ifndef OSTaskPendOnWaiting
#define OSTaskPendOnWaiting 3
#endif

#ifndef MutServiceID
#define MutServiceID 'M'
#endif

#ifndef EventServiceID
#define EventServiceID 'E'
#endif

#ifndef MutReady
#define MutReady 0
#endif
#ifndef MutTaken
#define MutTaken 1
#endif


#ifndef NewTick
#define NewTick 0
#endif
#ifndef TaskIsOver
#define TaskIsOver 1
#endif
#ifndef MutexReleased
#define MutexReleased 2
#endif

#define SchedulerOn   1
#define SchedulerOff  0

/*	OS_Handler
 * Function Description: Handle new timer tick
 	 	 	 	 	 	 This function is called by the timer handler
 * Parameters(In): None.
 * Parameters(Out): None.
 * Return Value: None.
 * */
void OS_Handler(void);

/*	CheckPendingTasks
 * Function Description: Check the pend on values in the TCBs of pend tasks
 	 	 	 	 	 	 This function is called at the beginning of starting OS function
 * Parameters(In): None.
 * Parameters(Out): None.
 * Return Value: None.
 * */
static void CheckPendingTasks(void);

/*	CheckPriority
 * Function Description: Check priority of a specific task
 	 	 	 	 	 	 This function is called by OS
 * Parameters(In): Prior --> Priority of the task.
 * Parameters(Out): None.
 * Return Value: 1 --> if the priority exists
 	 	 	 	 0 --> if the priority doesn't exist
 * */
static uint8 CheckPriority(uint8 Prior);

/*	DeleteFromPeriodicQueue
 * Function Description: Delete task from TCB array
 	 	 	 	 	 	 This function is called by the OS
 * Parameters(In): Index --> Index of the task in the TCB array.
 * Parameters(Out): None.
 * Return Value: None.
 * */
static void DeleteFromPeriodicQueue(uint8 Index);

/*	StartScheduling
 * Function Description: Scheduling on the tasks
 	 	 	 	 	 	 This function is called by OS
 * Parameters(In): None.
 * Parameters(Out): None.
 * Return Value: None.
 * */
static void StartScheduling(void);

/*	UpdateReleaseTime
 * Function Description: Update the release time of the tasks
 	 	 	 	 	 	 This function is called by OS in the beginning of timer handler
 * Parameters(In): None.
 * Parameters(Out): None.
 * Return Value: None.
 * */
static void UpdateReleaseTime(void);

/*	AddTaskToPeriodicQueue
 * Function Description: Adding Task to TCB array
 	 	 	 	 	 	 This function is called by OS
 * Parameters(In): PtrToTaskStruct --> Pointer to task's TCB.
 * Parameters(Out): None.
 * Return Value: None.
 * */
static void AddTaskToPeriodicQueue(TCB* PtrToTaskStruct);

/*	SaveContext
 * Function Description: Saving context of the interrupted task
 	 	 	 	 	 	 This function is called by OS in the case of context switching
 * Parameters(In): None.
 * Parameters(Out): None.
 * Return Value: None.
 * */
static void SaveContext(void);

/*	RestoreContext
 * Function Description: Restoring context of the interrupted task
 	 	 	 	 	 	 This function is called by OS after handling context switching context switching
 * Parameters(In): None.
 * Parameters(Out): None.
 * Return Value: None.
 * */
static void RestoreContext(void);

/*	Scheduler
 * Function Description: Scheduler function
 	 	 	 	 	 	 This function is called by OS
 * Parameters(In): None.
 * Parameters(Out): None.
 * Return Value: None.
 * */
void Scheduler(void);

/*	InitiatePeriodicQueue
 * Function Description: Initialise TCB array
 	 	 	 	 	 	 This function is called by RTOS_Init
 * Parameters(In): None.
 * Parameters(Out): None.
 * Return Value: None.
 * */
static void InitiatePeriodicQueue(void);

/*	InitiatePeriodInitiateMutexicQueue
 * Function Description: Initialise mutex array
 	 	 	 	 	 	 This function is called by RTOS_Init
 * Parameters(In): None.
 * Parameters(Out): None.
 * Return Value: None.
 * */
void InitiateMutex(void);

#endif /* RTOS_PRIVATE_H_ */
