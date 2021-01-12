/*
 * main.c
 *
 *  Created on: Dec 11, 2020
 *      Author: SOUQ COMPUTER
 */
//#include "memoryMap_avr32.h"
#include "STD_Types.h"
#include "BIT_MATH.h"
#include "DIO_Reg.h"
#include "DIO.h"
//#include"ADC.h"
//#include"SEVEN_SEG.h"
#include"key_pad.h"
//#include"sensorLDR.h"
#include"calculator.h"

//#include "EX_interrupt.h"

#define F_CPU 8000000
#include<util/delay.h>






int main()
{

	DIO_Init();
	LCD_Init();



/*
uint8 arr[15]="yara mohsen";
uint8 i=0;
for(i=0;arr[i]!='\0';i++){
	LCD_WriteData(arr[i]);
}
*/


	while(1){

		calculator();

	}

	return 0;
}
