/*
 * prg.c
 *
 *  Created on: 22 Aug 2019
 *      Author: Yahia
 */

#include "std_types.h"
#include "common_macros.h"

#include "SysTick_interface.h"
#include "SysTick_config.h"
#include "SysTick_private.h"

volatile uint32 SysTick_Tick = 0;

void(*SysTick_CallBack)(void) = STUB;

void SysTick_Init(uint32 OS_TickMS){
#if(AHB_FreQ)
	STK_LOAD = ((RCC_Clock_FreQ/1000)*OS_TickMS) - 1;
#elif(AHB_FreQ_8)
	STK_LOAD = ((RCC_Clock_FreQ/8)/1000) - 1;
#else
	// Error message
#endif
	STK_VAL = 0;
	STK_CTRL = (Enable_SysTick | (Enable_TICKINT << 1) | (SysTick_CLKSOURCE << 2));
}

void Set_CallBack(void(*PTR)(void)){
	SysTick_CallBack = PTR;
}

//void __attribute__ ((section(".after_vectors"),weak))
void SysTick_Handler (void)
{
	STK_VAL = 0;
	STK_CTRL = (Enable_SysTick | (Enable_TICKINT << 1) | (SysTick_CLKSOURCE << 2));
	SysTick_CallBack();
}

float32 CurrentTimeMS(void){
	float32 result = 0;
	result = ((STK_VAL+1)*1000)/((float32)RCC_Clock_FreQ);
	return result;
}

uint8 IsTimeOver(uint32 TimeMS){
	uint8 TimeOverFlag = 0;
	if((((RCC_Clock_FreQ/1000)*TimeMS) - 1) >= STK_VAL)
		TimeOverFlag = 1;
	else
		TimeOverFlag = 0;
	return TimeOverFlag;
}

void STUB(void){

}
