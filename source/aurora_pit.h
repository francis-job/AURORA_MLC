
/**
 * @file	aurora_pit.h
 * @brief contains functions related to the Programmable Interrupt Timer
 *
 *
 *
 * @note
 *
 * Revision History:
 *		- 110121 MAP : Creation Date
 */

#ifndef AURORA_PIT_H_
#define AURORA_PIT_H_


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
#include "math.h"
#include "fsl_common.h"
#include "stdbool.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include"aurora_pg_data_manager.h"
#include "aurora_pat_gen.h"
/*******************************************
 * Const and Macro Defines
 *******************************************/
#define start_pit_timer(PIT, kPIT_Chnl_0)  PIT_StartTimer((PIT), (kPIT_Chnl_0));
#define stop_pit_timer(PIT, kPIT_Chnl_0)  PIT_StopTimer((PIT), (kPIT_Chnl_0));

/***********************************
 * Typedefs and Enum Declarations
 ***********************************/

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
status_t set_pit_interval(config_hex_t *xconfig_hex);
void enable_pit_interrupt(void);
void pit_init(void);
bool change_rate_delay(config_hex_t* xconfig_hex,command_ascii_t * xcommand_ascii);

#endif /* AURORA_PIT_H_ */
