/*
 * pattern_generator.c
 *
 *  Created on: 13-Jan-2021
 *      Author: dct
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


/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include "aurora_pat_gen.h"
#include"aurora_pit.h"
#include "aurora_pg_data_manager.h"

/*******************************************
 * Const and Macro Defines
 *******************************************/
//none
/***********************************
 * Typedefs and Enum Declarations
 ***********************************/
//none

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
 * @brief void pattern_generator(int8_t *current_color)
 *
 *
 *
 * @param  int8_t *current_color It will contain the current color configuration by which the pattern is to be generated.
 *
 * @return void
 *
 * @note  This function will generate led pattern based on the current color received.
 *
 * Revision History:
 * 	- 130121 MAP: 	Creation Date
 */

void pattern_generator(int8_t *current_color,config_hex_t* xconfig_hex){
	static int count = 0;



	if(count >= current_color[0]){
		LED_RED_OFF();
	}
	else{
		LED_RED_ON();
	}

	if(count >= current_color[1]){
		LED_GREEN_OFF();
	}
	else{
		LED_GREEN_ON();
	}
	if(count >= current_color[2]){
		LED_BLUE_OFF();
	}
	else{
		LED_BLUE_ON();
	}


	count++;
	if(count > (pow(2,xconfig_hex->color_code)-1)){
		count = 0;
	}

}

/**
 * @brief update_current_color_up
 *
 *
 *
 * @param  config_hex_t xconfig It will contain the configuration including start color,End color,step size from which .
 *
 * @return void
 *
 * @note  This function will generate led pattern based on the current color received.
 *
 * Revision History:
 * 	- 140121 Nikhil: 	Creation Date
 */

int8_t update_current_color_up(config_hex_t* xconfig,int8_t* current_colorinitial)
{
	uint8_t stepsize;
	uint8_t start_color[3];
	uint8_t end_color[3];
	static int8_t current_color[3];

	stepsize=xconfig->step_size;

	memcpy(end_color,xconfig->end_color,sizeof(end_color));
	memcpy(start_color,xconfig->start_color,sizeof(start_color));
	memcpy(current_color,current_colorinitial,sizeof(current_color));


	if(current_color[2]==end_color[2]){
		if(current_color[1]==end_color[1]){
			if(current_color[0]==end_color[0]){
				current_color[2]=start_color[2];
				current_color[1]=start_color[1];
				current_color[0]=start_color[0];

				memcpy(current_colorinitial,current_color,sizeof(current_color));
				return 0;

			}
			else{
				current_color[2]=start_color[2];
				current_color[1]=start_color[1];
				current_color[0]=current_color[0]+stepsize;
				if(current_color[0] < end_color[0]){

					memcpy(current_colorinitial,current_color,sizeof(current_color));
					return 0;
				}
				else{
					current_color[0]=end_color[0];

					memcpy(current_colorinitial,current_color,sizeof(current_color));
					return 0;
				}
			}

		}
		else{
			current_color[2]=start_color[2];
			current_color[1]=current_color[1]+stepsize;
			if(current_color[1]<end_color[1]){

				memcpy(current_colorinitial,current_color,sizeof(current_color));
				return 0;
			}
			else{
				current_color[1]=end_color[1];

				memcpy(current_colorinitial,current_color,sizeof(current_color));
				return 0;
			}
		}

	}
	else{
		current_color[2]=current_color[2]+stepsize;
		if(current_color[2]<end_color[2]){

			memcpy(current_colorinitial,current_color,sizeof(current_color));
			return 0;
		}
		else{
			current_color[2]=end_color[2];

			memcpy(current_colorinitial,current_color,sizeof(current_color));
			return 0;
		}

	}


}

/**
 * @brief update_current_color_down
 *
 *
 *
 * @param  config_hex_t xconfig It will contain the configuration including start color,End color,step size from which .
 *
 * @return void
 *
 * @note  This function will generate led pattern based on the current color received in down direction
 *
 * Revision History:
 * 	- 140121 Nikhil: 	Creation Date
 */

int8_t update_current_color_down(config_hex_t* xconfig,int8_t* current_colorinitial)
{
	uint8_t stepsize;
	uint8_t start_color[3];
	uint8_t end_color[3];
	static int8_t current_color[3];

	stepsize=xconfig->step_size;

	memcpy(end_color,xconfig->end_color,sizeof(end_color));
	memcpy(start_color,xconfig->start_color,sizeof(start_color));
	memcpy(current_color,current_colorinitial,sizeof(current_color));


	if(current_color[2]==start_color[2]){
		if(current_color[1]==start_color[1]){
			if(current_color[0]==start_color[0]){
				current_color[2]=end_color[2];
				current_color[1]=end_color[1];
				current_color[0]=end_color[0];

				memcpy(current_colorinitial,current_color,sizeof(current_color));
				return 0;

			}
			else{
				current_color[2]=end_color[2];
				current_color[1]=end_color[1];
				current_color[0]=current_color[0]-stepsize;
				if(current_color[0] > start_color[0]){

					memcpy(current_colorinitial,current_color,sizeof(current_color));
					return 0;
				}
				else{
					current_color[0]=start_color[0];

					memcpy(current_colorinitial,current_color,sizeof(current_color));
					return 0;
				}
			}

		}
		else{
			current_color[2]=end_color[2];
			current_color[1]=current_color[1]-stepsize;
			if(current_color[1]>start_color[1]){

				memcpy(current_colorinitial,current_color,sizeof(current_color));
				return 0;
			}
			else{
				current_color[1]=start_color[1];

				memcpy(current_colorinitial,current_color,sizeof(current_color));
				return 0;
			}
		}

	}
	else{
		current_color[2]=current_color[2]-stepsize;
		if(current_color[2]>start_color[2]){

			memcpy(current_colorinitial,current_color,sizeof(current_color));
			return 0;
		}
		else{
			current_color[2]=start_color[2];

			memcpy(current_colorinitial,current_color,sizeof(current_color));
			return 0;
		}

	}


}

/* @brief manual_pattern
 *
 *
 *
 * @param  config_hex_t xconfig It will contain the configuration including start color,End color,step size from which .
 *
 * @param command_ascii
 *
 *  @param current_color
 *
 *
 * @return void
 *
 * @note  This function will generate led pattern based on the command configuration.
 *
 * Revision History:
 * 	- 140121 Nikhil: 	Creation Date
 */

void manual_pattern(config_hex_t* xconfig_hex, command_ascii_t* xcommand_ascii){
	int8_t current_color[3]={0,0,0};
	status_ascii_t xstatus_ascii;
	status_hex_t xstatus_hex;
	xstatus_hex.cycles_left = MODE_INDICATOR;
	memcpy(current_color,xconfig_hex->start_color,sizeof(current_color));
	while(xcommand_ascii->command!='Q')
	{

		ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
		pattern_generator(current_color,xconfig_hex);
		memcpy(xstatus_hex.current_color,current_color,sizeof(current_color));


		switch(xcommand_ascii->command){
		case '>':
			update_current_color_up(xconfig_hex,current_color);
			xcommand_ascii->command = DEFAULT_COMMAND;
			break;

		case '<':
			update_current_color_down(xconfig_hex,current_color);
			xcommand_ascii->command = DEFAULT_COMMAND;
			break;
		}
		receive_command_from_task (xcommand_ascii);
		xstatus_ascii = hex_to_ascii_converter(&xstatus_hex);
		send_status_to_task (&xstatus_ascii);
	}
	pattern_log_off();

	xTaskNotifyGive(config_task_handle);
}
/* @brief auto_pattern
 *
 *
 *
 * @param  config_hex_t xconfig It will contain the configuration including start color,End color,step size from which .
 *
 * @param command_ascii_t xcommand_ascii It will contain the commands from user
 *
 *  @param current_color
 *
 *
 * @return void
 *
 * @note  This function will generate led pattern based on the command configuration.
 *
 * Revision History:
 * 	- 140121 Nikhil: 	Creation Date
 */

void auto_pattern(config_hex_t* xconfig_hex, command_ascii_t* xcommand_ascii)
{



	switch(xconfig_hex->direction){
	case 1:
		up_direction(xconfig_hex,xcommand_ascii);
		break;

	case 2://down

		down_direction(xconfig_hex,xcommand_ascii);
		break;
	case 3://up-down
		up_down_direction(xconfig_hex,xcommand_ascii);

		break;
	case 4://down-up
		down_up_direction(xconfig_hex,xcommand_ascii);

		break;
	}
}

/* @brief auto_pattern
 *
 *
 *
 * @param  config_hex_t xconfig It will contain the configuration including start color,End color,step size from which .
 *
 * @param command_ascii_t xcommand_ascii It will contain the commands from user
 *
 *  @param current_color
 *
 *
 * @return void
 *
 * @note  This function will generate led pattern based on the command configuration.
 *
 * Revision History:
 * 	- 140121 Nikhil: 	Creation Date
 *
 */



void up_direction(config_hex_t* xconfig_hex, command_ascii_t* xcommand_ascii)
{
	status_ascii_t xstatus_ascii;
	status_hex_t xstatus_hex;
	int8_t current_color[3] = {0,0,0};
	uint8_t compare = DEFAULT_VALUE;
	uint8_t num_cycles_temp = DEFAULT_VALUE;
	xstatus_hex.cycles_left = MODE_INDICATOR;

	/*only change the num_cycles_temp for automatic fixed.*/

	if(xconfig_hex->cycles){
		num_cycles_temp = xconfig_hex->cycles;
		xstatus_hex.cycles_left = xconfig_hex->cycles;
	}

	/*copies the start color tocurrent color*/
	memcpy(current_color,xconfig_hex->start_color,sizeof(current_color));


	while(num_cycles_temp !=0)
	{

		//		if(xcommand_ascii->command == 'Q'){
		//			break;
		//		}
		while(!change_rate_delay(xconfig_hex,xcommand_ascii)){


			receive_command_from_task(xcommand_ascii);

			/*Waits for notification from pit isr*/
			ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
			pattern_generator(current_color,xconfig_hex);

			/*compares the end color and current color to determine whether end of cycle is occured*/
			compare = memcmp(xconfig_hex->end_color,current_color,sizeof(current_color));

			/*num_Cycles_temp is decremented only if number of cycles from config is greater than 0*/
			if(compare == 0 && (xconfig_hex->cycles != 0) ){
				num_cycles_temp -= 1;
				xstatus_hex.cycles_left = num_cycles_temp;
				break;
			}

			if(xcommand_ascii->command == 'Q'){
				pattern_log_off();

				xTaskNotifyGive(config_task_handle);
				break;
			}

		}

		update_current_color_up(xconfig_hex,current_color);
		/*Send color status*/
		memcpy(xstatus_hex.current_color,current_color,sizeof(current_color));
		xstatus_ascii =hex_to_ascii_converter(&xstatus_hex);
		send_status_to_task(&xstatus_ascii);

		if(xcommand_ascii->command == 'Q'){
			pattern_log_off();
			xTaskNotifyGive(config_task_handle);
			break;
		}
	}



	while(xcommand_ascii->command != 'Q'){
		receive_command_from_task(xcommand_ascii);
		pattern_generator(current_color,xconfig_hex);
	}

	pattern_log_off();

	xTaskNotifyGive(config_task_handle);

}



/* @brief down_direction
 *
 *
 *
 * @param  config_hex_t xconfig It will contain the configuration including start color,End color,step size from which .
 *
 * @param command_ascii_t xcommand_ascii It will contain the commands from user
 *
 *  @param current_color
 *
 *
 * @return void
 *
 * @note  This function will generate led pattern based on the command configuration.
 *
 * Revision History:
 * 	- 140121 Nikhil: 	Creation Date
 */


void down_direction(config_hex_t* xconfig_hex, command_ascii_t* xcommand_ascii)
{
	status_ascii_t xstatus_ascii;
	status_hex_t xstatus_hex;

	int8_t current_color[3] = {0,0,0};
	uint8_t compare = DEFAULT_VALUE;
	uint8_t num_cycles_temp = DEFAULT_VALUE;

	xstatus_hex.cycles_left = MODE_INDICATOR;

	if(xconfig_hex->cycles){
		num_cycles_temp = xconfig_hex->cycles; // only change the num_cycles_temp for automatic fixed.
		xstatus_hex.cycles_left = xconfig_hex->cycles;
	}

	memcpy(current_color,xconfig_hex->end_color,sizeof(current_color));


	while(num_cycles_temp !=0){
		if(xcommand_ascii->command == 'Q'){
			break;
		}
		update_current_color_down(xconfig_hex,current_color);
		while(!change_rate_delay(xconfig_hex,xcommand_ascii)){
			receive_command_from_task(xcommand_ascii);

			pattern_generator(current_color,xconfig_hex);
			ulTaskNotifyTake( pdTRUE, portMAX_DELAY );

			memcpy(xstatus_hex.current_color,current_color,sizeof(current_color));

			compare = memcmp(xconfig_hex->start_color,current_color,sizeof(current_color));

			if(compare == 0 && (xconfig_hex->cycles != 0) ){
				//num_Cycles_temp is decremented only if number of cycles from config is greater than 0
				num_cycles_temp -= 1 ;
				xstatus_hex.cycles_left = num_cycles_temp;

				break;
			}
			if(xcommand_ascii->command == 'Q'){
				pattern_log_off();

				xTaskNotifyGive(config_task_handle);
				break;
			}
		}


		xstatus_ascii =hex_to_ascii_converter(&xstatus_hex);
		send_status_to_task(&xstatus_ascii);
		if(xcommand_ascii->command == 'Q'){
			pattern_log_off();

			break;
		}

	}

	while(xcommand_ascii->command != 'Q'){
		receive_command_from_task(xcommand_ascii);
		pattern_generator(current_color,xconfig_hex);
	}

	pattern_log_off();

	xTaskNotifyGive(config_task_handle);

}





/* @brief up_down_direction
 *
 *
 *
 * @param  config_hex_t xconfig It will contain the configuration including start color,End color,step size from which .
 *
 * @param command_ascii_t xcommand_ascii It will contain the commands from user
 *
 *
 *
 *
 * @return void
 *
 * @note  This function will generate led pattern based on the command configuration.
 *
 * Revision History:
 * 	- 190121 Nikhil: 	Creation Date
 */

void up_down_direction(config_hex_t* xconfig_hex, command_ascii_t* xcommand_ascii)
{
	status_ascii_t xstatus_ascii;
	static status_hex_t xstatus_hex;

	int8_t current_color[3]={0,0,0};
	uint8_t compare = DEFAULT_VALUE;
	uint8_t num_cycles_temp = DEFAULT_VALUE;
	bool direction= true;

	xstatus_hex.cycles_left = MODE_INDICATOR;

	if(xconfig_hex->cycles){
		num_cycles_temp = 2*xconfig_hex->cycles;// only change the num_cycles_temp for automatic fixed.
		xstatus_hex.cycles_left = xconfig_hex->cycles;
	}

	memcpy(current_color,xconfig_hex->start_color,sizeof(current_color));


	while(num_cycles_temp !=0){

		if(xcommand_ascii->command == 'Q'){
			break;
		}

		if(direction){
			update_current_color_up(xconfig_hex,current_color);
		}
		else if (!direction){
			update_current_color_down(xconfig_hex,current_color);
		}

		while(!change_rate_delay(xconfig_hex,xcommand_ascii)){

			receive_command_from_task(xcommand_ascii);
			pattern_generator(current_color,xconfig_hex);
			ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
			memcpy(xstatus_hex.current_color,current_color,sizeof(current_color));

			if(direction){
				compare = memcmp(xconfig_hex->end_color,current_color,sizeof(current_color));
			}
			else if (!direction){
				compare = memcmp(xconfig_hex->start_color,current_color,sizeof(current_color));
			}


			if(compare == 0  ){ //num_Cycles_temp is decremented only if number of cycles from config is greater than 0
				direction = !direction;
				if(xconfig_hex->cycles != 0){
					num_cycles_temp -=1;


				}
				if(!(num_cycles_temp %2)){

					xstatus_hex.cycles_left--;
				}
				break;
			}
			if(xcommand_ascii->command == 'Q'){
				pattern_log_off();

				xTaskNotifyGive(config_task_handle);
				break;
			}
		}


		xstatus_ascii =hex_to_ascii_converter(&xstatus_hex);
		send_status_to_task(&xstatus_ascii);
		if(xcommand_ascii->command == 'Q'){
			pattern_log_off();
			stop_pit_timer(PIT, kPIT_Chnl_0);
			break;
		}
	}

	while(xcommand_ascii->command != 'Q'){
		receive_command_from_task(xcommand_ascii);
		pattern_generator(current_color,xconfig_hex);
	}

	pattern_log_off();

	xTaskNotifyGive(config_task_handle);

}




/* @brief down_up_direction
 *
 *
 *
 * @param  config_hex_t xconfig It will contain the configuration including start color,End color,step size from which .
 *
 * @param command_ascii_t xcommand_ascii It will contain the commands from user
 *
 *
 *
 *
 * @return void
 *
 * @note  This function will generate led pattern based on the command configuration.
 *
 * Revision History:
 * 	- 190121 Nikhil: 	Creation Date
 */

void down_up_direction(config_hex_t* xconfig_hex, command_ascii_t* xcommand_ascii)
{
	status_ascii_t xstatus_ascii;
	static status_hex_t xstatus_hex;

	int8_t current_color[3]={0,0,0};
	uint8_t compare = DEFAULT_VALUE;
	uint8_t num_cycles_temp = DEFAULT_VALUE;
	bool direction= false;

	xstatus_hex.cycles_left = MODE_INDICATOR;//incase of continuous cycles

	if(xconfig_hex->cycles){
		num_cycles_temp = 2*xconfig_hex->cycles; // only change the num_cycles_temp for automatic fixed.
		xstatus_hex.cycles_left = xconfig_hex->cycles;
	}

	memcpy(current_color,xconfig_hex->end_color,sizeof(current_color));


	while(num_cycles_temp !=0){

		if(xcommand_ascii->command == 'Q'){
			break;
		}
		if(direction){
			update_current_color_up(xconfig_hex,current_color);
		}
		else if (!direction){
			update_current_color_down(xconfig_hex,current_color);
		}

		while(!change_rate_delay(xconfig_hex,xcommand_ascii)){

			receive_command_from_task(xcommand_ascii);
			pattern_generator(current_color,xconfig_hex);
			ulTaskNotifyTake( pdTRUE, portMAX_DELAY );

			memcpy(xstatus_hex.current_color,current_color,sizeof(current_color));

			if(direction){
				compare = memcmp(xconfig_hex->end_color,current_color,sizeof(current_color));
			}
			else if (!direction){
				compare = memcmp(xconfig_hex->start_color,current_color,sizeof(current_color));
			}


			if(compare == 0  ){ //num_Cycles_temp is decremented only if number of cycles from config is greater than 0
				direction = !direction;
				if(xconfig_hex->cycles != 0){
					num_cycles_temp -=1;
				}
				if(!(num_cycles_temp %2)){

					xstatus_hex.cycles_left--;
				}
				break;
			}
			if(xcommand_ascii->command == 'Q'){
				pattern_log_off();

				xTaskNotifyGive(config_task_handle);
				break;
			}
		}

		xstatus_ascii =hex_to_ascii_converter(&xstatus_hex);
		send_status_to_task(&xstatus_ascii);
		if(xcommand_ascii->command == 'Q'){
			pattern_log_off();

			break;
		}
	}

	while(xcommand_ascii->command != 'Q'){
		receive_command_from_task(xcommand_ascii);
		pattern_generator(current_color,xconfig_hex);
	}

	pattern_log_off();
	xTaskNotifyGive(config_task_handle);

}

/* @brief switch_led_off()
 *
 *

 *
 *
 *
 *
 * @return void
 *
 * @note  This function will switch off all the LEDs
 *
 * Revision History:
 * 	- 200121 MAP: 	Creation Date
 */

void pattern_log_off(){
	LED_RED_OFF();
	LED_GREEN_OFF();
	LED_BLUE_OFF();
	stop_pit_timer(PIT, kPIT_Chnl_0);

}


