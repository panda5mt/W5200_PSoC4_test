/*******************************************************************************
* File Name: SPI_SPI_UART_PVT.h
* Version 1.10
*
* Description:
*  This private file provides constants and parameter values for the
*  SCB Component in SPI and UART modes.
*  Please do not use this file or its content in your project.
*
* Note:
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_SPI_UART_PVT_SPI_H)
#define CY_SCB_SPI_UART_PVT_SPI_H

#include "SPI_SPI_UART.h"


/***************************************
*     Internal Global Vars
***************************************/

#if(SPI_INTERNAL_RX_SW_BUFFER_CONST)
    extern volatile uint32  SPI_rxBufferHead;
    extern volatile uint32  SPI_rxBufferTail;
    extern volatile uint8   SPI_rxBufferOverflow;
#endif /* (SPI_INTERNAL_RX_SW_BUFFER_CONST) */

#if(SPI_INTERNAL_TX_SW_BUFFER_CONST)
    extern volatile uint32  SPI_txBufferHead;
    extern volatile uint32  SPI_txBufferTail;
#endif /* (SPI_INTERNAL_TX_SW_BUFFER_CONST) */

#if(SPI_INTERNAL_RX_SW_BUFFER)
    extern volatile uint8 SPI_rxBufferInternal[SPI_RX_BUFFER_SIZE];
#endif /* (SPI_INTERNAL_RX_SW_BUFFER) */

#if(SPI_INTERNAL_TX_SW_BUFFER)
    extern volatile uint8 SPI_txBufferInternal[SPI_TX_BUFFER_SIZE];
#endif /* (SPI_INTERNAL_TX_SW_BUFFER) */


/***************************************
*     Private Function Prototypes
***************************************/

#if(SPI_SCB_MODE_SPI_CONST_CFG)
    void SPI_SpiInit(void);
#endif /* (SPI_SCB_MODE_SPI_CONST_CFG) */

#if(SPI_SPI_WAKE_ENABLE_CONST)
    void SPI_SpiSaveConfig(void);
    void SPI_SpiRestoreConfig(void);
#endif /* (SPI_SPI_WAKE_ENABLE_CONST) */

#if(SPI_SCB_MODE_UART_CONST_CFG)
    void SPI_UartInit(void);
#endif /* (SPI_SCB_MODE_UART_CONST_CFG) */

#if(SPI_UART_WAKE_ENABLE_CONST)
    void SPI_UartSaveConfig(void);
    void SPI_UartRestoreConfig(void);
#endif /* (SPI_UART_WAKE_ENABLE_CONST) */

/* Interrupt processing */
#define SPI_SpiUartEnableIntRx(intSourceMask)  SPI_SetRxInterruptMode(intSourceMask)
#define SPI_SpiUartEnableIntTx(intSourceMask)  SPI_SetTxInterruptMode(intSourceMask)
uint32 SPI_SpiUartDisableIntRx(void);
uint32 SPI_SpiUartDisableIntTx(void);

#endif /* (CY_SCB_SPI_UART_PVT_SPI_H) */


/* [] END OF FILE */
