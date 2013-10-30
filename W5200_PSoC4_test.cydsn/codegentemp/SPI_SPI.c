/*******************************************************************************
* File Name: SPI_SPI.c
* Version 1.10
*
* Description:
*  This file provides the source code to the API for the SCB Component in
*  SPI mode.
*
* Note:
*
*******************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "SPI_PVT.h"
#include "SPI_SPI_UART_PVT.h"

#if(SPI_SCB_MODE_UNCONFIG_CONST_CFG)

    /***************************************
    *  Config Structure Initialization
    ***************************************/

    const SPI_SPI_INIT_STRUCT SPI_configSpi =
    {
        SPI_SPI_MODE,
        SPI_SPI_SUB_MODE,
        SPI_SPI_CLOCK_MODE,
        SPI_SPI_OVS_FACTOR,
        SPI_SPI_MEDIAN_FILTER_ENABLE,
        SPI_SPI_LATE_MISO_SAMPLE_ENABLE,
        SPI_SPI_WAKE_ENABLE,
        SPI_SPI_RX_DATA_BITS_NUM,
        SPI_SPI_TX_DATA_BITS_NUM,
        SPI_SPI_BITS_ORDER,
        SPI_SPI_TRANSFER_SEPARATION,
        0u,
        NULL,
        0u,
        NULL,
        SPI_SCB_IRQ_INTERNAL,
        SPI_SPI_INTR_RX_MASK,
        SPI_SPI_RX_TRIGGER_LEVEL,
        SPI_SPI_INTR_TX_MASK,
        SPI_SPI_TX_TRIGGER_LEVEL
    };


    /*******************************************************************************
    * Function Name: SPI_SpiInit
    ********************************************************************************
    *
    * Summary:
    *  Configures the SCB for SPI operation.
    *
    * Parameters:
    *  config:  Pointer to a structure that contains the following ordered list of
    *           fields. These fields match the selections available in the
    *           customizer.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void SPI_SpiInit(const SPI_SPI_INIT_STRUCT *config)
    {
        if(NULL == config)
        {
            CYASSERT(0u != 0u); /* Halt execution due bad fucntion parameter */
        }
        else
        {
            /* Configure pins */
            SPI_SetPins(SPI_SCB_MODE_SPI, config->mode, SPI_DUMMY_PARAM);

            /* Store internal configuration */
            SPI_scbMode       = (uint8) SPI_SCB_MODE_SPI;
            SPI_scbEnableWake = (uint8) config->enableWake;
            SPI_scbEnableIntr = (uint8) config->enableInterrupt;

            /* Set RX direction internal variables */
            SPI_rxBuffer      =         config->rxBuffer;
            SPI_rxDataBits    = (uint8) config->rxDataBits;
            SPI_rxBufferSize  = (uint8) config->rxBufferSize;

            /* Set TX direction internal variables */
            SPI_txBuffer      =         config->txBuffer;
            SPI_txDataBits    = (uint8) config->txDataBits;
            SPI_txBufferSize  = (uint8) config->txBufferSize;


            /* Configure SPI interface */
            SPI_CTRL_REG     = SPI_GET_CTRL_OVS(config->oversample)        |
                                            SPI_GET_CTRL_EC_AM_MODE(config->enableWake) |
                                            SPI_CTRL_SPI;

            SPI_SPI_CTRL_REG = SPI_GET_SPI_CTRL_CONTINUOUS    (config->transferSeperation)  |
                                            SPI_GET_SPI_CTRL_SELECT_PRECEDE(config->submode &
                                                                          SPI_SPI_MODE_TI_PRECEDES_MASK) |
                                            SPI_GET_SPI_CTRL_SCLK_MODE     (config->sclkMode)            |
                                            SPI_GET_SPI_CTRL_LATE_MISO_SAMPLE(config->enableLateSampling)|
                                            SPI_GET_SPI_CTRL_SUB_MODE      (config->submode)             |
                                            SPI_GET_SPI_CTRL_MASTER_MODE   (config->mode);

            /* Configure RX direction */
            SPI_RX_CTRL_REG     =  SPI_GET_RX_CTRL_DATA_WIDTH(config->rxDataBits)         |
                                                SPI_GET_RX_CTRL_BIT_ORDER (config->bitOrder)           |
                                                SPI_GET_RX_CTRL_MEDIAN    (config->enableMedianFilter) |
                                                SPI_SPI_RX_CTRL;

            SPI_RX_FIFO_CTRL_REG = SPI_GET_RX_FIFO_CTRL_TRIGGER_LEVEL(config->rxTriggerLevel);

            /* Configure TX direction */
            SPI_TX_CTRL_REG      = SPI_GET_TX_CTRL_DATA_WIDTH(config->txDataBits) |
                                                SPI_GET_TX_CTRL_BIT_ORDER (config->bitOrder)   |
                                                SPI_SPI_TX_CTRL;

            SPI_TX_FIFO_CTRL_REG = SPI_GET_TX_FIFO_CTRL_TRIGGER_LEVEL(config->txTriggerLevel);

            /* Configure interrupt with SPI handler */
            SPI_SCB_IRQ_Disable();
            SPI_SCB_IRQ_SetVector(&SPI_SPI_UART_ISR);
            SPI_SCB_IRQ_SetPriority((uint8)SPI_SCB_IRQ_INTC_PRIOR_NUMBER);

            /* Configure interrupt sources */
            SPI_INTR_I2C_EC_MASK_REG = SPI_NO_INTR_SOURCES;
            SPI_INTR_SPI_EC_MASK_REG = SPI_NO_INTR_SOURCES;
            SPI_INTR_SLAVE_MASK_REG  = SPI_GET_SPI_INTR_SLAVE_MASK(config->rxInterruptMask);
            SPI_INTR_MASTER_MASK_REG = SPI_GET_SPI_INTR_MASTER_MASK(config->txInterruptMask);
            SPI_INTR_RX_MASK_REG     = SPI_GET_SPI_INTR_RX_MASK(config->rxInterruptMask);
            SPI_INTR_TX_MASK_REG     = SPI_GET_SPI_INTR_TX_MASK(config->txInterruptMask);


            /* Clear RX buffer indexes */
            SPI_rxBufferHead     = 0u;
            SPI_rxBufferTail     = 0u;
            SPI_rxBufferOverflow = 0u;

            /* Clrea TX buffer indexes */
            SPI_txBufferHead = 0u;
            SPI_txBufferTail = 0u;
        }
    }

#else

    /*******************************************************************************
    * Function Name: SPI_SpiInit
    ********************************************************************************
    *
    * Summary:
    *  Configures the SCB for SPI operation.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void SPI_SpiInit(void)
    {
        /* Configure SPI interface */
        SPI_CTRL_REG     = SPI_SPI_DEFAULT_CTRL;
        SPI_SPI_CTRL_REG = SPI_SPI_DEFAULT_SPI_CTRL;

        /* Configure TX and RX direction */
        SPI_RX_CTRL_REG      = SPI_SPI_DEFAULT_RX_CTRL;
        SPI_RX_FIFO_CTRL_REG = SPI_SPI_DEFAULT_RX_FIFO_CTRL;

        /* Configure TX and RX direction */
        SPI_TX_CTRL_REG      = SPI_SPI_DEFAULT_TX_CTRL;
        SPI_TX_FIFO_CTRL_REG = SPI_SPI_DEFAULT_TX_FIFO_CTRL;

        /* Configure interrupt with SPI handler */
        #if(SPI_SCB_IRQ_INTERNAL)
            SPI_SCB_IRQ_Disable();
            SPI_SCB_IRQ_SetVector(&SPI_SPI_UART_ISR);
            SPI_SCB_IRQ_SetPriority((uint8)SPI_SCB_IRQ_INTC_PRIOR_NUMBER);
        #endif /* (SPI_SCB_IRQ_INTERNAL) */
        
        /* Configure interrupt sources */
        SPI_INTR_I2C_EC_MASK_REG = SPI_SPI_DEFAULT_INTR_I2C_EC_MASK;
        SPI_INTR_SPI_EC_MASK_REG = SPI_SPI_DEFAULT_INTR_SPI_EC_MASK;
        SPI_INTR_SLAVE_MASK_REG  = SPI_SPI_DEFAULT_INTR_SLAVE_MASK;
        SPI_INTR_MASTER_MASK_REG = SPI_SPI_DEFAULT_INTR_MASTER_MASK;
        SPI_INTR_RX_MASK_REG     = SPI_SPI_DEFAULT_INTR_RX_MASK;
        SPI_INTR_TX_MASK_REG     = SPI_SPI_DEFAULT_INTR_TX_MASK;

        #if(SPI_INTERNAL_RX_SW_BUFFER_CONST)
            SPI_rxBufferHead     = 0u;
            SPI_rxBufferTail     = 0u;
            SPI_rxBufferOverflow = 0u;
        #endif /* (SPI_INTERNAL_RX_SW_BUFFER_CONST) */

        #if(SPI_INTERNAL_TX_SW_BUFFER_CONST)
            SPI_txBufferHead = 0u;
            SPI_txBufferTail = 0u;
        #endif /* (SPI_INTERNAL_TX_SW_BUFFER_CONST) */
    }

#endif /* (SPI_SCB_MODE_UNCONFIG_CONST_CFG) */


/*******************************************************************************
* Function Name: SPI_SetActiveSlaveSelect
********************************************************************************
*
* Summary:
*  Selects one of the four SPI slave select signals.
*  The component should be in one of the following states to change the active
*  slave select signal source correctly:
*   - the component is disabled
*   - the component has completed all transactions (TX FIFO is empty and the
*     SpiDone flag is set)
*  This function does not check that these conditions are met.
*  At initialization time the active slave select line is slave select 0.
*
* Parameters:
*  activeSelect: The four lines available to utilize Slave Select function.
*
* Return:
*  None
*
*******************************************************************************/
void SPI_SpiSetActiveSlaveSelect(uint32 activeSelect)
{
    uint32 spiCtrl;

    spiCtrl = SPI_SPI_CTRL_REG;

    spiCtrl &= (uint32) ~SPI_SPI_CTRL_SLAVE_SELECT_MASK; /* Clear SS bits */
    spiCtrl |= (uint32)  SPI_GET_SPI_CTRL_SS(activeSelect);

    SPI_SPI_CTRL_REG = spiCtrl;
}


#if(SPI_SPI_WAKE_ENABLE_CONST)
    /*******************************************************************************
    * Function Name: SPI_SpiSaveConfig
    ********************************************************************************
    *
    * Summary:
    *  Wakeup disabled: does nothing.
    *  Wakeup  enabled: clears SCB_backup.enableState to keep component enabled
    *  while DeepSleep. The SCB_INTR_SPI_EC_WAKE_UP enabled while initialization
    *  and tracks in active mode therefore it does not require to be cleared.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    * Global variables:
    *  None
    *
    *******************************************************************************/
    void SPI_SpiSaveConfig(void)
    {
        /* Clear wake-up before enable */
        SPI_ClearSpiExtClkInterruptSource(SPI_INTR_SPI_EC_WAKE_UP);

        /* Enable interrupt to wakeup the device */
        SPI_SetSpiExtClkInterruptMode(SPI_INTR_SPI_EC_WAKE_UP);
    }


    /*******************************************************************************
    * Function Name: SPI_SpiRestoreConfig
    ********************************************************************************
    *
    * Summary:
    *  Does nothing.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    * Global variables:
    *  None
    *
    *******************************************************************************/
    void SPI_SpiRestoreConfig(void)
    {
        /* Disable interrupt to wakeup the device */
        SPI_SetSpiExtClkInterruptMode(SPI_NO_INTR_SOURCES);
    }
#endif /* (SPI_SPI_WAKE_ENABLE_CONST) */


/* [] END OF FILE */
