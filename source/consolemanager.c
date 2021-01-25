/**
 * @file	consolemanager.c
 * @brief	Functions that manages the console.
 *
 *
 * @note
 *
 * Revision History:
 *		- 110121 DA : Creation Date
 *		  130121 DA:  Added validation functions
 *		  140121 DA:  Modified execution_validation function
 */

/* FreeRTOS kernel includes. */
#include <consolemanager.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include "board.h"
#include "fsl_uart.h"
#include "string.h"
#include "stdlib.h"
#include "datamanager.h"
#include "pin_mux.h"

/*******************************************
 * Const and Macro Defines
 *******************************************/
// none

/***********************************
 * Typedefs and Enum Declarations
 ***********************************/
//none

/***********************************
 * External Variable Declarations
 ***********************************/
extern QueueHandle_t uart_queue;
extern config_ascii_t curr_config;

/***********************************
 * Const Declarations
 ***********************************/
// none

/***********************************
 * Public Variables
 ***********************************/
BaseType_t config_task_woke;
/***********************************
 * Private Variables
 ***********************************/
//none

/***********************************
 * Private Prototypes
 ***********************************/
static int color(uint8_t, uint8_t);
static int step_size(uint8_t, uint8_t);
static int mode(uint8_t, uint8_t);
static int direction(uint8_t, uint8_t);

/***********************************
 * Public Functions
 ***********************************/

/**
 * @brief Enables the uart interrupt
 *
 * Sets the priority of the uart interrupt,
 *  and enables the receiver interrupts
 *
 * @param 	none.
 * @return 	none
 *
 * @note
 *
 * Revision History:
 * 	- 120121 DA: 	Creation Date
 *
 */
void enable_uart_isr(void)
{
	  /* Enable at the NVIC */
	NVIC_SetPriority (UART_IRQn, configMAX_SYSCALL_INTERRUPT_PRIORITY-1);
	EnableIRQ(UART_IRQn);
	UART_EnableRx(UART,1);      // enables uart receiver
	UART_EnableInterrupts (UART,kUART_RxDataRegFullInterruptEnable);

}

/**
 * @brief Performs uart initialization.
 *
 * Function calls the uart api function to
 * initialize the uart.
 *
 * @param 	none.
 * @return 	none
 *
 * @note
 *
 * Revision History:
 * 	- 120121 DA: 	Creation Date
 *
 */
void uart_init(void)
{
    BOARD_UARTPins();
}
/**
 * @brief Uart interrupt handler
 *
 * Function represent the uart interrupt
 * handler function. Clears all the interrupt
 * flags, disables the MSBF and stores the
 * received data to the queue.
 *
 * @param 	none.
 * @return 	none
 *
 * @note
 *
 * Revision History:
 * 	- 120121 DA: 	Creation Date
 */
void UART_IRQHandler(void)
{
    /* Clear interrupt flag.*/
	UART_ClearStatusFlags(UART,kUART_RxDataRegFullInterruptEnable);
	UART_ClearStatusFlags(UART,kUART_RxActiveEdgeFlag);

	uart_msbf(UART,0U);                //disables MSBF( most significant bit first).

	uint8_t data2[1];
	data2[0]=UART_ReadByte(UART);
	config_task_woke = false;
	xQueueSendFromISR(uart_queue,data2,&config_task_woke);
	portYIELD_FROM_ISR(&config_task_woke);
    __DSB();
}
/**
 * @brief Receive the data from queue and return the data
 *
 * Stores the data from the queue, received from isr
 * function will be in the blocked state till the
 * data is received.
 * Displays the same data and returns the same.
 *
 * @param 	none.
 * @return 	data that is received from the uart
 *
 * @note
 *
 * Revision History:
 * 	- 120121 DA: 	Creation Date
 */
int get_keyinput()
{

	uint8_t data;
	BaseType_t xstatus;

    xstatus=xQueueReceive(uart_queue,&data,portMAX_DELAY);

    if(xstatus==pdPASS){
    	UART_WriteByte(UART,data);
     	return data;
     }
    return 0;
}
/**
 * @brief Receive the data from queue and return the data
 *
 * Stores the data from the queue, received from isr
 * function will be in the non blocked state and will
 * return immediately if no data in queue.
 * Displays the same data and returns the same.
 *
 * @param 	none.
 * @return 	data that is received from the uart
 *
 * @note
 *
 * Revision History:
 * 	- 200121 DA: 	Creation Date
 */
int get_keyinput_nonblocking()
{

	uint8_t data;
	BaseType_t xstatus;

    xstatus=xQueueReceive(uart_queue,&data,0);

    if(xstatus==pdPASS){
    	return data;
     }
    return 0;
}
/**
 * @brief Disables the uart receiver
 *
 * This function enables the UART receiver and
 * all the data from uart receiver is ignored.
 *
 * @param 	none.
 * @return 	none
 *
 * @note
 *
 * Revision History:
 * 	- 190121 DA: 	Creation Date
 */
void uart_slave_enable()
{
	UART_EnableRx(UART,0);  //disables uart receiver
}
/**
 * @brief Creates new queue
 *
 * Create new Queue for to receive
 * the key from the  uart isr.
 *
 * @param 	none.
 * @return 	bool	0 - if success queue created
 * 					1 - if failure, queue not created
 *
 * @note
 *
 * Revision History:
 * 	- 120121 DA: 	Creation Date
 */
int create_uartqueue()
{
	uart_queue=xQueueCreate(1,sizeof(uint8_t));
	if(uart_queue!=NULL){
		return SUCCESS;
	}
	return FAIL;
}
/**
 * @brief Displays a message
 *
 * This function displays the message

 *
 * @param 	const uint8_t *data	: message to be displayed
 * @return 	none
 *
 * @note
 *
 * Revision History:
 * 	- 130121 DA: 	Creation Date
 */
void print_message(char data[])
{
	int8_t length=strlen((const char*)data);
	UART_WriteBlocking(UART,(uint8_t*)data,length);
}
/**
 * @brief Displays a message/data
 *
 * This function displays the message/data

 *
 * @param 	data	: message to be displayed
 * 			size of the data displayed
 * @return 	none
 *
 * @note
 *
 * Revision History:
 * 	- 120121 DA: 	Creation Date
 *
 */
void print_data(uint8_t data[],uint8_t size)
{
	UART_WriteBlocking(UART,(uint8_t*)data,size);
	return;
}
/**
 * @brief validate the buffer received
 *
 * Function validates the buffer
 *
 * @param 	    id representing the which buffer
 * 				buffer to be validated
 * @return 	bool	success - if vaild input is received
 * 					failed - If invalid input is received
 *
 * @note
 *
 * Revision History:
 * 	- 130121 DA: 	Creation Date
 */
int validate_buffer(param_t buffer_id, uint8_t *data)
{
	uint32_t int_value;
	char *p=0;

	switch(buffer_id)
	{

		case REFRESH_RATE_ID:
			int_value=strtol((const char*)data,&p,10);
			if(int_value>=MIN_REFRESH_RATE && int_value<=MAX_REFRESH_RATE && *p==0)
				return SUCCESS;
			break;

		case CYCLES_ID:
			int_value=strtol((const char*)data,&p,10);
			if(int_value>=MIN_CYCLE && int_value<=MAX_CYCLE && *p==0)
				return SUCCESS;
			break;


		case CHANGE_RATE_ID:
			int_value=strtol((const char*)data,&p,10);
			if(int_value>=MIN_CHANGE_RATE && int_value<=MAX_CHANGE_RATE && *p==0)
				return SUCCESS;
			break;


		default:
			return FAIL;
	}
	return FAIL;

}
/**
 * @brief validate the byte received
 *
 * Function validates the byte based on the
 * received id and index.
 *
 * @param 	    byte id -id representing the which buffer
 * 						 to be validated
 * 				index - index for which validation is to be done.
 * @return 	bool	success - if valid input is received
 * 					failed - If invalid input is received
 * @note
 *
 * Revision History:
 * 	- 130121 DA: 	Creation Date
 */

int validate_byte(param_t byte_id, uint8_t index , uint8_t data)
{
	uint8_t status;
	switch(byte_id)
	{
		case START_COLOR_ID:
			status=color(index,data);
			break;

		case END_COLOR_ID:
				status=color(index,data);
				break;

		case STEP_SIZE_ID:
			status=step_size(index,data);
			break;

		case MODE_ID:
			status=mode(index,data);
			break;

		case DIRECTION_ID:
			status=direction(index,data);
			break;

		default:
			return FAIL;

	}
	return status;
}
/**
 * @brief validate color bytes received
 *
 * Function validates the color byte based on the
 * index. Both the start color and end color is
 * validated by this function.
 *
 * @param 	    byte - byte to be validated.
 * 				index - index for which validation is to be done.
 * @return 	bool	success - if valid input is received
 * 					failed - If invalid input is received
 * @note
 *
 * Revision History:
 * 	- 130121 DA: 	Creation Date
 */

static int color(uint8_t index, uint8_t byte)
{
	switch(index)
	{
		case 0:
			if(byte<'0'||byte>'7')
				return FAIL;
			break;
		case 1:
			if(byte!=',')
				return FAIL;
			break;
		case 2:
			if(byte<'0'||byte>'7')
				return FAIL;
			break;
		case 3:
			if(byte!=',')
				return FAIL;
			break;
		case 4:
			if(byte<'0'||byte>'7')
				return FAIL;
			break;
		case 5:
			if(byte!='\r')
				return FAIL;
			break;
		default:
			return FAIL;
	}
	return SUCCESS;
}
/**
 * @brief validate bytes for step size.
 *
 * Function validates the bytes of step size
 * based on the index.
 *
 * @param 	    byte - byte to be validated.
 * 				index - index for which validation is to be done.
 *
 * @return 		success - if valid input is received
 * 				failed - If invalid input is received
 * @note
 *
 * Revision History:
 * 	- 130121 DA: 	Creation Date
 */
static int step_size(uint8_t index, uint8_t byte)
{
	switch(index)
	{
		case 0:
			if(byte<'1'||byte>'7')
				return FAIL;
			break;
		case 1:
			if(byte!='\r')
				return FAIL;
			break;
		default:
			return FAIL;
	}
	return SUCCESS;

}
/**
 * @brief validate bytes for mode.
 *
 * Function validates the bytes of mode
 * based on the index.
 *
 * @param 	    byte - byte to be validated.
 * 				index - index for which validation is to be done.
 *
 * @return 		success - if valid input is received
 * 				failed - If invalid input is received
 * @note
 *
 * Revision History:
 * 	- 130121 DA: 	Creation Date
 */
static int mode(uint8_t index, uint8_t byte)
{
	switch(index)
	{
		case 0:
			if(byte!='1'&& byte!='2')
				return FAIL;
			break;
		case 1:
			if(byte!='\r')
				return FAIL;
			break;
		default:
			return FAIL;
	}
	return SUCCESS;

}
/**
 * @brief validate bytes for direction.
 *
 * Function validates the bytes of direction
 * based on the index.
 *
 * @param 	    byte - byte to be validated.
 * 				index - index for which validation is to be done.
 *
 * @return 		success - if valid input is received
 * 				failed - If invalid input is received
 * @note
 *
 * Revision History:
 * 	- 130121 DA: 	Creation Date
 */
static int direction(uint8_t index, uint8_t byte)
{
	switch(index)
	{
		case 0:
			if(byte<'1'||byte>'3')
				return FAIL;
			break;
		case 1:
			if(byte!='\r')
				return FAIL;
			break;
		default:
			return FAIL;
	}
	return SUCCESS;
}
/**
 * @brief compares the start color and end color
 *
 * Function checks whether the start color is
 * smaller than the end color.
 *
 * @param 	Start color
 * 			End color
 * @return 	bool	success - if end color is greater/equal to start color.
 * 					failed - if end color is smaller.
 *
 * @note
 *
 * Revision History:
 * 	- 140121 DA: 	Creation Date
 */
int compare(uint8_t *start_color, uint8_t *end_color)
{

   for(int i=0;i<5;i++)
   {
		if(*(start_color+i)>*(end_color+i))
			return FAIL;

   }
	return SUCCESS;
}
/**
 * @brief validate the byte received
 * during the execution.
 *
 * Function validates the byte received
 * during the execution.
 *
 * @param 	byte - byte to be validated
 * @return 	bool	success - if valid input received
 * 					failed - if invalid input was received.
 *
 * @note
 *
 * Revision History:
 * 	- 140121 DA: 	Creation Date
 */
int validate_executionkey(uint8_t byte)
{
	uint8_t mode_value = curr_config.mode;

	if(byte=='Q'){
		return SUCCESS;
	}
	else if(mode_value == '2' && byte == ' '){
		return SUCCESS;
	}

	else if(mode_value =='1'&&( byte == '>' || byte == '<')){
		return SUCCESS;
	}
	return FAIL;
}

