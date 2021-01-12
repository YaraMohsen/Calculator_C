/*

 * calculator.c
 *
 *  Created on: Dec 26, 2020
 *      Author: yara mohsen
 */


#include "DIO_Reg.h"
#include "STD_Types.h"
#include "BIT_MATH.h"
#include "DIO.h"
#include "key_pad.h"
#include"LCD.h"
#include"calculator.h"


/*************************************************************************
* Initialization of variables :
* ********
*
* char_input  : the current char set by "Get_char()"function
* sign        : refer to the operator , used to determine num1 and num2
* cound_digit : count if the input numbers
*
* invalid_flag : should be volatile as it removed
*               during optimazation of the compiler
* *************************************************************************/

uint8 char_input='%';
uint8 sign='%',count_digit=0;
uint64 number1=0,number2=0;

volatile uint8 invalid_flag=0;
uint64 res=0;
uint8 index_sign=255,index=0;

uint8 invalid[]="invalid input";


volatile uint64 (*ptr)(uint64,uint64 );

/*************************************************************************
* calculator :
* ************
*
* input  : void
*
* output : void
*
* The main function called by the application. used 2functions
* " Get_char() - Check_char()" to handel the calculation process
*
* *************************************************************************/


void calculator(void)
{
    calc_init();

    while( char_input!= 'c' )
    {
        if(Get_char()==SUCCCESSFUL)
        {
            Check_char();
            index++;

            if(index==255){char_input= 'c';}
        }
    }

}

/*************************************************************************
* Get_char :
* **********
*
* input  : void
* output : return the state of enum "calculator_return_state"
*
* use the keypad files to gets the input and saves it into variable
* "char_input" and check if a key is pressed return "SUCCCESSFUL"
* if no key pressed return "FAIL"
*
* *************************************************************************/


calculator_return_state Get_char(void)
{

    calculator_return_state value;

    char_input = pressed_key();

    if(char_input!='%')
    {
        value= SUCCCESSFUL;
    }

    else
    {
        value= FAIL;

    }

 return value;
}

/*************************************************************************
* Check_char :
* ************
*
* input  : void
*
* output : void
*
* after get the char, it uses the global variable "char_input" and check
* its state : "number1 , number2 , operator , equal , c or invalid input"
* using enum "ACTION_TYPE"
*
* saves the state in local variable "input"  and pass it to "Action" function to
* take the action based on the state of the "input" variable.
*
* *************************************************************************/


void Check_char(void)
{

    ACTION_TYPE input;
    /**********************************************
    * check if numbers
    * separate num1 and num2
    * count the digit
    *********************************************/
    if((char_input >='0') && (char_input <='9'))
    {

        if(char_input=='0' && count_digit==0){}

        else
        {
            if(count_digit<5)
            {
                if(sign=='%')
                {
                    input=NUM1_save;
                }

                else if(sign!='%')
                {
                    input=NUM2_save;


                }
                count_digit++;
            }

            else if(count_digit==5)
            {
                input=INVALID_AVTION;

            }
        }

    }

    /**********************************************
    * check if operator
    * check the right position of the operator
    * edit the index of operator to rewrite it
    *********************************************/

    else if((char_input =='*') || (char_input =='/')||(char_input =='-') || (char_input =='+'))
    {

        count_digit=0;  //count digits for the new number

        if(index==0)    //first char is op
        {
            input=INVALID_AVTION;
        }


        else
        {
        	//index_sign=index;

            if(index==(index_sign+1) )
            {
                index=index_sign;
            }
            else
            {
                index_sign=index;
            }
            input=SIGN_save;
        }

    }
    /**********************************************
    * check if c key is pressed
    *********************************************/
    else if(char_input=='c')
    {
        input=CLEAR;
    }

    /**********************************************
    * check if = key is pressed
    *********************************************/
    else if ((char_input =='=' ) && (index!=(index_sign+1)))        //if it is pressed = and sign=& then there is no second number
    {

        input=EQUAL;


    }

    /**********************************************
    * check if invaled case
    *********************************************/
    else
    {
        input=INVALID_AVTION;

    }

    /****************************
     * pass the final decision
     * "input" to Action function
     ******************************/

    Action(input);
}


/*************************************************************************
* Action :
* ********
*
* input  : variable from enum "ACTION_TYPE" refer the state of the calculator
*
* output : void
*
* this function take decisions , if input :
* ****************************************
* 1. invalid : it stop all things except printing the input "char"s
*              till the equal key pressed.
*
* 2. equal   : call callback function and finish the calculation
*              then save it in the result
*              then send result ot print function to print "invalid or result"
*
* 3. num1    : call save function and send the address of num1 to edit it.
* 4. num2    : call save function and send the address of num2 to edit it.
*
* 5. sign    : switch on the operator and send the address of the function
*              to the global pointer "ptr" to use it in the callback function
*
* 6.clear    : clear all
*
* *************************************************************************/

void Action( ACTION_TYPE input )
{

    print_char(char_input,index);

    if(input==INVALID_AVTION )
    {
        invalid_flag=1;

    }

    if((input==EQUAL )|| (char_input=='='))
    {
        print_result();
    }
    else if(input==CLEAR)
    {
        LCD_voidClear();
    }

    else if(invalid_flag==0)
    {

        switch(input)
        {
        case NUM1_save:
            save_number(&number1);
            break;

        case NUM2_save:
            save_number(&number2);
            break;

        case SIGN_save:
            sign=char_input;

            switch(sign)
            {
            case '+':
                ptr=&add;
                break;
            case '*':
                ptr=&mul;
                break;
            case '/':
                ptr=&div;
                break;
            case '-':
                ptr=&sub;
                break;
            }

            break;
        }
    }

}



/*************************************************************************
* callback :
* **********
*
* input : pointer to function take 2 arguments from uint64 type
*         return uint64
*
* output : return uint64
*
* used to call the functions of calculations " add - sub - mul - div "
* using global pointer to function
*
* note:
* ****
* this pointer initialized by the address of add function
* by calc_initi function
*
* *************************************************************************/
uint64 callback(uint64 (*p)(uint64, uint64 ))
{
    return (p)(number1,number2);

}

/*************************************************************************
* print_char:
* ***********
*
* input  : take 2 arguments : the char form uint8 type and its index uint8.
*
* output : void
*
* this function use LCD functions : LCD_GoTo() ,LCD_WriteData()
*
* *************************************************************************/
void print_char(uint8 ch,uint8 index)
{
    LCD_GoTo(index,0);
    LCD_WriteData(ch);
}

/*************************************************************************
* print_result:
* ************
*
* input  : take "pointer to uint8" : take the address of invalid flag
*
* output : void
*
* this function use LCD functions :
* LCD_GoTo() ,LCD_WriteString(),LCD_WriteNumber64()
*
* check if invalid flag is 1 : print unvaliad input
* check if invalid flag is 0 : print the result
*
* *************************************************************************/
void print_result(void)
{


    LCD_GoTo(1,1);
    res=callback(ptr);

    if(invalid_flag==1)
    {

        LCD_WriteString(invalid);
    }
    else
    {

        LCD_WriteNumber64(res);
    }
    char_input= 'c';
}

/*************************************************************************
* save_number:
* ************
*
* input  : take pointer to uint64 : take address of number and save digits on it
*
* output : void
*
* *************************************************************************/
void save_number(uint64 *num)
{
    (*num)=(uint64)((*num)*10+(char_input-'0'));

}

/*************************************************************************
* add:
* *****
*
* input  : 2 uint64 numbers
*
* output : uint64 the sum on the 2 numbers
* *************************************************************************/
uint64 add(uint64 num1,uint64 num2)
{
    return (num1+num2);

}
/*************************************************************************
* sub:
* *****
*
* input  : 2 uint64 numbers
*
* output : uint64 the sub on the 2 numbers, if num1<num2 =invalid input
* *************************************************************************/
uint64 sub(uint64 num1,uint64 num2)
{
    if(num1<num2)
    {
    	  LCD_GoTo(0,1);
    	  LCD_WriteData('-');
    	  return (num2-num1);
    }
    else
    {
        return (num1-num2);
    }

}
/*************************************************************************
* div:
* *****
*
* input  : 2 uint64 numbers
*
* output : uint64 the div on the 2 numbers , if num2 =0 =invalid input
* *************************************************************************/
uint64 div(uint64 num1,uint64 num2)
{
    if(num2==0)
    {
    	invalid_flag=1;
    }
    else
    {
        return (num1/num2);
    }

    return 0;

}
/*************************************************************************
* mul:
* *****
*
* input  : 2 uint64 numbers
*
* output : uint64 the mul on the 2 numbers
* *************************************************************************/
uint64 mul(uint64 num1,uint64 num2)
{
    return (num1*num2);
}

/*************************************************************************
* calc_init:
* *****
*
* input  : void
*
* output : void
*
* initalize all variables
* *************************************************************************/
void calc_init(void)
{
    ptr=&add;
    char_input='%';
    sign='%';
    count_digit=0;
    number1=0;
    number2=0;
    invalid_flag=0;
    res=0;
    index_sign=250;
    index=0;
}
