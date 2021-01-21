/*
 * datamanager.h
 *
 *  Created on: 11-Jan-2021
 *      Author: Daniya
 */

#ifndef DATAMANAGER_H_
#define DATAMANAGER_H_
#include"aurora_pg_data_manager.h"
/*******************************************
 * Const and Macro Defines
 *******************************************/
//none

/***********************************
 * Typedefs and Enum Declarations
 ***********************************/

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

/**
 * @brief saves the default configuration to the structure.
 *
 * Function loads a default configuration to the
 * each element of the  structue
 *
 * @param 	 config*  pointer to the configuration structure.
 * @return 	none.
 *
 * @note
 *
 * Revision History:
 * 	- 150121 DA: 	Creation Date
 */
void get_default_config(config_ascii_t*);
/**
 * @brief saves new configuration to the structure.
 *
 * Function update the configuration structure with
 * new data. Each element of the  structure can be
 * updated differently.
 *
 * @param 	 parameter id - id to represent the each
 *                          element of structure.
 *           data - new value to write to the structure.
 * @return 	success/failure.
 *
 * @note
 *
 * Revision History:
 * 	- 150121 DA: 	Creation Date
 */
int update_config_struct(uint8_t ,uint8_t*);
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
int update_cmd_struct(uint8_t);
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
BaseType_t send_config_pg();
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
BaseType_t send_cmd_pg();
/**
 * @brief receive the status from other task
 *
 * Function gets the values from the status queue
 * and stores it to the  array and returns
 * the same array.
 *
 * @param 	none
 * @return 	array
 *
 * @note
 *
 * Revision History:
 * 	- 190121 DA: 	Creation Date
 */
uint8_t *receive_status_pg();
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
int reverse_direction();


#endif /* DATAMANAGER_H_ */
