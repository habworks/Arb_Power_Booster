 /*****************************************************************************************************
 * @file 			UART_Support.h
 * @brief			This is the Header file used to support UART_Support.c
 * ****************************************************************************************************
 * @author original Hab Collector (habco)\n
 *
 * @version         See Main_Support.h: FW_MAJOR_REV, FW_MINOR_REV, FW_TEST_REV
 *
 * @param Development_Environment \n
 * Hardware:        <IC PN>  \n
 * IDE:             STMCubeIDE VER 1.17.0 \n
 * Compiler:        GCC \n
 * Editor Settings: 1 Tab = 4 Spaces, Recommended Courier New 11
 *
 * @note            <Add user notes starting here>
 *
 *                  This is an embedded application
 *                  It will be necessary to consult the reference documents to fully understand the code
 *                  It is suggested that the documents be reviewed in the order shown.
 *                    Schematic: <Schematic PN>
 *				      STM32F746G_DISCO
 *                    IMR Engineering
 *
 * @copyright       IMR Engineering, LLC
 ******************************************************************************************************/

#ifndef UART_SUPPORT_H_
#define UART_SUPPORT_H_
#ifdef __cplusplus
extern"C" {
#endif

#include "Hab_Types.h"
#include "stm32f7xx_hal.h"


// DEFINES
#define UART_TX_BUFFER_SIZE     256U
#define UART_RX_BUFFER_SIZE     256U
#define UART_RX_FIFO_BUFFER     10U


// TYPEDEFS AND ENUMS
typedef void (*Type_UART_Parser)(char *, uint8_t);
typedef struct
{
    UART_HandleTypeDef *    Handle;
    bool                    IsTransmitting;
    uint8_t                 Rx_FIFO_Count;
    char                    Rx_FIFO[UART_RX_FIFO_BUFFER];
    char                    RxDataBuffer[UART_RX_BUFFER_SIZE];
    char                    TxDataBuffer[UART_TX_BUFFER_SIZE];
    Type_UART_Parser        Parser;
}Type_UART_Bus;


// EXTERNS
extern Type_UART_Bus UART_DebugPort;


// FUNCTION PROTOTYPES
void Init_UART_Hardware(void);

#ifdef __cplusplus
}
#endif
#endif /* UART_SUPPORT_H_ */
