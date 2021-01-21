/**
 * @file	datamanager.c
 * @brief	Functions related configuration structure and Queue management to other task.
 *
 * All functions to manage configuration and data to other tasks.
 *
 * @note
 *
 * Revision History:
 *		- 110121 DA : Creation Date
 *		- 150121 DA : Added configuration update functions
 *		- 190121 DA : Added command update functions,functions
 *					  for send/receive queue.
 *		-
  */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "stdint.h"
#include "string.h"
#include "datamanager.h"
#include "consolemanager.h"

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
config_ascii_t curr_config;
command_ascii_t cmd_ascii;
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
// none

/***********************************
 * Private Variables
 ***********************************/
//none

/***********************************
 * Private Prototypes
 ***********************************/

static int startcolor_update(uint8_t*);
static int endcolor_update(uint8_t*);
static int refreshrate_update(uint8_t*);
static int stepsize_update(uint8_t*);
static int direction_update(uint8_t *);
static int cycles_update(uint8_t *);
static int mode_update(uint8_t *);
static int changerate_update(uint8_t *);


/***********************************
 * Public Functions
 ***********************************/

/**
 * @brief saves the default configuration to the structure.
 *
 * Function loads a default configuration to the
 * each element of the  structure
 *
 * @param 	 config*  pointer to the configuration structure.
 * @return 	 none.
 *
 * @note
 *
 * Revision History:
 * 	- 150121 DA: 	Creation Date
 */
void get_default_config(config_ascii_t *config){

    /* Initializes the configure structure to null. */
    memset(config, '\0', sizeof(*config));

    config->data_id ='0';
	config->color_code = '3';

	config->refresh_rate[0] = '1';
	config->refresh_rate[1] = '0';

	config->start_color[0] = '2';
	config->start_color[1] = '0';
	config->start_color[2] = '0';
	config->end_color[0] = '7';
	config->end_color[1] = '7';
	config->end_color[2] = '7';

	config->step_size = '2';
	config->mode = '1';
	config->change_rate[0] ='2';
	config->change_rate[1] ='0';
	config->change_rate[2] ='9';

	config->cycles[0] = '2';
	config->cycles[1] = '0';
	config->direction = '1';
}
/**
 * @brief saves new configuration to the structure.
 *
 * Function update the configuration structure with
 * new data. Each element of the  structure can be
 * updated differently.
 *
 * @param 	 buffer id - id to represent the each
 *                          element of structure.
 *           data - new value to write to the structure.
 * @return 	success - if structure updated with new value.
 * 			failure. - if structure not updated
 *
 * @note
 *
 * Revision History:
 * 	- 150121 DA: 	Creation Date
 */
int update_config_struct(uint8_t buffer_id, uint8_t *data)
{
	uint8_t status;
	switch(buffer_id)
	{
		case START_COLOR_ID:
			status=startcolor_update(data);
			break;

		case END_COLOR_ID:
			status=endcolor_update(data);
			break;

		case STEP_SIZE_ID:
			status=stepsize_update(data);
			break;

		case MODE_ID:
			status=mode_update(data);
			break;

		case CYCLES_ID:
			status=cycles_update(data);
			break;

		case CHANGE_RATE_ID:
			status=changerate_update(data);
			break;

		case DIRECTION_ID:
			status=direction_update(data);
			break;

		case REFRESH_RATE_ID:
			status=refreshrate_update(data);
			break;
	}
	return status;
}
/**
 * @brief update new value to the start color.
 *
 * Function update the configuration structure with
 * new data for element start_color of the  structure.
 *
 * @param    data - new value to write to the structure.
 * @return 	success.
 *
 * @note
 *
 * Revision History:
 * 	- 150121 DA: 	Creation Date
 */
static int startcolor_update(uint8_t *data)
{
	for(int i=0;i<sizeof(curr_config.start_color);i++){
		curr_config.start_color[i]=*(data+2*i);
	}
	return 0;
}
/**
 * @brief update new value to the end color.
 *
 * Function update the configuration structure with
 * new data for element end_color of the  structure.
 *
 * @param    data - new value to write to the structure.
 * @return 	success.
 *
 * @note
 *
 * Revision History:
 * 	- 150121 DA: 	Creation Date
 */
static int endcolor_update(uint8_t *data)
{
	for(int i=0;i<sizeof(curr_config.end_color);i++){
		curr_config.end_color[i]=*(data+2*i);
	}
	return 0;
}
/**
 * @brief update new value to the step size.
 *
 * Function update the configuration structure with
 * new data for element step size of the  structure.
 *
 * @param    data - new value to write to the structure.
 * @return 	success.
 *
 * @note
 *
 * Revision History:
 * 	- 150121 DA: 	Creation Date
 */
static int stepsize_update(uint8_t *data)
{
	curr_config.step_size=*data;
	return 0;
}
/**
 * @brief update new value to the mode configuration.
 *
 * Function update the configuration structure with
 * new data for element mode of the  structure.
 *
 * @param    data - new value to write to the structure.
 * @return 	success.
 *
 * @note
 *
 * Revision History:
 * 	- 150121 DA: 	Creation Date
 */
static int mode_update(uint8_t *data)
{
	curr_config.mode=*data;
	return 0;
}
/**
 * @brief update new value to the no: of cycles.
 *
 * Function update the configuration structure with
 * new data for element cycles of the  structure.
 *
 * @param    data - new value to write to the structure.
 * @return 	success.
 *
 * @note
 *
 * Revision History:
 * 	- 150121 DA: 	Creation Date
 */
static int cycles_update(uint8_t *data)
{
	for(int i=0;i<sizeof(curr_config.cycles);i++){
	curr_config.cycles[i]=*(data+i);
	}
	return 0;
}
/**
 * @brief update new value to the change rate.
 *
 * Function update the configuration structure with
 * new data for element change rate of the  structure.
 *
 * @param    data - new value to write to the structure.
 * @return 	success.
 *
 * @note
 *
 * Revision History:
 * 	- 150121 DA: 	Creation Date
 */
static int changerate_update(uint8_t *data)
{
	for(int i=0;i<sizeof(curr_config.change_rate);i++){
		curr_config.change_rate[i]=*(data+i);
	}
	return 0;
}
/**
 * @brief update new value to the direction.
 *
 * Function update the configuration structure with
 * new data for element direction of the  structure.
 *
 * @param    data - new value to write to the structure.
 * @return 	success.
 *
 * @note
 *
 * Revision History:
 * 	- 150121 DA: 	Creation Date
 */
static int direction_update(uint8_t *data)
{
	curr_config.direction=*data;
	return 0;
}
/**
 * @brief update new value to the refresh rate.
 *
 * Function update the configuration structure with
 * new data for element step size of the  structure.
 *
 * @param    data - new value to write to the structure.
 * @return 	success.
 *
 * @note
 *
 * Revision History:
 * 	- 150121 DA: 	Creation Date
 */
static int refreshrate_update(uint8_t *data)
{
	for(int i=0;i<sizeof(curr_config.refresh_rate);i++){
		curr_config.refresh_rate[i]=*(data+i);
	}
	return 0;
}
/**
 * @brief saves new command to the commamd_struct.
 *
 * Function update the command structure with
 * new data. Both the element of the structure
 * is updated here.
 *
 * @param 	data - new value to write to the structure.
 * @return 	success/failure.
 *
 * @note
 *
 * Revision History:
 * 	- 190121 DA: 	Creation Date
 */
int update_cmd_struct(uint8_t data)
{
	cmd_ascii.data_id='1';
	cmd_ascii.command=data;

	return 0;
}
/**
 * @brief sends the configuration to other task
 *
 * Function first clears the config queue and
 * send the configuration to the config queue.
 *
 * @param 	none
 * @return 	success
 *
 * @note
 *
 * Revision History:
 * 	- 190121 DA: 	Creation Date
 */
BaseType_t send_config_pg()
{
	xQueueReset(config_queue );
	xQueueSend(config_queue,&curr_config,0);
	return pdPASS;
}
/**
 * @brief sends the command to other task
 *
 * Function first clears the command(cmd) queue and
 * send the configuration to the cmd queue.
 *
 * @param 	none
 * @return 	success
 *
 * @note
 *
 * Revision History:
 * 	- 190121 DA: 	Creation Date
 */
BaseType_t send_cmd_pg()
{
	xQueueReset(cmd_queue );
	xQueueSend(cmd_queue,&cmd_ascii,0);
	return pdPASS;
}
/**
 * @brief receive the status from other task
 *
 * Function gets the values from the status queue
 * and stores it to the status_data array and
 * returns the same array.
 *
 * @param 	none
 * @return 	status_data array
 *
 * @note
 *
 * Revision History:
 * 	- 190121 DA: 	Creation Date
 */
uint8_t *receive_status_pg()
{
	static uint8_t status_data[5];
	xQueueReceive(status_queue,&status_data,portMAX_DELAY);
	return status_data;
}
/**
 * @brief reverse the direction
 *
 * Function reverse the direction and the
 * update the same in the configuration structure.
 * The function is used when the MLC acts in
 * slave mode.
 *
 * @param 	none
 * @return 	1- success
 *
 * @note
 *
 * Revision History:
 * 	- 190121 DA: 	Creation Date
 */
int reverse_direction()
{
	if(curr_config.direction == '1')
		curr_config.direction = '2';
	else if(curr_config.direction == '2')
		curr_config.direction = '1';
	else if(curr_config.direction == '3')
		curr_config.direction = '4';
	else
		return 1;

	return 0;
}
