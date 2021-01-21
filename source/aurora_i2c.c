/**
* @file Aurora_i2c.c
* @brief functional driver of i2cTransfer in Aurora MLC Application.
*
*
*
* @note
*
* Revision History:
* - 150121 FTJ : Creation Date
*/
#include "aurora_i2c.h"


/*******************************************
* Const and Macro Defines
*******************************************/

/***********************************
* Typedefs and Enum Declarations
***********************************/
// none
/***********************************
* External Variable Declarations
***********************************/
//none
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
* **********************************/
i2c_master_config_t masterConfig;
i2c_master_handle_t masterHandle;
i2c_master_transfer_t masterXfer;
i2c_slave_transfer_t slaveXfer;
edma_config_t config;

i2c_master_edma_handle_t g_m_dma_handle;
edma_handle_t edmaHandle;
i2c_slave_handle_t g_s_handle;

uint8_t tx_Buff[CONFIG_LENGTH];
uint8_t rx_Buff[CONFIG_LENGTH];



volatile bool g_SlaveCompletionFlag = false;
volatile bool g_MasterCompletionFlag = false;

/***********************************
* Private Prototypes
***********************************/
static void i2c_slave_callback(I2C_Type *base, i2c_slave_transfer_t *xfer, void *userData);
static void i2c_master_callback1(I2C_Type *base, i2c_master_edma_handle_t *handle, status_t status, void *userData);
static void i2c_master_callback2(I2C_Type *base, i2c_master_handle_t *handle, status_t status, void *userData);
/***********************************
* Public Functions
***********************************/
status_t init_i2c_tranfer();
status_t i2c_send_data(config_ascii_t *configuration,command_ascii_t *command);
status_t i2c_recieve_data(config_ascii_t *configuration,command_ascii_t *command);


static void i2c_slave_callback(I2C_Type *base, i2c_slave_transfer_t *xfer, void *userData)
{
    switch (xfer->event){
        /*  Address match event */
        case kI2C_SlaveAddressMatchEvent:
            xfer->data     = NULL;
            xfer->dataSize = 0;
            break;
        /*  Receive request */
        case kI2C_SlaveReceiveEvent:
            /*  Update information for received process */
            xfer->data     = rx_Buff;
            xfer->dataSize = CONFIG_LENGTH;
            break;

        /*  Transfer done */
        case kI2C_SlaveCompletionEvent:
            g_SlaveCompletionFlag = true;
            xfer->data            = NULL;
            xfer->dataSize        = 0;
            break;

        default:
            g_SlaveCompletionFlag = false;
            break;
    }
}

static void i2c_master_callback1(I2C_Type *base, i2c_master_edma_handle_t *handle, status_t status, void *userData)
{
    /* Signal transfer success when received success status. */
    if (status == kStatus_Success){
        g_MasterCompletionFlag = true;
    }
}
static void i2c_master_callback2(I2C_Type *base, i2c_master_handle_t *handle, status_t status, void *userData)
{
    /* Signal transfer success when received success status. */
    if (status == kStatus_Success){
        g_MasterCompletionFlag = true;
    }
}
/*!
 * brief This Function Check System Configuration MODE &  initializes the Hardware based on  SYSTEM CONFIGURATION
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

status_t init_i2c_tranfer()
{

	  uint32_t sourceClock;
	  i2c_slave_config_t slaveConfig;
	  gpio_pin_config_t jumper_config = {
	  		    		kGPIO_DigitalInput,
	  					0,
	  		    	    };

      /*JUMPER INITIALIZATION AND STATE CHECK*/
      GPIO_PinInit(SYSTEM_JUMPER_PORT,SYSTEM_JUMPER_PIN,&jumper_config);
      uint16_t state = GPIO_PinRead(SYSTEM_JUMPER_PORT,SYSTEM_JUMPER_PIN);

    sourceClock = I2C_CLK_FREQ;
//MASTER
    if (state == MASTER){
    	DMAMUX_Init(I2C_DMAMUX_BASEADDR);
    	EDMA_GetDefaultConfig(&config);
    	EDMA_Init(I2C_DMA_BASEADDR, &config);
    	I2C_MasterGetDefaultConfig(&masterConfig);
    	masterConfig.baudRate_Bps = I2C_BAUDRATE;
/*I2C initializations as Master*/
    	I2C_MasterInit(I2C_BASEADDR, &masterConfig, sourceClock);
    	memset(&g_m_dma_handle, 0, sizeof(g_m_dma_handle));
    	memset(&masterXfer, 0, sizeof(masterXfer));

    	uint8_t deviceAddress     = 0x01U;
    	masterXfer.slaveAddress   = I2C_MASTER_SLAVE_ADDR_7BIT;
    	masterXfer.direction      = kI2C_Write;
        masterXfer.subaddress     = (uint32_t)deviceAddress;
    	masterXfer.subaddressSize = 1;
        masterXfer.flags          = kI2C_TransferDefaultFlag;

        DMAMUX_SetSource(I2C_DMAMUX_BASEADDR, I2C_DMA_CHANNEL, DMA_REQUEST_SRC);
        EDMA_CreateHandle(&edmaHandle,I2C_DMA_BASEADDR, I2C_DMA_CHANNEL);
        DMAMUX_EnableChannel(I2C_DMAMUX_BASEADDR, I2C_DMA_CHANNEL);

        I2C_MasterCreateEDMAHandle(I2C_BASEADDR, &g_m_dma_handle, i2c_master_callback1, NULL, &edmaHandle);
        I2C_MasterTransferCreateHandle(I2C_BASEADDR,&masterHandle, i2c_master_callback2,NULL);

        return MASTER_CONFIGURED;
    }
//SLAVE
    else if(state == SLAVE)
    {
    	I2C_SlaveGetDefaultConfig(&slaveConfig);   /*I2C initializations as SLAVE*/

    	slaveConfig.addressingMode = kI2C_Address7bit;
    	slaveConfig.slaveAddress   = I2C_MASTER_SLAVE_ADDR_7BIT;
    	slaveConfig.upperAddress   = 0;

    	I2C_SlaveInit(I2C_BASEADDR, &slaveConfig,sourceClock);
    	for (uint32_t i = 0U; i < CONFIG_LENGTH; i++){
    	        rx_Buff[i] = 0;
    	    }

    	 memset(&g_s_handle, 0, sizeof(g_s_handle));

         I2C_SlaveTransferCreateHandle(I2C_BASEADDR, &g_s_handle, i2c_slave_callback, NULL);
         return SLAVE_CONFIGURED;
    }

   return 0;
}

/*!
 * brief Transmit commands and configurations in MASTER MODE
 *
 * Call this Function with appropriate paramters ,say the structure we want to Transmit
 * at that time , will initiate the transfer with DMA (in the case of configuration transfer)
 * and a Normal NON DMA transfer in the case of Command
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
status_t i2c_send_data(config_ascii_t *configuration,command_ascii_t *command)
{
	uint32_t size =0;
	uint16_t result = NOTHING_SPECIAL_HAPPENED;
	memset(&tx_Buff, 0, sizeof(rx_Buff));
	if(configuration != NULL){
		size = sizeof(*configuration);
	    memcpy(&tx_Buff[0],configuration,size);
	    masterXfer.data           = tx_Buff;
	    masterXfer.dataSize       = CONFIG_LENGTH;
	    result = I2C_MasterTransferEDMA(I2C_BASEADDR, &g_m_dma_handle, &masterXfer);

	    if((result == kStatus_I2C_Nak || result == kStatus_I2C_Addr_Nak)){

	    	result = SENDCONFIG_FAIL ;
	    }else{
	    	result = SENDCONFIG_SUCCESS;
	    }
	}else{
		size = sizeof(*command);
	    memcpy(&tx_Buff[0],command,size);
	    masterXfer.data           = tx_Buff;
	    masterXfer.dataSize       = COMMAND_LENGTH;
	    result = I2C_MasterTransferNonBlocking(I2C_BASEADDR,&masterHandle,&masterXfer);

	    if(result ==kStatus_Success){
	         result = SENDCOMMAND_SUCCESS;
	       }else{
              result = SENDCOMMAND_FAIL;
	    	    }
	      }
	return result;
}
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

status_t i2c_recieve_data(config_ascii_t *configuration,command_ascii_t *command)
{
	uint8_t id = 0;
	uint16_t result = NOTHING_SPECIAL_HAPPENED;
	I2C_SlaveTransferNonBlocking(I2C_BASEADDR, &g_s_handle,
	                                 kI2C_SlaveCompletionEvent | kI2C_SlaveAddressMatchEvent);
/*HALT here only while receiving or expecting configuration */
  while (!g_SlaveCompletionFlag && (configuration != NULL)){
	    }
	if(g_SlaveCompletionFlag == true){
	      id = rx_Buff[1];
       if(id == CONFIG_DATA_ID){
	      memcpy(configuration,&rx_Buff[1],CONFIG_LENGTH -1);
	      g_SlaveCompletionFlag = false;
	      memset(&rx_Buff, 0, sizeof(rx_Buff));

	      result = RECIEVECONFIG_SUCCESS;

        }else if(id == COMMAND_DATA_ID){
    	   memcpy(command,&rx_Buff[1],COMMAND_LENGTH -1);
    	   g_SlaveCompletionFlag = false;
    	   memset(&rx_Buff, 0, sizeof(rx_Buff));
    	  result = RECIEVECOMMAND_SUCCESS;
         }

	  }
	return result;
}
