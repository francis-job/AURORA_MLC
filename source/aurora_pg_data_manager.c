
/**
 * @file	aurora_pg_data_manager.c
 *
 * @brief contains functions related to pattern generation data management
 *
 *
 *
 *
 * @note
 *
 * Revision History:
 *		- 130121 MAP : Creation Date
 */

#include "aurora_pg_data_manager.h"
#include "string.h"
#include"queue.h"
#include"aurora_pat_gen.h"
/*******************************************
 * Const and Macro Defines
 *******************************************/
#define PIT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)
/***********************************
 * Typedefs and Enum Declarations
 ***********************************/
//none

/***********************************
 * External Variable Declarations
 ***********************************/
extern config_ascii_t xconfig_ascii;
extern QueueHandle_t config_queue;
extern QueueHandle_t cmd_queue;
extern QueueHandle_t status_queue;
/***********************************
 * Const Declarations
 ***********************************/
// none

/***********************************
 * Public Variables
 ***********************************/


/***********************************
 * Private Variables
 ***********************************/
// none

/***********************************
 * Private Prototypes
 ***********************************/
// none

/***********************************
 * Public Functions
 ***********************************/





/**
 * @brief config_hex_t ascii_to_hex_converter(config_ascii_t * xconfig_ascii)
 *
 *
 *
 * @param  config_ascii_t * xconfig_ascii
 *
 *
 * @return 	config_hex_t
 *
 * @note  This function will convert the ascii data into he*/

config_hex_t ascii_to_hex_converter(config_ascii_t * xconfig_ascii){

	static config_hex_t xconfig_hex;/*declared as static so that the vaiable has life outside the function*/
	char refresh_rate[2],change_rate[3],cycles[2];

	memcpy(refresh_rate,xconfig_ascii->refresh_rate,sizeof(xconfig_ascii->refresh_rate));
	memcpy(change_rate,xconfig_ascii->change_rate,sizeof(xconfig_ascii->change_rate));
	memcpy(cycles,xconfig_ascii->cycles,sizeof(xconfig_ascii->cycles));

	xconfig_hex.refresh_rate		=	atoi(refresh_rate);
	xconfig_hex.change_rate			=	atoi(change_rate)*10;
	xconfig_hex.cycles				=	atoi(cycles);
	xconfig_hex.data_id             =	xconfig_ascii->data_id        - 48;
	xconfig_hex.color_code			=	xconfig_ascii->color_code     - 48;
	xconfig_hex.start_color[0]		=	xconfig_ascii->start_color[0] - 48;
	xconfig_hex.start_color[1]		=	xconfig_ascii->start_color[1] - 48;
	xconfig_hex.start_color[2]		=	xconfig_ascii->start_color[2] - 48;
	xconfig_hex.end_color[0]		=	xconfig_ascii->end_color[0]   - 48;
	xconfig_hex.end_color[1]		=	xconfig_ascii->end_color[1]   - 48;
	xconfig_hex.end_color[2]		=	xconfig_ascii->end_color[2]   - 48;
	xconfig_hex.step_size			=	xconfig_ascii->step_size      - 48;
	xconfig_hex.mode				=	xconfig_ascii->mode			  - 48;
	xconfig_hex.direction 			=   xconfig_ascii->direction	  - 48;
	return xconfig_hex;

}

/**
 * @brief send_to_queue_blocking( xQueue,byte_to_send)
 *
 *
 *
 * @param
 *
 * @return
 *
 * @note it is used to send to queue from the task:blocking type
 *
 * Revision History:
 * 	- 190121 MAP: 	Creation Date
 */
BaseType_t send_config_task(config_ascii_t* xconfig_ascii)
{
	xQueueReset(config_queue );
	xQueueSend(config_queue,xconfig_ascii,0);
	return pdTRUE;
}



/**
 * @brief BaseType_t receive_config_task_non_blocking()
 *
 *
 *
 * @param
 *
 *
 * @return
 *
 * @note it is non blocking type send to queue
 *
 * Revision History:
 * 	- 190121 MAP: 	Creation Date
 */

BaseType_t receive_config_task_blocking(config_ascii_t* xconfig_ascii)
{

	xQueueReceive(config_queue,xconfig_ascii,portMAX_DELAY);
	if(xconfig_ascii->data_id == '0'){
		return pdTRUE;
	}
	else{
		return pdFALSE;
	}

}

/**
 * @brief BaseType_t send_command_to_task(command_ascii_t* xcommand_ascii)
 *
 *
 *
 * @param command_ascii_t* xcommand_ascii
 *
 *
 * @return
 *
 * @note
 *
 * Revision History:
 * 	- 190121 MAP: 	Creation Date
 */



BaseType_t send_command_to_task(command_ascii_t* xcommand_ascii)
{
	xQueueReset(cmd_queue );
	xQueueSend(cmd_queue,xcommand_ascii,0);

	return pdTRUE;
}

/**
 * @brief BaseType_t receive_command_to_task(command_ascii_t* xcommand_ascii)
 *
 *
 *
 * @param command_ascii_t* xcommand_ascii
 *
 *
 * @return
 *
 * @note
 *
 * Revision History:
 * 	- 190121 MAP: 	Creation Date
 */



BaseType_t receive_command_from_task(command_ascii_t* xcommand_ascii)
{

	xQueueReceive(cmd_queue,xcommand_ascii,0);
	if(xcommand_ascii->data_id == 1)
		return pdTRUE;
	else{
		return pdFALSE;
	}
}

/**
 * @brief BaseType_t send_status_to_task(command_ascii_t* xcommand_ascii)
 *
 *
 *
 * @param command_ascii_t* xcommand_ascii
 *
 *
 * @return
 *
 * @note
 *
 * Revision History:
 * 	- 190121 MAP: 	Creation Date
 */



BaseType_t send_status_to_task(status_ascii_t* xstatus_ascii)
{
	xQueueReset(status_queue );

	xQueueSend(status_queue,xstatus_ascii,0);

	return pdTRUE;
}
/**
 * @brief BaseType_t receive_status_from_task(status_ascii_t* xstatus_ascii)
 *
 *
 *
 * @paramstatus_ascii_t* xstatus_ascii
 *
 *
 * @return
 *
 * @note
 *
 * Revision History:
 * 	- 190121 MAP: 	Creation Date
 */
BaseType_t receive_status_from_task(uint8_t* current_status)
{

	xQueueReceive(status_queue,current_status,50);

	return pdTRUE;
}



/**
  @brief hex_to_ascii_converter(status_hex_t  xstatus_hex)
 *
 *
 *
  @param  status_hex_t  xstatus_ascii
 *
 *
 * @return 	status_ascii_t
 *
 * @note  This function will convert the hex data into ascii data and store it into status_ascii_t type struct and returns it.
 *
 * Revision History:
 * 	- 190121 MAP: 	Creation Date
 * 	- 200121 NS : added minor change in the data being sent in cycles left.
 */

status_ascii_t hex_to_ascii_converter(status_hex_t * xstatus_hex){

	static status_ascii_t xstatus_ascii;/*declared as static so that the vaiable has life outside the function*/


	xstatus_ascii.current_color[0]  = xstatus_hex->current_color[0] + 48;
	xstatus_ascii.current_color[1]  = xstatus_hex->current_color[1] + 48;
	xstatus_ascii.current_color[2]  = xstatus_hex->current_color[2] + 48;

	xstatus_ascii.cycles_left[0]=((xstatus_hex->cycles_left)/10) + 48;
	xstatus_ascii.cycles_left[1]=((xstatus_hex->cycles_left)%10) + 48;



	return xstatus_ascii;


}




