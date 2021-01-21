/**
* @file_Aurora_mk1.h
* @brief Aurora i2c header mk1
*
* Header file for Aurora i2c mk1
*
* @note
*
* Revision History:
* - 150121 FTJ: Creation Date
*/
#ifndef _AURORA_I2C_H_INCLUDED
#define _AURORA_I2C_H_INCLUDED

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"

#include "fsl_i2c.h"
#include "fsl_i2c_edma.h"
#include "fsl_dmamux.h"

#include "fsl_common.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "statemachine.h"
#include "datamanager.h"
/***********************************
* Const and Macro Defines
***********************************/
#define I2C_MASTER_CLK_SRC I2C0_CLK_SRC      /**/
#define I2C_CLK_FREQ CLOCK_GetFreq(I2C0_CLK_SRC)
#define I2C_BASEADDR I2C0
#define DMA_REQUEST_SRC kDmaRequestMux0I2C0
#define I2C_DMAMUX_BASEADDR DMAMUX0
#define I2C_DMA_BASEADDR DMA0
#define I2C_DMA_CHANNEL 0U

#define I2C_MASTER_SLAVE_ADDR_7BIT 0x7EU /*I2C 7bit Address in Slave Mode*/
#define I2C_BAUDRATE 100000U             /*I2C Clock 100KHz */
#define CONFIG_LENGTH 19U                /*Configuration Maximum Length*/
#define COMMAND_LENGTH 3U                /*Command Maximum Length*/
/*PORTTC4 is used as the System Configuration Pin in this application*/
#define SYSTEM_JUMPER_PORT GPIOC         /*PORT for Jumper Selection*/
#define SYSTEM_JUMPER_PIN 4U             /*Pin for Jumper Selection*/
/*An integer value to differentiate MASTER OR SLAVE*/
#define MASTER 1
#define SLAVE 0
/*DATA ID to differentiate between Configuration and Command*/
#define CONFIG_DATA_ID '0'
#define COMMAND_DATA_ID '1'
/***********************************
* Typedefs and Enum Declarations
***********************************/

typedef enum {
	MASTER_CONFIGURED         =  0U,
	SLAVE_CONFIGURED          =  1U,
	SENDCONFIG_SUCCESS        =  2U,
	RECIEVECONFIG_SUCCESS     =  3U,
	SENDCOMMAND_SUCCESS       =  4U,
	RECIEVECOMMAND_SUCCESS    =  5U,
	SENDCONFIG_FAIL           =  6U,
	RECIEVECONFIG_FAIL        =  7U,
	SENDCOMMAND_FAIL          =  8U,
	RECIEVECOMMAND_FAIL       =  9U,
	NOTHING_SPECIAL_HAPPENED  = 10U,
}transfer_status;



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

/*!
 * brief This Function initializes the Hardware based on application configuration
 * as MASTER or SLAVE
 *
 *Calling this Function will check the system configuration as master or slave
 *by checking the status of the SYSTEM_JUMPER_PIN
 *IF JUMPER PRESENT "SLAVE MODE"
 *ELSE "MASTER MODE"
 *
 *
 *
 * param  NONE
 *
 * retval MASTER_CONFIGURED on successful initialization of  Master
 * retval SLAVE_CONFIGURED  on successful initialization of  Slave
 */
status_t init_i2c_tranfer();
/*!
 * brief Transmit commands and configurations in MASTER MODE only
 *
 * Call this Function with appropriate parameters ,say the structure we want to Transmit
 * at that time , will initiate the transfer with DMA (in the case of configuration transfer)
 * and a Normal NON-DMA transfer in the case of Command transfer (which is only 2 bytes).
 *
 *
 * param configuration structure pointer.
 * param Command Structure pointer
 *
 * retval SENDCONFIG_SUCCESS on successfully receiving the configuration
 * retval SENDCONFIG_SUCCESS on successfully receiving the command
 * retval SENDCOMMAND_FAIL on command transfer failure
 * retval SENDCONFIG_FAIL on configuration transfer failure.
 */
status_t i2c_send_data(config_ascii_t *configuration,command_ascii_t *command);
/*!
 * brief Recieves commands and configurations in SLAVE MODE only
 *
 * Call this Function with appropriate parameter , and the Received data from MASTER is checked and
 * assigned to the structure based on the data_id received
 *
 *
 * param configuration structure pointer.
 * param Command Structure pointer
 *
 * retval RECIEVECONFIG_SUCCESS on successfully receiving the configuration
 * retval RECIEVECOMMAND_SUCCESS on successfully receiving the command
 */

status_t i2c_recieve_data(config_ascii_t *configuration,command_ascii_t *command);


#endif /*_AURORA_I2C_H_ INCLUDED*/
