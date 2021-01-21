//
///**
// * @file    config_manager.c
// * @brief   Application entry point.
// */
///* FreeRTOS kernel includes. */
//
//#include <aurora_i2c.h>
//#include "FreeRTOS.h"
//#include "task.h"
//#include "queue.h"
//#include "timers.h"
//
//#include "board.h"
//#include "peripherals.h"
//#include "pin_mux.h"
//#include "clock_config.h"
//#include "fsl_debug_console.h"
//
//#include <consolemanager.h>
//
//#define config_task_PRIORITY 1U
//
//
//QueueHandle_t uart_queue;
//
///*******************************************************************************
// * Prototypes
// ******************************************************************************/
//
////none
//
///*******************************************************************************
// * Variables
// ******************************************************************************/
//
//
//
///*
// * @brief   Application entry point.
// */
//int main(void) {
//
//  	/* Init board hardware. */
//	BOARD_InitBootPins();
//    BOARD_InitBootClocks();
//    BOARD_InitBootPeripherals();
//  	/* Init FSL debug console. */
//    BOARD_InitDebugConsole();
//    uart_init();
//
//    uint32_t result = init_i2c_tranfer();
//    if (result == MASTER_CONFIGURED)
//    	is_master = 1;
//    else
//    	is_master = 0;
//     uart_queue=xQueueCreate(1,sizeof(uint8_t));
////    create_uartqueue();
//    if (xTaskCreate(config_task, "uart_task", configMINIMAL_STACK_SIZE + 10, NULL, config_task_PRIORITY, NULL) != pdPASS)
//       {
//           PRINTF("Task creation failed!.\r\n");
//           while (1)
//               ;
//       }
//       vTaskStartScheduler();
//       for (;;)
//           ;
//    return 0 ;
//}
//
//
