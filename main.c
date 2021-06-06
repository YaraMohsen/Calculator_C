/*
 * main.c
 *
 *  Created on: Dec 11, 2020
 *      Author: SOUQ COMPUTER
 */

#include "STD_Types.h"
#include "BIT_MATH.h"
#include "DIO_Reg.h"
#include "DIO.h"
#include"key_pad.h"
#include"calculator.h"



int main()
{

	DIO_Init();
	LCD_Init();



	while(1){

		calculator();

	}

	return 0;
}
