/*
 * RTOS_prg.c
 *
 *  Created on: 14 Sep 2019
 *      Author: Yahia
 */

#include "std_types.h"
#include "common_macros.h"

#include "SysTick_interface.h"
#include "SysTick_config.h"
//#include "SysTick_private.h"

#include "RTOS_interface.h"
#include "RTOS_config.h"
#include "RTOS_private.h"

volatile uint32 OS_Tick = 0;                     // Counter for timer ticks.
TCB PeriodicQueue[MaxTaskNum]; 					 // Task Control Block(TCB) for tasks.
volatile uint8 Periodic_Task_index;              // The index(number) of the created tasks in the TCB.
volatile uint8 RunningTask;                      // The running task on the processor(if there is no running task then RunningTask will be the value of the lowest priority task +1).
volatile uint8 NonCompleted_Task_index = 0;      // The index(number) of the interrupted tasks(non-completed tasks).
uint8 NonCompletedTasks[MaxTaskNum];             //	Array of the interrupted tasks(non-completed tasks).
volatile uint8 EnterSchedulerState = NewTick;    // Variable that indicate which state that made processor enter the scheduler.
void(*RunTask)(void) = STUB;                     // Pointer to the task that must be running on the processor.
void(*RunPreemptiveTask)(void) = STUB;           // Pointer to the task that halted the processor and must run on it.
volatile uint8 IDLE_Flag = 1;                    // Variable that indicates that processor has no task to run and should enter idle state.
volatile uint8 TaskIsAlreadyRunning = 1;         // Variable that tells the processor that the task that must run is already running.
volatile uint8 SchedulerState;

/* OS_Handler */
/* Function that is called when the the timer fires an interrupt. */
void OS_Handler(void){
	OS_Tick++;                      	  /* Increase OS_Tick by 1 */
	if(SchedulerState == SchedulerOn){	  /* Check if the Scheduler if on */
		EnterSchedulerState = NewTick;    /* Define state of entering scheduler by NewTick state */
		Scheduler();                      /* Call scheduler */
	}
	else if(SchedulerState == SchedulerOff){ /* Check if the Scheduler if on */

	}
}

/* OSSchedulerOn */
/* Function that turn the scheduler on */
/* Scheduler is on by default */
void OSSchedulerOn(void){
	SchedulerState = SchedulerOn;
}

/* OSSchedulerOff */
/* Function that turn the scheduler off */
void OSSchedulerOff(void){
	SchedulerState = SchedulerOff;
}

/* Start_OS */
/* Function that starts running OS. */
void Start_OS(void){
	CheckPendingTasks();                                           /* Call CheckPendingTasks to check if there a pending
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	  task and not defined what that is pending on. */
	Set_CallBack(OS_Handler);                                      /* Set timer handler to call OS_Handler */
	SysTick_Init(OS_TickLength);                                   /* Initialise timer to generate an interrupt every OS_TickLength */
	SchedulerState = SchedulerOn;								   /* Turn the scheduler on */
	while(1){
		Scheduler();                                               /* Call scheduler */
		if(ThereWasARunningTask){								   /* Check if there is ready task to run */
			RunTask();											   /* Start executing the next task */
			PeriodicQueue[RunningTask].state = OSTaskStateReady;   /* The task is finished so we put it in the ready state */
			NoRunningTask;										   /* Tell the processor that there is no task to run */
			EnterSchedulerState = TaskIsOver;					   /* Define state of entering scheduler by TaskIsOver state */
		}
		else{
			IDLE_Flag = 1;                                         /* If there is no task to run then the processor must enter an idle state */
			RunTask = STUB;										   /* Make the pointer points to an empty function */
		}
		while(IDLE_Flag);										   /* Enter an idle state */
	}
}

/* RTOS_Init */
/* Function that initiates OS. */
void RTOS_Init(void){
	Periodic_Task_index = 0;			/* Initiate the index of TCB by 0 */
	InitiatePeriodicQueue();			/* Initiate TCB */
	InitiateMutex();					/* Initiate mutex */
}

/* ChangePriority */
/* Function for user to change the priority of task using its name. */
void ChangePriority(void(*Task_name)(void), uint8 Prior){
	TCB TempStruct;						/* Create local TCB structure */
	TCB* Periodic_Task = &TempStruct;      /* Create a pointer to the local TCB structure */
	uint8 count = 0;									    /* Create and initiate counter */
	if(CheckPriority(Prior)){								/* Check if the task exists in the TCB */
		return;												/* Exit */
	}
	while(count < Periodic_Task_index){					    /* Loop on the task in the TCB */
		if(PeriodicQueue[count].task_name == Task_name){    /* Check if the task's name exists in the TCB */
			TempStruct = PeriodicQueue[count];  			/* Assign the task that should be deleted to local TCB structure */
			TempStruct.priority = Prior;					/* Change priority */
			DeleteFromPeriodicQueue(count);					/* Delete the original task from TCB */
			AddTaskToPeriodicQueue(Periodic_Task);			/* Add the task with new priority to TCB */
			return;							    	    	/* Exit */
		}
		count++;                               		        /* Increase count by 1 */
	}
	//Error message: task not found
}

/* BlockTask */
/* Function for user to block task using its name. */
void BlockTask(void(*Task_name)(void), uint8 BlockerID){
	uint8 count = 0;									     /* Create and initiate counter */
	while(count < Periodic_Task_index){ 					 /* Loop on the task in the TCB */
		if(PeriodicQueue[count].task_name == Task_name){     /* Check if the task's name exists in the TCB */
			PeriodicQueue[count].state = OSTaskStateBlocked; /* Change the task's state to Blocked state */
			PeriodicQueue[count].Blocker = BlockerID;		 /* Change the task's Blocker ID to the new one */
			return;							    	    	 /* Exit */
		}
		count++;                               		         /* Increase count by 1 */
	}
	//Error message: task not found
}

/* UnblockTask */
/* Function for user to unblock task using its name. */
void UnblockTask(void(*Task_name)(void)){
	uint8 count = 0;									      /* Create and initiate counter */
	while(count < Periodic_Task_index){  					  /* Loop on the task in the TCB */
		if(PeriodicQueue[count].task_name == Task_name){      /* Check if the task's name exists in the TCB */
			PeriodicQueue[count].state = OSTaskStateReady;    /* Change the task's state to Ready state */
			PeriodicQueue[count].Blocker = BLOCKER_NoBlocker; /* Change the task's Blocker ID to No Blocker */
			return;							    	    	  /* Exit */
		}
		count++;                               		          /* Increase count by 1 */
	}
	//Error message: task not found
}

/* OSTaskCreatePending */
/* Function for user to tell what the function is pending on. */
void OSTaskCreatePending(void(*Task_name)(void), uint8* PTRtoService){
	uint8 count = 0;									      				/* Create and initiate counter */
	while(count < Periodic_Task_index){			 			  				/* Loop on the task in the TCB */
		if(PeriodicQueue[count].task_name == Task_name){      				/* Check if the task's name exists in the TCB */
			PeriodicQueue[count].state = OSTaskStatePending;  				/* Change the task's state to Pending state */
			if(*PTRtoService == MutServiceID){				 				/* Check if the task is pending on mutex */
				if(((MutexStruct*)PTRtoService) -> MutStatus == MutTaken){	/* Check if the mutex is taken */
					PeriodicQueue[count].PendOn = OSTaskPendOnMutex |\
							((((MutexStruct*)PTRtoService) -> MutID) << 4);		/* Change the pend on value in the TCB to mutex */
				}
				else{
					//Error message: the mutex you want to pend on is already ready.
					PeriodicQueue[count].state = OSTaskStateReady;			/* Change the task's state to Ready state */
					PeriodicQueue[count].PendOn = OSTaskPendOnNothing;		/* The task is pending on nothing */
					return;							    	    	        /* Exit */
				}
			}
			else if(*PTRtoService == EventServiceID){						/* Check if the task is pending on event */

			}
			else{
				//Error message: unknown Service ID.
			}
			return;				    	    	        					/* Exit */
		}
		count++;                               		          				/* Increase count by 1 */
	}
	//Error message: task not found
}

/* CreatePeriodicTask */
/* Function for user create a periodic task. */
void CreatePeriodicTask(void(*Task_name)(void),\
		uint32 Task_period,\
		uint8 Task_priority,\
		uint8 Task_state,\
		sint32 Task_release_time){

	TCB TempStruct;						  /* Create local TCB structure */
	TCB* Periodic_Task = &TempStruct;		  /* Create a pointer to the local TCB structure */

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 * 	Assigning the value of the task to the local TCB   *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Periodic_Task -> period = Task_period;
	Periodic_Task -> priority = Task_priority;
	Periodic_Task -> state = Task_state;
	Periodic_Task -> task_name = Task_name;
	Periodic_Task -> release_time = Task_release_time;
	Periodic_Task -> Blocker = BLOCKER_NoBlocker;
	Periodic_Task -> PendOn = OSTaskPendOnNothing;

	if(CheckPriority(Task_priority)){							/* Check if the task exists in the TCB */
		//Error message: The priority you choose for task is not valid.
		return;		    	    	        					/* Exit */
	}
	else if((Periodic_Task -> state) == OSTaskStatePending){    /* Check if the task in the Pending state */
		Periodic_Task -> PendOn = OSTaskPendOnWaiting;			/* Change the pend on value in the task's TCB to waiting state */
		AddTaskToPeriodicQueue(Periodic_Task);					/* Add task to TCB blocks */
	}
	else if((Periodic_Task -> state) == OSTaskStateReady){      /* Check if the task in the Ready state */
		AddTaskToPeriodicQueue(Periodic_Task);					/* Add task to TCB blocks */
	}
	else if((Periodic_Task -> state) == OSTaskStateBlocked){    /* Check if the task in the Blocked state */
		Periodic_Task -> Blocker = BLOCKER_USER;				/* Change the value of the Blocker in TCB to USER */
		AddTaskToPeriodicQueue(Periodic_Task);					/* Add task to TCB blocks */
	}
	else if((Periodic_Task -> state) == OSTaskStateRunning){    /* Check if the task in the Running state */
		//Error message: it is not allowed to enter task state as Running.
	}
	else{
		//Error message: Invalid value of task state.
	}
}

/* OS_DelayMS */
/* Function to make delay */
void OS_DelayMS(uint32 milliseconds){
	volatile uint32 Tick;										/* Create a variable of timer ticks */
	Tick = OS_Tick + (uint32)(milliseconds/OS_TickLength);		/* Tick = the value of the ticks when the function exit */
	while(OS_Tick < Tick);										/* Loop until the time is out */
}

/* AddTaskToPeriodicQueue */
/* Function to add a task's TCB to TCB block. */
static void AddTaskToPeriodicQueue(TCB* PtrToTaskStruct){
	uint8 count = 0;									      			/* Create and initiate counter */
	TCB temp_struct;								    /* Create local TCB structure */
	PeriodicQueue[Periodic_Task_index] = *PtrToTaskStruct;				/* Move the received TCB to TCB block */
	while(count != Periodic_Task_index){								/* Loop on the TCB array */
		if((PeriodicQueue[Periodic_Task_index].priority) <\
				(PeriodicQueue[count].priority)){						/* Check if there is any higher priority task */
			/* * * * * * * * * * * * *
			 * 	Swap between tasks   *
			 * * * * * * * * * * * * */
			temp_struct = PeriodicQueue[count];
			PeriodicQueue[count] = PeriodicQueue[Periodic_Task_index];
			PeriodicQueue[Periodic_Task_index] = temp_struct;
		}
		count++;               		          							/* Increase count by 1 */
	}
	Periodic_Task_index++;												/* Increment index of TCB block */
}

/* SaveContext */
/* Function to save the context of the interrupted task. */
static void SaveContext(void){
	volatile uint8 NonCompletedTask = RunningTask;					/* Create a variable of the running task */
	PeriodicQueue[NonCompletedTask].state = OSTaskStateReady;		/* Change the state of the running task to ready state */
	NonCompletedTasks[NonCompleted_Task_index] = NonCompletedTask;	/* Put the index of the task in an array that holds the non completed tasks */
	NonCompleted_Task_index++;										/* Increment index of the array on non completed tasks */
}

/* RestoreContext */
/* Function to restore the context of the interrupted task. */
static void RestoreContext(void){
	uint8 index = NonCompletedTasks[NonCompleted_Task_index - 1];  /* Create a variable that holds the value of the running task */
	PeriodicQueue[index].state = OSTaskStateReady;		           /* Change the state of the running task to ready state */
	NonCompleted_Task_index--;									   /* Decrement index of the array on non completed tasks */
	PeriodicQueue[index].state = OSTaskStateRunning;			   /* Change the state of the interrupted task to running state */
	RunTask = PeriodicQueue[index].task_name;					   /* Make the pointer points to the interrupted task */
	RunningTask = index;										   /* Change running task variable to the interrupted task */
}

/* UpdateReleaseTime */
/* Function to update the release time for tasks in TCB */
static void UpdateReleaseTime(void){
	uint8 count = 0;											/* Create and initiate counter */
	while(count != Periodic_Task_index){						/* Loop on the TCB array */
		PeriodicQueue[count].release_time -= OS_TickLength;		/* Update release time */
		count++;               		          					/* Increase count by 1 */
	}
}

/* StartScheduling */
/* Function to schedule on tasks */
static void StartScheduling(void){
	uint8 count = 0;									      						/* Create and initiate counter */
	while(count != Periodic_Task_index){											/* Loop on the TCB array */
		if((PeriodicQueue[count].release_time < 1) &&\
				((PeriodicQueue[count].state == OSTaskStateReady) ||\
						(PeriodicQueue[count].state == OSTaskStateRunning))){				/* Check if there is ready task to execute */
			if(PeriodicQueue[count].state == OSTaskStateReady){						/* Check if the task is ready or if it is already running */
				PeriodicQueue[count].state = OSTaskStateRunning;					/* Change the state of the state of task to running state */
				PeriodicQueue[count].release_time += PeriodicQueue[count].period;	/* reset release time */
				if(ThereWasARunningTask){											/* Check if there was a running task */
					SaveContext();													/* Save the context of the interrupted task */
					RunPreemptiveTask = PeriodicQueue[count].task_name;				/* Make the pointer points to the new task */
				}
				else{
					RunTask = PeriodicQueue[count].task_name;						/* Make the pointer points to the ready task */
				}
			}
			else if(PeriodicQueue[count].state == OSTaskStateRunning){				/* Check if the task was already running */
				if(TaskSwitching)													/* Check if there was a task switching */
					TaskIsAlreadyRunning = 1;										/* Tell the processor that the new task is already running */
			}
			RunningTask = PeriodicQueue[count].priority;							/* Change running task variable to the new task */
			return;																	/* Exit */
		}
		count++;               		          										/* Increase count by 1 */
	}
}

/* HandleTaskSwitching */
/* Function to handle task switching on tasks */
static void HandleTaskSwitching(void){
	RunPreemptiveTask();		/* Run the new task */
	RestoreContext();			/* Restore the context of the interrupted task */
}

/* Scheduler */
/* Function to schedule on task */
void Scheduler(void){
	IDLE_Flag = 0;											/* Reset IDLE_Flag */
	if(EnterSchedulerState == NewTick){						/* Check if the scheduler is entered by new timer tick */
		UpdateReleaseTime();								/* Update release time of Tasks*/
		StartScheduling();									/* Start scheduling */
		if(TaskSwitching && (TaskIsAlreadyRunning != 1)){	/* Check if there is task switching */
			HandleTaskSwitching();							/* Handle task switching and run the new task */
			EnterSchedulerState = TaskIsOver;				/* Update the state of entering the scheduler by TaskIsOver state */
			TaskIsAlreadyRunning = 0;						/* Reset the flag */
		}
	}
	else if(EnterSchedulerState == TaskIsOver){				/* Check if the scheduler is entered after finishing previous task */
		StartScheduling();									/* Start scheduling */
	}
	else if(EnterSchedulerState == MutexReleased){			/* Check if the scheduler is entered after releasing mutex */
		StartScheduling();									/* Start scheduling */
		if(TaskSwitching && (TaskIsAlreadyRunning != 1)){	/* Check if there is task switching */
			HandleTaskSwitching();							/* Handle task switching and run the new task */
			EnterSchedulerState = TaskIsOver;				/* Update the state of entering the scheduler by TaskIsOver state */
			TaskIsAlreadyRunning = 0;						/* Reset the flag */
		}
	}
}

/* CheckPendingTasks */
/* Function that checks if the task in the pending state. */
static void CheckPendingTasks(void){
	uint8 count = 0;                          /* Create and initiate counter */
	while(count < Periodic_Task_index){       /* Loop on the task in the TCB */
		if((PeriodicQueue[count].state == OSTaskStatePending) &&\
				(PeriodicQueue[count].PendOn == OSTaskPendOnWaiting)){      /* Check if the state of the task is pending and if it has nothing to pend on */
			//Error Message; you didn't define the value that the task is pending on.
			return;                           /* Exit from the function */
		}
		count++;                              /* Increase count by 1 */
	}
}


/* InitiatePeriodicQueue */
/* Function that initiates the TCB. */
static void InitiatePeriodicQueue(void){
	uint8 count = 0;                                      /* Create and initiate counter */
	while(count < MaxTaskNum){                            /* Loop on all possible places for task in TCB */
		PeriodicQueue[count].state = OSTaskStateNoTask;   /* Put all tasks in NoTask state */
		count++;                                          /* Increase count by 1 */
	}
}

/* CheckPriority */
/* Function to check the priority of tasks. */
static uint8 CheckPriority(uint8 Prior){
	uint8 count = 0;									 /* Create and initiate counter */
	while(count < Periodic_Task_index){					 /* Loop on the task in the TCB */
		if(Prior == PeriodicQueue[count].priority)		 /* Check if the priority of the task exists in the TCB */
			return 1;									 /* Return 1 that indicate that that function with this specified priority exists */
		count++;                                         /* Increase count by 1 */
	}
	return 0;											 /* Return 0 that indicate that that function with this specified priority doesn't exist */
}

/* DeleteFromPeriodicQueue */
/* Function to delete a task from TCB. */
static void DeleteFromPeriodicQueue(uint8 Index){
	uint8 count = Index;									/* Create and initiate counter by the index of function that should be deleted */
	while(count < Periodic_Task_index){						/* Loop on the above tasks in the TCB */
		PeriodicQueue[count] = PeriodicQueue[count +1];		/* Replace each task by the function above it in the TCB */
		count++;                                            /* Increase count by 1 */
	}
	Periodic_Task_index--;									/* Decrement index of TCB by 1 */
}

/* DeleteTask */
/* Function for user to delete a task using its name. */
void DeleteTask(void(*Task_name)(void)){
	uint8 count = 0;												 /* Create and initiate counter */
	while(count < Periodic_Task_index){								 /* Loop on the task in the TCB */
		if(PeriodicQueue[count].task_name == Task_name){			 /* Check if the task's name exists in the TCB */
			DeleteFromPeriodicQueue(PeriodicQueue[count].priority);	 /* Delete the Task */
			return;													 /* Exit */
		}
		count++;                                        		     /* Increase count by 1 */
	}
	//Error message: task not found
}
