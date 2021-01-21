/**
 * @file	pattern_manager_task.c
 * @brief It contains the pattern generation task.
 *
 *
 *
 * @note
 *
 * Revision History:
 *		- 180121 MAP : Creation Date
 */

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
#include "fsl_uart_freertos.h"
#include "fsl_uart.h"
#include "fsl_pit.h"

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/*aurora header files*/
#include "aurora_pg_data_manager.h"
#include "aurora_pit.h"
#include "aurora_pat_gen.h"
/*******"************************************
 * Const and Macro Defines
 *******************************************/
//none

/***********************************
 * Typedefs and Enum Declarations
 ***********************************/
// none

/***********************************
 * External Variable Declarations
 ***********************************/
extern TaskHandle_t config_task_handle;

/***********************************
 * Const Declarations
 ***********************************/
// none

/***********************************
 * Public Variables
 ***********************************/
extern QueueHandle_t cc_queue;

/***********************************
 * Private Variables
 ***********************************/
//none

/***********************************
 * Private Prototypes
 ***********************************/
// none

/***********************************
 * Public Functions
 ***********************************/

/**
 * @brief pattern generation task
 *
 *
 *
 *
 *
 * @param
 * @return 	void
 *
 * @note  calls all the function related to pattern generation.
 *
 * Revision History:
 * 	- 180121 MAP: 	Creation Date
 */

void pattern_generation_task(void *pvParameters){

	/*struct parameters*/
	static config_hex_t xconfig_hex;
	static config_ascii_t xconfig_ascii;
	static command_ascii_t xcommand_ascii;

	/*Pit initialisations*/

	pit_init();
	enable_pit_interrupt();

	while(1){

		while(receive_config_task_blocking(&xconfig_ascii)){  //receive configuration from the configuration manager task


			xconfig_hex = ascii_to_hex_converter(&xconfig_ascii);

			xTaskNotifyGive(config_task_handle);

			receive_command_from_task(&xcommand_ascii);//receive command from the configuration manager task

			if(xcommand_ascii.data_id == '1' && xcommand_ascii.command == 'X' ){

				break;

			}
			else{

				continue;
			}

		}

		set_pit_interval(&xconfig_hex);

		start_pit_timer(PIT, kPIT_Chnl_0);

		switch(xconfig_hex.mode){
		case 1 ://manual mode

			manual_pattern(&xconfig_hex, &xcommand_ascii);

			break;

		case 2://auto mode

			auto_pattern(&xconfig_hex,&xcommand_ascii);
			break;

		default :
			PRINTF("UNKNOWN MODE");//TO BE DELETED FOR TESTING PURPOSES ONLY
			break;
		}
	}
	PRINTF("YOU'RE oUT");//to delete
}
