/******************************************************************************************************
 * @file 			UART_Support.c
 * @brief			Functions related to the use of the UART peripheral
 * ****************************************************************************************************
 * @author original	Hab Collector (habco)\n
 *
 * @version       	See Main_Support.h: FIRMWARE_REV_MAJOR, FIRMWARE_REV_MINOR
 *
 * @param Development_Environment \n
 * Hardware:		<IC PN> \n
 * IDE:             STMCubeIDE VER 1.9.0 \n
 * Compiler:        GCC \n
 * Editor Settings: 1 Tab = 4 Spaces, Recommended Courier New 11
 *
 * @note            The associated header file provides MACRO functions for IO control
 *
 *                  This is an embedded application
 *                  It will be necessary to consult the reference documents to fully understand the code
 *                  It is suggested that the documents be reviewed in the order shown.
 *                    Schematic: IMR-005-SCH
 *                    STM32F746G_DISCO
 *                    IMR Engineering
 *
 * @copyright		IMR Engineering, LLC
 ******************************************************************************************************/

#include "UART_Support.h"
#include "Main_Support.h"
#include "Debug_Port.h"
#include <string.h>
#include <stdio.h>

extern UART_HandleTypeDef huart6;

// Static Function Declarations
static void Init_UART_DMA(Type_UART_Bus *UART_Bus, UART_HandleTypeDef *UART_Handle, Type_UART_Parser ParserFunction);
static uint16_t UART_DMA_Transmit(Type_UART_Bus *UART_Bus, uint8_t *TxBuffer, uint16_t TxLength);

// Global
Type_UART_Bus UART_DebugPort;


/********************************************************************************************************
* @brief Init of the UART peripheral
*
* @author original: Hab Collector \n
*
* @note: Only a single UART (UART 6) is used at this time
* @note: UART 6 will be used as the debug port
*
* STEP 1: Init the UART in DMA mode
********************************************************************************************************/
void Init_UART_Hardware(void)
{
    // STEP 1: Init the UART in DMA mode
    Init_UART_DMA(&UART_DebugPort, &huart6, debugPortParser);

} // END OF Init_UART_Hardware



/********************************************************************************************************
* @brief Init of the UART peripheral in DMA mode
*
* @author original: Hab Collector \n
*
* @note: UART 6 will be used as the debug port
* @note: The UART will have to be set for receive each time a receive condition is met (RxCmpltCallback)
* @note: The IRQ is set on the reception of one byte - makes it easy for parsing the data
*
* @param UART_Bus: UART Bus type handle;
* @param UART_Handle: UART handle as used by the HAL
* @param
*
* STEP 1: Init struct member to default POR values and clear the Rx and Tx buffers
* STEP 2: Ready the UART to receive data
********************************************************************************************************/
static void Init_UART_DMA(Type_UART_Bus *UART_Bus, UART_HandleTypeDef *UART_Handle, Type_UART_Parser ParserFunction)
{
    // STEP 1: Init struct member to default POR values and clear the Rx and Tx buffers
    UART_Bus->Handle = UART_Handle;
    UART_Bus->IsTransmitting = false;
    UART_Bus->Parser = ParserFunction;
    UART_Bus->Rx_FIFO_Count = 0;
    memset(UART_Bus->Rx_FIFO, 0x00, sizeof(UART_Bus->Rx_FIFO));
    memset(UART_Bus->RxDataBuffer, 0x00, sizeof(UART_Bus->RxDataBuffer));
    memset(UART_Bus->TxDataBuffer, 0x00, sizeof(UART_Bus->TxDataBuffer));

    // STEP 2: Ready the UART to receive data
    HAL_UART_Receive_IT(UART_Bus->Handle, (uint8_t *)UART_Bus->RxDataBuffer, 1);

} // END OF Init_UART_DMA



/********************************************************************************************************
* @brief Transmit UART data via DMA mode
*
* @author original: Hab Collector \n
*
* @note: Transmitting the data on the UART causes the UART to enter the busy (transmitting) state
* @note: The UART busy (IsTransmitting) state is cleared when the transmission is complete TxCmpltCallback
*
* @param UART_Bus: UART Bus type handle
* @param TxBuffer: The buffer to transmit
* @param TxLength: The number of bytes in the buffer to transmit
*
* @return The number of bytes transmitted via the UART
*
* STEP 1: Do nothing if UART is transmitting
* STEP 2: Transmit data buffer - set status to transmitting
********************************************************************************************************/
static uint16_t UART_DMA_Transmit(Type_UART_Bus *UART_Bus, uint8_t *TxBuffer, uint16_t TxLength)
{
    // STEP 1: Do nothing if UART is transmitting
    if (UART_Bus->IsTransmitting)
        return(0);

    // STEP 2: Transmit data buffer - set status to transmitting
    HAL_StatusTypeDef Status = HAL_UART_Transmit_DMA(UART_Bus->Handle, TxBuffer, TxLength);
    if (Status == HAL_OK)
    {
        UART_Bus->IsTransmitting = true;
        return(TxLength);
    }
    else
    {
        return(0);
    }

} // END OF UART_DMA_Transmit



/********************************************************************************************************
* @brief REDECLARATION of the HAL HAL_UART_TxCpltCallback.  The function is defined weak elsewhere.  It is
* redefined here for use.  This function is called by the HAL when the UART has completed the transmission.
*
* @author original: Hab Collector \n
*
* @note: This is re-declaration of a weak HAL function
* @note: Handles all active UARTs
* @note: Called on the completion of any UART transmission
*
* @param huart: HAL UART handle
*
* STEP 1: Identify the active UART and clear the transmitting status
********************************************************************************************************/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    // STEP 1: Identify the active UART and clear the transmitting status
    if (huart == UART_DebugPort.Handle)
    {
        UART_DebugPort.IsTransmitting = false;
    }

} // END OF HAL_UART_TxCpltCallback



/********************************************************************************************************
* @brief REDECLARATION of the HAL HAL_UART_RxCpltCallback.  The function is defined weak elsewhere.  It is
* redefined here for use.  This function is called by the HAL when the UART has completed a reception.
*
* @author original: Hab Collector \n
*
* @note: This is re-declaration of a weak HAL function
* @note: Handles all active UARTs
* @note: Called on the reception of any UART
* @note: Receive only 1 char per IRQ
*
* @param huart: HAL UART handle
*
* STEP 1: Identify the active UART
* STEP 2: Load the RX FIFO Buffer and re-enable the UART to receive via IRQ
********************************************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    // STEP 1: Identify the active UART
    if (huart == UART_DebugPort.Handle)
    {
        // STEP 2: Load the RX FIFO Buffer and re-enable the UART to receive via IRQ
        UART_DebugPort.Rx_FIFO[UART_DebugPort.Rx_FIFO_Count] = UART_DebugPort.RxDataBuffer[0];
        UART_DebugPort.Rx_FIFO_Count++;
        // Should not occur - but just to be safe test for over-write
        if (UART_DebugPort.Rx_FIFO_Count >= UART_RX_FIFO_BUFFER_SIZE)
            UART_DebugPort.Rx_FIFO_Count = 0;
        HAL_UART_Receive_IT(UART_DebugPort.Handle, (uint8_t *)UART_DebugPort.RxDataBuffer, 1);
    }

} // END OF HAL_UART_RxCpltCallback



/********************************************************************************************************
 * @brief This is the redefinition of the write output function used by printf.  Which is defined weak elsewhere
* in syscalls.c and redefined here to target the UART for output.
*
* @author original: Hab Collector \n
*
* @note The full implementation is not used - hence file is ignored.  This is simply to use printf on UART
*
* @param ptr: Pointer to the print message
* @param len: Size of the print message
*
* @return: The number of characters printed
*
* STEP 1: If Not transmitting write the contents to be be printed to the UART DMA
* STEP 2: Do nothing until the DMA has completed the transmission - this will stop over-writes of the output printf
********************************************************************************************************/
int _write(int file, char *ptr, int len)
{
    uint16_t BytesTransmitted = 0;
    if (!UART_DebugPort.IsTransmitting)
    {
        strncpy(UART_DebugPort.RxDataBuffer, ptr, len);
        BytesTransmitted = UART_DMA_Transmit(&UART_DebugPort, (uint8_t *)UART_DebugPort.RxDataBuffer, len);
    }
    do
    {
        DO_NOTHING();
    } while(UART_DebugPort.IsTransmitting);

    return(BytesTransmitted);

} // END OF _write
