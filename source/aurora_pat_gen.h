/*
 * aurora_pat_gen.h
 *
 *  Created on: 15-Jan-2021
 *      Author: Nikhil
 */

#ifndef AURORA_PAT_GEN_H_
#define AURORA_PAT_GEN_H_

#include"string.h"
#include "aurora_pg_data_manager.h"
/*******************************************
 * Const and Macro Defines
 *******************************************/
#define start_pit_timer(PIT, kPIT_Chnl_0)  PIT_StartTimer((PIT), (kPIT_Chnl_0));
#define DEFAULT_VALUE 1u
#define MODE_INDICATOR '_' //If this is sent via status queue it shows that the pattern generation is in continuous mode
#define DEFAULT_COMMAND '5'
/***********************************
 * Typedefs and Enum Declarations
 ***********************************/
//none

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
void pattern_generator(int8_t *current_color,config_hex_t* xconfig_hex);
void pattern_log_off();
int8_t update_current_color_up(config_hex_t* xconfig,int8_t* current_colorinitial);
int8_t update_current_color_down(config_hex_t* xconfig,int8_t* current_colorinitial);
void manual_pattern(config_hex_t* xconfig_hex, command_ascii_t* xcommand_ascii);
void auto_pattern(config_hex_t* xconfig_hex, command_ascii_t* xcommand_ascii);

void up_direction(config_hex_t* xconfig_hex, command_ascii_t* xcommand_ascii);
void down_direction(config_hex_t* xconfig_hex, command_ascii_t* xcommand_ascii);
void up_down_direction(config_hex_t* xconfig_hex, command_ascii_t* xcommand_ascii);
void down_up_direction(config_hex_t* xconfig_hex, command_ascii_t* xcommand_ascii);

#endif /* AURORA_PAT_GEN_H_ */
