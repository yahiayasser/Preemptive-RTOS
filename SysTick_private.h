/*
 * private.h
 *
 *  Created on: 22 Aug 2019
 *      Author: Yahia
 */

#ifndef PRIVATE_H_
#define PRIVATE_H_

#ifndef RCC_Clock_FreQ
#define RCC_Clock_FreQ 72000000
#endif

#define MaxValue 4294967295

typedef struct{
	uint32 CTRL;
	uint32 LOAD;
	uint32 VAL;
	uint32 CALIB;
}SysTickRegisters;

#define SysTick ((volatile SysTickRegisters*) 0xE000E010)

#define STK_CTRL (SysTick -> CTRL)
#define STK_LOAD (SysTick -> LOAD)
#define STK_VAL (SysTick -> VAL)
#define STK_CALIB (SysTick -> CALIB)


#endif /* PRIVATE_H_ */
