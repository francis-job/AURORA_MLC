/*
 * consolemanager.h
 *
 *
 * Revision History:
 *		- 110121 DA : Creation Date
  */
#ifndef CONSOLEMANAGER_H_
#define CONSOLEMANAGER_H_
#include "stdbool.h"
#include "MK64F12.h"
/*******************************************
 * Const and Macro Defines
 *******************************************/
#define UART UART0
#define UART_CLKSRC UART0_CLK_SRC
#define UART_CLK_FREQ CLOCK_GetFreq(UART0_CLK_SRC)
#define UART_IRQn UART0_RX_TX_IRQn
#define UART_IRQHandler UART0_RX_TX_IRQHandler

#define MIN_REFRESH_RATE 1U
#define MAX_REFRESH_RATE 99U
#define MIN_CYCLE 0U
#define MAX_CYCLE 99U
#define MIN_CHANGE_RATE 1U
#define MAX_CHANGE_RATE 999U
/***********************************
 * Typedefs and Enum Declarations
 ***********************************/
typedef enum _parameter_id
{
	START_COLOR_ID,
	END_COLOR_ID,
	STEP_SIZE_ID,
	MODE_ID,
	DIRECTION_ID,
	REFRESH_RATE_ID,
	CYCLES_ID,
	CHANGE_RATE_ID,
}param_t;


/***********************************
 * Const Declarations
 ***********************************/
// none

/***********************************
 * Variable Declarations
 ***********************************/
// none

/***********************************
 * Prototypes
 ***********************************/

void uart_init(void);
/**
 * @brief Enables the uart interrupt
 *
 * Sets the priority of the uart interrupt,
 *  and enables the reciever interrupts
 *
 * @param 	none.
 * @return 	none
 *
 * @note
 *
 * Revision History:
 * 	- 120121 DA: 	Creation Date
 */
void enable_uart_isr();

/*!
 * @brief Enables or disables the UART MSBF.
 *
 * This function used to make ensure that
 * LSB (bit0) is the first bit that is
 * transmitted following the start bit,
 * i,e disables MSBF( most significant bit first). .
 *
 * @param base UART peripheral base address.
 * @param enable True to enable, false to disable.
 */
static inline void uart_msbf(UART_Type *base, bool enable)
{
    if (enable)
    {
        base->S2 |= UART_S2_MSBF_MASK;
    }
    else
    {
        base->S2 &= ~UART_S2_MSBF_MASK;
    }
}

/**
 * @brief Receive the data from queue and return the data
 *
 * Stores the data from the queue, received from isr
 * function will be in the blocked state till the
 * data is recieved.
 * Displays the same data and returns the same.
 *
 * @param 	none.
 * @return 	data
 *
 * @note
 *
 * Revision History:
 * 	- 120121 DA: 	Creation Date
 */

int get_keyinput();
/**
 * @brief Receive the data from queue and return the data
 *
 * Stores the data from the queue, received from isr
 * function will be in the non blocked state and will
 * return immediately when no data in queue.
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
int get_keyinput_nonblocking();
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
void uart_slave_enable();
/**
 * @brief Creates new queue
 *
 * Create new Queue for to receive
 * the key from the  uart isr.
 *
 * @param 	none.
 * @return 	bool	success/failed
 *
 * @note
 *
 * Revision History:
 * 	- 120121 DA: 	Creation Date
 */
int create_uartqueue();

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
void print_message(char *);

/**
 * @brief Displays a message
 *
 * This function displays the message/data

 *
 * @param 	const uint8_t *data	: message to be displayed
 * 			size of the data displayed
 * @return 	none
 *
 * @note
 *
 * Revision History:
 * 	- 130121 DA: 	Creation Date
 */
void print_data(uint8_t * data,uint8_t);
/**
 * @brief validate the buffer recieved
 *
 * Function validates the buffer
 *
 * @param 	    id representing the which buffer
 * 				buffer to be validated
 * @return 	bool	success/failed
 *
 * @note
 *
 * Revision History:
 * 	- 120121 DA: 	Creation Date
 */
int validate_buffer(uint8_t , uint8_t*);

/**
 * @brief validate the byte recieved
 *
 * Function validates the byte based on the
 * recieved id and index.
 *
 * @param 	    id representing the which buffer
 * 				buffer to be validated
 * @return 	bool	success/failed
 *
 * @note
 *
 * Revision History:
 * 	- 120121 DA: 	Creation Date
 */
int validate_byte(uint8_t, uint8_t , uint8_t);

/**
 * @brief validate the byte received
 * during the execution.
 *
 * Function validates the byte based on the
 * recieved id and index.
 *
 * @param 	key/data to be validated
 * @return 	bool	success/failed
 *
 * @note
 *
 * Revision History:
 * 	- 140121 DA: 	Creation Date
 */
int validate_executionkey(uint8_t);
/**
 * @brief compares the start color and end color
 *
 * Function checks whether the start color is
 * smaller than the end color.
 *
 * @param 	Start color
 * 			End color
 * @return 	bool	success/failed
 *
 * @note
 *
 * Revision History:
 * 	- 140121 DA: 	Creation Date
 */
int compare(uint8_t*, uint8_t*);


#endif /* CONSOLEMANAGER_H_ */
