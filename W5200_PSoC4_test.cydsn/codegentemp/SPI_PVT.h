/*******************************************************************************
* File Name: .h
* Version 1.10
*
* Description:
*  This private file provides constants and parameter values for the
*  SCB Component in I2C mode.
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

#if !defined(CY_SCB_PVT_SPI_H)
#define CY_SCB_PVT_SPI_H

#include "SPI.h"


/***************************************
*     Private Function Prototypes
***************************************/

/* APIs to service INTR_I2C_EC register */
#define SPI_SetI2CExtClkInterruptMode(interruptMask) SPI_WRITE_INTR_I2C_EC_MASK(interruptMask)
#define SPI_ClearI2CExtClkInterruptSource(interruptMask) SPI_CLEAR_INTR_I2C_EC(interruptMask)
#define SPI_GetI2CExtClkInterruptSource()                (SPI_INTR_I2C_EC_REG)
#define SPI_GetI2CExtClkInterruptMode()                  (SPI_INTR_I2C_EC_MASK_REG)
#define SPI_GetI2CExtClkInterruptSourceMasked()          (SPI_INTR_I2C_EC_MASKED_REG)

/* APIs to service INTR_SPI_EC register */
#define SPI_SetSpiExtClkInterruptMode(interruptMask) SPI_WRITE_INTR_SPI_EC_MASK(interruptMask)
#define SPI_ClearSpiExtClkInterruptSource(interruptMask) SPI_CLEAR_INTR_SPI_EC(interruptMask)
#define SPI_GetExtSpiClkInterruptSource()                 (SPI_INTR_SPI_EC_REG)
#define SPI_GetExtSpiClkInterruptMode()                   (SPI_INTR_SPI_EC_MASK_REG)
#define SPI_GetExtSpiClkInterruptSourceMasked()           (SPI_INTR_SPI_EC_MASKED_REG)

#if(SPI_SCB_MODE_UNCONFIG_CONST_CFG)
    extern void SPI_SetPins(uint32 mode, uint32 subMode, uint32 uartTxRx);
#endif /* (SPI_SCB_MODE_UNCONFIG_CONST_CFG) */

void SPI_DisableTxPinsInputBuffer(void);
void SPI_EnableTxPinsInputBuffer(void);


/**********************************
*     Vars with External Linkage
**********************************/

extern cyisraddress SPI_customIntrHandler;
extern SPI_BACKUP_STRUCT SPI_backup;

#if(SPI_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common config vars */
    extern uint8 SPI_scbMode;
    extern uint8 SPI_scbEnableWake;
    extern uint8 SPI_scbEnableIntr;

    /* I2C config vars */
    extern uint8 SPI_mode;
    extern uint8 SPI_acceptAddr;

    /* SPI/UART config vars */
    extern volatile uint8 * SPI_rxBuffer;
    extern uint8   SPI_rxDataBits;
    extern uint32  SPI_rxBufferSize;

    extern volatile uint8 * SPI_txBuffer;
    extern uint8   SPI_txDataBits;
    extern uint32  SPI_txBufferSize;

    /* EZI2C config vars */
    extern uint8 SPI_numberOfAddr;
    extern uint8 SPI_subAddrSize;
#endif /* (SPI_SCB_MODE_UNCONFIG_CONST_CFG) */

#endif /* (CY_SCB_PVT_SPI_H) */


/* [] END OF FILE */
