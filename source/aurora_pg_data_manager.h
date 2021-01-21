/*
 * aurora_pg_data_manager.h
 *
 *  Created on: 13-Jan-2021
 *      Author: azf
 */

#ifndef AURORA_PG_DATA_MANAGER_H_
#define AURORA_PG_DATA_MANAGER_H_

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "stdlib.h"

/*******************************************
 * Const and Macro Defines
 *******************************************/
//none

/***********************************
 * Typedefs and Enum Declarations
 ***********************************/
typedef struct config_ascii{
	uint8_t data_id;
	uint8_t color_code;
	uint8_t refresh_rate[2];
	uint8_t start_color[3];
	uint8_t end_color[3];
	uint8_t step_size;
	uint8_t mode;
	uint8_t change_rate[3];
	uint8_t cycles[2];
	uint8_t direction;
}config_ascii_t;

typedef struct config_hex{
	uint8_t data_id;
	uint8_t color_code;
	uint8_t refresh_rate;
	uint8_t start_color[3];
	uint8_t end_color[3];
	uint8_t step_size;//1-7
	uint8_t mode;//1,2
	uint16_t change_rate;//1ms-999ms
	uint8_t cycles;
	uint8_t direction;
}config_hex_t;

typedef struct command_ascii{
	uint8_t data_id;
	uint8_t command;
}command_ascii_t;

typedef struct status_hex{
uint8_t current_color[3];
uint8_t cycles_left;
}status_hex_t;

typedef struct status_ascii{
uint8_t current_color[3];
uint8_t cycles_left[2];
}status_ascii_t;

/***********************************
 * External Variable Declarations
 ***********************************/
// none

/***********************************
 * Const Declarations
 ***********************************/
// none

/***********************************
 * Public Variables
 ***********************************/
//none
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
 * @note  This function will convert the ascii data into hex data and store it into config_hex_t type struct and returns it.
 *
 * Revision History:
 * 	- 140121 MAP: 	Creation Date
 */

config_hex_t ascii_to_hex_converter(config_ascii_t * xconfig_ascii);
status_ascii_t hex_to_ascii_converter(status_hex_t * xstatus_hex);

BaseType_t send_config_task(config_ascii_t* xconfig_ascii);
BaseType_t receive_config_task_blocking(config_ascii_t* xconfig_ascii);
BaseType_t send_command_to_task(command_ascii_t* xcommand_ascii);
BaseType_t receive_command_from_task(command_ascii_t* xcommand_ascii);
BaseType_t receive_status_from_task(uint8_t* current_status);
BaseType_t send_status_to_task(status_ascii_t* xstatus_ascii);


#endif /* AURORA_PG_DATA_MANAGER_H_ */
