/**
 * @file	statemachine.c
 * @brief	Functions that controls the statemachine.
 *
 *
 * @note
 *
 * Revision History:
 *		- 110121 MG : Creation Date
 */


#include"statemachine.h"


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
extern config_ascii_t curr_config;
extern uint8_t state;
extern uint8_t is_master;
extern command_ascii_t cmd_ascii;

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
uint8_t companion_status =SLAVE_NOT_CONNECTED;

/***********************************
 * Private Prototypes
 ***********************************/
void display_config();
int systemconfig_func();
int color_config_func();
int mode_config();
int direction_config();
int homescreen_func();
int execution_func();
int patternconfig_func();

/***********************************
 * Public Functions
 ***********************************/
/**
 * @brief Configuration task
 *
 * Manages statemachine,
 * switches the screens according to states
 *
 * @param 	*pvParameters.
 * @return 	none
 *
 * @note
 *
 * Revision History:
 * 	- 120121 MG: 	Creation Date
 */

void config_task(void *pvParameters)
{
	if(is_master){
		enable_uart_isr();
		get_default_config(&curr_config);
	}
	else
		uart_slave_enable();

	state=HOMESCREEN;
	while (1)
	{
		switch (state)
		{
		case HOMESCREEN:
			state = homescreen_func();
			print_message("\x1b[2J");
			break;

		case SYSTEMCONFIG_SCREEN:
			state = systemconfig_func();
			print_message("\x1b[2J");
			break;

		case PATTERNCONFIG_SCREEN:
			state = patternconfig_func();
			print_message("\x1b[2J");
			break;

		case EXECUTION_SCREEN:
			state = execution_func();
			print_message("\x1b[2J");
			break;

		default:
			print_message("Invalid state");
			break;
		}
	}
}


/**
 * @brief Display configuration
 *
 * Displays the configuration
 * from the structure.
 *
 * @param 	none.
 * @return 	none
 *
 * @note
 *
 * Revision History:
 * 	- 120121 MG: 	Creation Date
 */

void display_config(){

	print_data((uint8_t*)CURR_CONFIG,sizeof(CURR_CONFIG));
	print_message(REFRESH_RATE);
	print_data(curr_config.refresh_rate,sizeof(curr_config.refresh_rate));
	print_message(COLOUR_CODING);

	if ((curr_config.color_code == '3')){
		print_message("333RGB");
	}

	print_message(START_COLOUR);
	print_data(curr_config.start_color,sizeof(curr_config.start_color));
	print_message(END_COLOUR);
	print_data(curr_config.end_color,sizeof(curr_config.end_color));
	print_message(STEP_SIZE);
	print_data(&curr_config.step_size,sizeof(curr_config.step_size));
	print_message(MODE);
	if (curr_config.mode == '1') {
		print_message("Manual");
	} else if (curr_config.mode == '2') {
		print_message("Automatic");
		print_message(CYCLE);
		print_data(curr_config.cycles,sizeof(curr_config.cycles));
		print_message(DIRECTION);
		if (curr_config.direction=='1') {
			print_message("Up");
		} else if (curr_config.direction=='2') {
			print_message("Down");
		} else if (curr_config.direction=='3') {
			print_message("Up-Down");
		} else if (curr_config.direction=='4') {
			print_message("Down-Up");
		}
		print_message(CHANGE_RATE);
		print_data(curr_config.change_rate,sizeof(curr_config.change_rate));
	}

}
/**
 * @brief System configuration function
 *
 * Displays the system configuration screen,validates
 * refresh rate, and stores the valid refresh rate into structure.
 *
 * @param 	none.
 * @return 	HOMESCREEN
 *
 * @note
 *
 * Revision History:
 * 	- 120121 MG: 	Creation Date
 */
int systemconfig_func()
{
	print_data((uint8_t*)SYS_CONFIG,sizeof(SYS_CONFIG));
	print_message("\tCurrent refresh rate\t:\t");
	print_data(curr_config.refresh_rate,sizeof(curr_config.refresh_rate));
	print_message("\n\r");
	uint8_t data,i;
	uint8_t buff[MAX_SIZE_REFRESH];
	memset(&buff[0],'\0', sizeof(buff));
	char get1[] = "\r\nEnter refresh rate";


	while(1) {
		memset(&buff[0],'\0', sizeof(buff));
		print_message(get1);
		for(i=0;i<3;)
		{
			data=get_keyinput();
			data = toupper(data);
			if (data == 'Q') {
				return HOMESCREEN;
			}
			if(data=='\r')
				break;
			*(buff+i)=data;
			i++;
		}
		if(validate_buffer(REFRESH_RATE_ID,buff)==kStatus_Fail)
		{
			print_message(ERROR_MSG);
			continue;
		}
		break;
	}
	update_config_struct(REFRESH_RATE_ID,buff);

	return HOMESCREEN;

}
/**
 * @brief Colour configuration function
 *
 * Displays the colour configuration screen,validates start colour,end colour
 * and step size, and stores the valid entries into structure.
 *
 * @param 	none.
 * @return 	PATTERNCONFIG_SCREEN
 *
 * @note
 *
 * Revision History:
 * 	- 120121 MG: 	Creation Date
 */
int color_config_func()
{
	print_data((uint8_t*)COLOUR_CONFIG,sizeof(COLOUR_CONFIG));
	print_message("\n\rCurrent colour config\t:\n\r");
	print_message("\n\rStart Colour : ");
	for(int i=0;;i++) {
		print_data(&curr_config.start_color[i],1);
		if (i>=sizeof(curr_config.start_color)-1)
			break;
		print_message(",");
	}
	print_message("\tEnd Colour : ");
	for(int i=0;;i++) {
		print_data(&curr_config.end_color[i],1);
		if (i>=sizeof(curr_config.end_color)-1)
			break;
		print_message(",");
	}
	print_message("\tStep Size : ");
	print_data(&curr_config.step_size,sizeof(curr_config.step_size));
	print_message(QUIT);
	print_message(BACK);
	uint8_t data,i;
	uint8_t buff[6],buff1[6],buff2[2];
	char get[] = "\n\rEnter the start color (R,G,B)\t->\t ";
	print_message(get);
	for(i=0;i<6;)
	{
		data=get_keyinput();
		data = toupper(data);
		if(data=='Q'){
			return HOMESCREEN;
		}else if(data=='B'){
			return PATTERNCONFIG_SCREEN;
		}else if(validate_byte(START_COLOR_ID,i,data)==kStatus_Fail){
			print_message(ERROR_MSG);
			i=0;
			print_message(get);
		}else{
			*(buff+i)=data;
			i++;
		}
	}

	char get2[] = "\r\nEnter the end color   (R,G,B)\t->\t ";
	print_message(get2);
	for(i=0;i<6;)
	{
		data=get_keyinput();
		data = toupper(data);
		if(data=='Q'){
			return HOMESCREEN;
		}else if(data=='B'){
			return PATTERNCONFIG_SCREEN;
		}else if(validate_byte(END_COLOR_ID,i,data)==kStatus_Fail){
			print_message(ERROR_MSG);
			i=0;
			print_message(get2);
		}else{
			*(buff1+i)=data;
			i++;
		}

		if(i==6&&compare(buff,buff1)==kStatus_Fail){
			print_message(ERROR_MSG);
			print_message("End color should be greater\n\rEnter the end color again ->\t\t");
			i=0;
		}
	}
	char get3[] = "\n\rEnter the step size\t\t->\t ";
	print_message(get3);
	for(i=0;i<2;)
	{
		data=get_keyinput();
		data = toupper(data);
		if(data=='Q'){
			return HOMESCREEN;
		}else if(data=='B'){
			return PATTERNCONFIG_SCREEN;
		}else if(validate_byte(STEP_SIZE_ID,i,data)==kStatus_Fail){
			print_message(ERROR_MSG);
			i=0;
			print_message(get3);
		}
		else{
			*(buff2+i)=data;
			i++;
		}
	}
	update_config_struct(START_COLOR_ID,buff);
	update_config_struct(END_COLOR_ID,buff1);
	update_config_struct(STEP_SIZE_ID,buff2);

	return PATTERNCONFIG_SCREEN;

}
/**
 * @brief Mode configuration function
 *
 * Displays the mode configuration screen,validates mode, change rate
 * and number of cycles, and stores the valid entries into structure.
 *
 * @param 	none.
 * @return 	PATTERNCONFIG_SCREEN
 *
 * @note
 *
 * Revision History:
 * 	- 120121 MG: 	Creation Date
 */
int mode_config()
{
	uint8_t buff1[2], buff2[3], buff3[4];
	uint8_t data,i;
	print_data((uint8_t*)MODE_CONFIG,sizeof(MODE_CONFIG));
	print_message("\n\rCurrent Mode Setting\t:\n\r");
	print_message("\n\r\tMode\t\t\t:\t");
	if (curr_config.mode == '1') {
		print_message("Manual");
	} else if (curr_config.mode == '2') {
		print_message("Automatic");
		print_message("\n\r\tCycle\t\t\t:\t");
		print_data(curr_config.cycles,sizeof(curr_config.cycles));
		print_message("\n\r\tChange Rate (in 10 ms)\t:\t");
		print_data(curr_config.change_rate,sizeof(curr_config.change_rate));
	}
	print_message(QUIT);
	print_message(BACK);
	print_message(PROMPT_MS);
	char get1[] = "\n\rChoose a Preferred Mode\t\t->\t";
	print_message(get1);
	for(i=0;i<2;)
	{
		data=get_keyinput();
		data = toupper(data);
		if (data == 'Q'){
			return HOMESCREEN;
		}else if (data == 'B') {
			return PATTERNCONFIG_SCREEN;
		} else {
			if(validate_byte(MODE_ID,i,data)==kStatus_Fail) {
				print_message(ERROR_MSG);
				i=0;
				print_message(get1);
			} else {
				*(buff1+i)=data;
				i++;
			}
		}
	}
	if (*buff1 == '2')
	{
		char get3[] = "\n\rEnter Change Rate (1-999)\t->\t";
		while(1)
		{
			memset(&buff3[0],'\0', sizeof(buff3));
			print_message(get3);
			for(i=0;i<4;)
			{
				data=get_keyinput();
				data = toupper(data);
				if (data == 'Q') {
					return HOMESCREEN;
				}else if (data == 'B') {
					return PATTERNCONFIG_SCREEN;
				}
				if(data=='\r')
					break;
				*(buff3+i)=data;
				i++;
			}
			if(validate_buffer(CHANGE_RATE_ID,buff3)==kStatus_Fail){
				print_message(ERROR_MSG);
				continue;
			}
			break;
		}
		char get2[] = "\n\rEnter number of cycles (0-99)\t->\t";
		while(1)
		{
			memset(&buff2[0],'\0', sizeof(buff2));
			print_message(get2);
			for(i=0;i<4;)
			{
				data=get_keyinput();
				data = toupper(data);
				if (data == 'Q') {
					return HOMESCREEN;
				} else if (data == 'B') {
					return PATTERNCONFIG_SCREEN;
				}
				if(data=='\r')
					break;
				*(buff2+i)=data;
				i++;
			}
			if(validate_buffer(CYCLES_ID,buff2)==kStatus_Fail)
			{
				print_message(ERROR_MSG);
				continue;
			}
			break;
		}

		update_config_struct(CHANGE_RATE_ID,buff3);
		update_config_struct(CYCLES_ID,buff2);
	}
	update_config_struct(MODE_ID,buff1);
	return PATTERNCONFIG_SCREEN;
}
/**
 * @brief Direction configuration function
 *
 * Displays the direction configuration screen,validates
 * direction, and stores the valid entriy into structure.
 *
 * @param 	none.
 * @return 	PATTERNCONFIG_SCREEN
 *
 * @note
 *
 * Revision History:
 * 	- 120121 MG: 	Creation Date
 */
int direction_config()
{
	uint8_t buff1[2],data,i;
	print_data((uint8_t*)DIR_CONFIG,sizeof(DIR_CONFIG));
	print_message("\n\rCurrent Direction Setting\t:\n\r");
	print_message("\n\r\tDirection\t\t: ");
	if (curr_config.direction=='1') {
		print_message("Up");
	} else if (curr_config.direction=='2') {
		print_message("Down");
	} else if (curr_config.direction=='3') {
		print_message("Up-Down");
	}
	print_message(QUIT);
	print_message(BACK);
	print_message(PROMPT_DS);
	char get1[] = "\n\rEnter your preferred direction\t->\t";
	print_message(get1);
	for(i=0;i<2;)
	{
		data=get_keyinput();
		data = toupper(data);
		if (data == 'Q'){
			return HOMESCREEN;
		} else if (data == 'B') {
			return PATTERNCONFIG_SCREEN;
		} else {
			if(validate_byte(DIRECTION_ID,i,data)==kStatus_Fail) {
				print_message(ERROR_MSG);
				i=0;
				print_message(get1);
			} else {
				*(buff1+i)=data;
				i++;
			}
		}
	}
	update_config_struct(DIRECTION_ID,buff1);
	return PATTERNCONFIG_SCREEN;
}

/**
 * @brief Homescreen function
 *
 * Displays the home screen on master and slave devices, validates
 * user input, and switches screen according to user input.
 *
 * @param 	none.
 * @return 	state
 *
 * @note
 *
 * Revision History:
 * 	- 120121 MG: 	Creation Date
 */
int homescreen_func(){


	if (is_master==1) {
		uint8_t device_status[] = "Master\n\r";
		uint8_t data;

		print_data((uint8_t *)MAIN_HEADING,sizeof(MAIN_HEADING));
		print_message(DEV_STATUS);
		print_data(device_status,strlen((const char*)device_status));
		print_message(COMP_STATUS);


		if (i2c_send_data(&curr_config,NULL) == SENDCONFIG_SUCCESS){
			companion_status = SLAVE_CONNECTED;
			print_message("Connected");

		} else {
			companion_status = SLAVE_NOT_CONNECTED;
			print_message("Not Connected");
		}
		display_config();

		print_message(PROMPT_HS);

		data = get_keyinput();
		data = toupper(data);
		update_cmd_struct(data);

		switch (data){

		case 'S' :
			state = SYSTEMCONFIG_SCREEN;
			break;

		case 'P' :
			state = PATTERNCONFIG_SCREEN;
			break;

		case 'X' :
			state = EXECUTION_SCREEN;

			break;

		default :
			print_message(ERROR_MSG);
		}
		return state;
	}
	else {
		uint8_t device_status[] = "Slave\n\r";
		uint16_t response;
		print_data((uint8_t *)MAIN_HEADING,sizeof(MAIN_HEADING));
		print_message(DEV_STATUS);
		print_data(device_status,strlen((const char*)device_status));
		print_message(COMP_STATUS);
		print_message("Waiting for master");

		response =i2c_recieve_data(&curr_config,NULL);
		if (response == RECIEVECONFIG_SUCCESS ) {
			reverse_direction();
			print_message("\r");
			print_message(COMP_STATUS);
			print_message("Connected                     ");

			while(1)
			{
				print_message("\x1b[2J");
				print_data((uint8_t *)MAIN_HEADING,sizeof(MAIN_HEADING));
				print_message(DEV_STATUS);
				print_data(device_status,strlen((const char*)device_status));
				print_message(COMP_STATUS);
				print_message("Connected                     ");
				display_config();
				response = i2c_recieve_data(&curr_config,&cmd_ascii);


				if ( response == RECIEVECONFIG_SUCCESS) {
					reverse_direction();
					continue;
				} else if (response== RECIEVECOMMAND_SUCCESS){

					state = EXECUTION_SCREEN;
					return state;
				}
			}
		}
	}
	return state;
}
/**
 * @brief Execution function
 *
 * Displays the execution screen on master and slave devices,
 * displays current status of the led operation, validates user input
 * and switches screen according to user input.
 *
 * @param 	none.
 * @return 	state
 *
 * @note
 *
 * Revision History:
 * 	- 120121 MG: 	Creation Date
 */

int execution_func()
{
	uint8_t data ;
	uint8_t cycles_hex = DEFAULT_VALUE;
	uint8_t current_status[5],value[1];
	if ((is_master==1)) {
		uint8_t device_status[] = "Master\n\r";
		print_data((uint8_t *)MAIN_HEADING,sizeof(MAIN_HEADING));
		print_message(DEV_STATUS);
		print_data(device_status,strlen((const char*)device_status));
		print_message(COMP_STATUS);


		if (companion_status == SLAVE_CONNECTED){
			i2c_send_data(NULL,&cmd_ascii);
			print_message("Connected");
		}

		else {
			print_message("Not Connected");
		}

	} else {
		uint8_t device_status[] = "Slave\n\r";
		print_data((uint8_t *)MAIN_HEADING,sizeof(MAIN_HEADING));
		print_message(DEV_STATUS);
		print_data(device_status,strlen((const char*)device_status));
		print_message(COMP_STATUS);

		if (i2c_recieve_data(NULL,&cmd_ascii) == RECIEVECOMMAND_SUCCESS ) {


			print_message("Connected");
		}
	}

	send_config_task(&curr_config);
	ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
	send_command_to_task(&cmd_ascii);

	display_config();

	if(is_master == 1){
		if (curr_config.mode == '1'){
			print_data((uint8_t *)MANUAL_KEYS,sizeof(MANUAL_KEYS));
		} else {
			print_data((uint8_t *)AUTO_KEYS,sizeof(AUTO_KEYS));
		}
	}else{
	print_message("\n\n\r********************************************************************************\n\n");
	}
	while(1){

		receive_status_from_task(current_status);

		print_message(COLOUR_STATUS);
		for(int i=0;;i++) {
			memcpy(&value,&current_status[i],1);
			print_data(value,1);
			if(i==2)
				break;
			print_message(",");
		}
		cycles_hex=atoi((char *)curr_config.cycles);

		if (curr_config.mode == '2' &&  cycles_hex != 0) {
			print_message(CYCLE_STATUS);
			for(int i=3;i<5;i++){
				memcpy(&value,&current_status[i],1);
				print_data(value,1);
			}
		}

		if (is_master == 1) {
			data = get_keyinput_nonblocking();
			data = toupper(data);
			if (validate_executionkey(data) == kStatus_Fail) {
				continue;
			} else {
				update_cmd_struct(data);
				i2c_send_data(NULL,&cmd_ascii);

			}
		} else {

			i2c_recieve_data(NULL,&cmd_ascii);
		}

		send_command_to_task(&cmd_ascii);

		if (data == 'Q'||cmd_ascii.command == 'Q'){
			ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
			state = HOMESCREEN;
			return state;
		} else {
			cmd_ascii.command = DEFAULT_COMMAND;
			continue;
		}
	}
}
/**
 * @brief Pattern Configuration function
 *
 * Displays the pattern screen, validates user input
 * and switches screen according to user input.
 *
 * @param 	none.
 * @return 	state
 *
 * @note
 *
 * Revision History:
 * 	- 120121 MG: 	Creation Date
 */
int patternconfig_func(){
	print_data((uint8_t*)PAT_CONFIG,sizeof(PAT_CONFIG));
	print_data((uint8_t*)PROMPT_PS,sizeof(PROMPT_PS));

	uint8_t data = get_keyinput();
	data = toupper(data);
	switch (data) {

	case 'Q' :
		state = HOMESCREEN;
		break;

	case 'C' :
		color_config_func();
		break;

	case 'M' :
		mode_config();
		break;

	case 'D' :
		direction_config();
		break;

	default :
		print_message(ERROR_MSG);

	}
	return state;
}




