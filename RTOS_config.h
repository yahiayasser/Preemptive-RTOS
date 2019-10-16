/*
 * RTOS_config.h
 *
 *  Created on: 14 Sep 2019
 *      Author: Yahia
 */

#ifndef RTOS_CONFIG_H_
#define RTOS_CONFIG_H_

/* OS Tick length in ms */
#define OS_TickLength 100

/* Number of mutex used in the program */
#define NumOfMutex 5

/* Number of tasks used in the program */
#ifndef MaxTaskNum
#define MaxTaskNum 20
#endif

#endif /* RTOS_CONFIG_H_ */
