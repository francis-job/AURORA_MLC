/*
 * aurora_mlc.h
 *
 *  Created on: 20-Jan-2021
 *      Author: dct
 */

#ifndef AURORA_MLC_H_
#define AURORA_MLC_H_
#include <pg_task.h>
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
#include <consolemanager.h>
#include "aurora_i2c.h"


#endif /* AURORA_MLC_H_ */
