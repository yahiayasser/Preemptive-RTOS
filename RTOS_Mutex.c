/*
 * RTOS_Mutex.c
 *
 *  Created on: 2 Oct 2019
 *      Author: Yahia
 */

#include "std_types.h"
#include "common_macros.h"

#include "SysTick_interface.h"
#include "SysTick_config.h"

#include "Mutex_private.h"

#include "RTOS_interface.h"
#include "RTOS_config.h"

MutexStruct MutexQueue[NumOfMutex];			  /* Array of mutexes */
uint8 MutexQueueCounter = 0;				  /* Index of MutexQueue */
extern TCB PeriodicQueue[MaxTaskNum];
extern volatile uint8 RunningTask;
extern volatile uint8 Periodic_Task_index;
extern volatile uint8 EnterSchedulerState;    /* Variable that indicate which state that made processor enter the scheduler. */

/* MutexCreate */
/* Mutex create */
MutexStruct* MutexCreate(uint8 MutexIndex){
	if(CheckMutID(MutexIndex)){								 /* Check if the index of the mutex id is valid */
		//Error message: The priority you choose for task is not valid.
		return NULL;
	}
	MutexQueue[MutexQueueCounter].MutID = MutexIndex;		/* Update ID of Mutex */
	MutexQueue[MutexQueueCounter].MutStatus = MutReady;		/* Update state of Mutex */
	MutexQueueCounter++;
	return &MutexQueue[MutexQueueCounter - 1];				/* Address of the created mutex */
}

/* MutexTake */
/* Acquire mutex */
void MutexTake(MutexStruct* PtrToMut){
	if(((PtrToMut -> MutStatus) == MutTaken) &&\
			((PeriodicQueue[RunningTask].task_name) != (PtrToMut ->MutTask))){  							/* Check if the mutex is taken by another task */
		BlockTask(PeriodicQueue[RunningTask].task_name, (BLOCKER_MUTEX | ((PtrToMut -> MutID) << 4)));		/* Block caller task */
	}
	else if((PtrToMut -> MutStatus) == MutReady){															/* If the mutex is free */
		PtrToMut -> MutTask = PeriodicQueue[RunningTask].task_name;											/* Update the task name in the mutex struct to the caller task's name */
		PtrToMut -> MutStatus = MutTaken;																	/* Update state of the mutex to taken */
	}
}

/* MutexRelease */
/* Release mutex */
void MutexRelease(MutexStruct* PtrToMut){
	uint8 count = 0;
	if((PtrToMut -> MutStatus) != MutTaken){										/* Check if the mutex is not taken */
		//Error message: The mutex you want to release is not taken.
		return;
	}
	else if((PeriodicQueue[RunningTask].task_name) != (PtrToMut -> MutTask)){		/* Check if the mutex is taken by another task so the caller task can't free it */
		//Error message: the function that releases mutex must be the one who took it.
		return;
	}
	while(count < Periodic_Task_index){												/* Loop on the task in the TCB block */
		if((PeriodicQueue[count].state == OSTaskStateBlocked) &&\
						((PeriodicQueue[count].Blocker) ==\
						(BLOCKER_MUTEX | ((PtrToMut -> MutID) << 4))))				/* Check if there is any task is blocked by mutex */
			UnblockTask(PeriodicQueue[count].task_name);							/* Unblock */
		else if((PeriodicQueue[count].state == OSTaskStatePending) &&\
						(PeriodicQueue[count].PendOn ==\
						(OSTaskPendOnMutex | ((PtrToMut -> MutID) << 4)))){			/* Check if there is any task is pending on the mutex */
			PeriodicQueue[count].state = OSTaskStateReady;							/* Make it ready */
			PeriodicQueue[count].PendOn = OSTaskPendOnNothing;						/* Update the pend on value in its TCB */
		}
		count++;
	}
	PtrToMut -> MutTask = STUB;					/* Update the name of the task that takes the mutex */
	PtrToMut -> MutStatus = MutReady;			/* Update the state of the mutex to ready */
	EnterSchedulerState = MutexReleased;		/* Update the state of entering the scheduler by MutexReleased state */
	Scheduler();								/* Call Scheduler */
}

/* InitiateMutex */
/* Intiate mutex array */
void InitiateMutex(void){
	uint8 count = 0;
	while(count < NumOfMutex){
		MutexQueue[count].ServiceID = MutServiceID;
		MutexQueue[count].MutStatus = MutReady;
		MutexQueue[count].MutID = NumOfMutex;
	}
}

/* CheckMutID */
/* Check if the index of the exists in the mutex array */
static uint8 CheckMutID(uint8 Index){
	uint8 count = 0;
	while(count < MutexQueueCounter){
		if(Index == MutexQueue[count].MutID)
			return 1;
		count++;
	}
	return 0;
}
