/*
 * interface.h
 *
 *  Created on: 22 Aug 2019
 *      Author: Yahia
 */

#ifndef INTERFACE_H_
#define INTERFACE_H_

#define AHB_FreQ_8  0
#define AHB_FreQ    1

void SysTick_Init(uint32 OS_TickMS);
void Set_CallBack(void(*PTR)(void));
void STUB(void);
float32 CurrentTimeMS(void);
uint8 IsTimeOver(uint32 TimeMS);

#endif /* INTERFACE_H_ */
