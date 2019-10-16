/*
 * Mutex_Private.h
 *
 *  Created on: 3 Oct 2019
 *      Author: Yahia
 */

#ifndef MUTEX_PRIVATE_H_
#define MUTEX_PRIVATE_H_

#define NULL 0

#ifndef MaxTaskNum
#define MaxTaskNum 20
#endif

//#define RunningTask (OS_Status.RunningTask)

#ifndef MutReady
#define MutReady 0
#endif
#ifndef MutTaken
#define MutTaken 1
#endif

#ifndef MutServiceID
#define MutServiceID 0
#endif

#ifndef MutexReleased
#define MutexReleased 2
#endif

void Scheduler(void);
void InitiateMutex(void);
static uint8 CheckMutID(uint8 Index);

#endif /* MUTEX_PRIVATE_H_ */
