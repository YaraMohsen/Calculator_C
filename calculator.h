/*

 * calculator.h
 *
 *  Created on: Dec 26, 2020
 *      Author: yara mohsen
 */

#ifndef CALCULATOR_H_
#define CALCULATOR_H_


typedef enum{
	FAIL,
	SUCCCESSFUL

}calculator_return_state;




typedef enum{
	INVALID_AVTION,
	NUM1_save,
	NUM2_save,
	SIGN_save,
	EQUAL,
	CLEAR

}ACTION_TYPE;

extern void calculator(void);
extern void calc_init(void);
extern calculator_return_state Get_char(void);
extern void Check_char(void);
extern void Action( ACTION_TYPE input );
extern void print_char(uint8 ch,uint8 index);
extern void print_result(void);
extern void save_number(uint64 *num);
extern uint64 callback(uint64 (*p)(uint64, uint64 ));
extern uint64 add(uint64 num1 ,uint64 num2);
extern uint64 sub(uint64 num1 ,uint64 num2);
extern uint64 div(uint64 num1 ,uint64 num2);
extern uint64 mul(uint64 num1 ,uint64 num2);

#endif /* CALCULATOR_H_ */
