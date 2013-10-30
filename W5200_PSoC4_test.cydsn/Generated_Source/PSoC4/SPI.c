/*******************************************************************************
* File Name: SPI.c
* Version 1.10
*
* Description:
*  This file provides the source code to the API for the SCB Component.
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

#if(SPI_SCB_MODE_I2C_INC)
    #include "SPI_I2C_PVT.h"
#endif /* (SPI_SCB_MODE_I2C_INC) */

#if(SPI_SCB_MODE_EZI2C_INC)
    #include "SPI_EZI2C_PVT.h"
#endif /* (SPI_SCB_MODE_EZI2C_INC) */

#if(SPI_SCB_MODE_SPI_INC || SPI_SCB_MODE_UART_INC)
    #include "SPI_SPI_UART_PVT.h"
#endif /* (SPI_SCB_MODE_SPI_INC || SPI_SCB_MODE_UART_INC) */


/**********************************
*    Run Time Configuration Vars
**********************************/

/* Stores internal component configuration for unconfigured mode */
#if(SPI_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common config vars */
    uint8 SPI_scbMode = SPI_SCB_MODE_UNCONFIG;
    uint8 SPI_scbEnableWake;
    uint8 SPI_scbEnableIntr;

    /* I2C config vars */
    uint8 SPI_mode;
    uint8 SPI_acceptAddr;

    /* SPI/UART config vars */
    volatile uint8 * SPI_rxBuffer;
    uint8  SPI_rxDataBits;
    uint32 SPI_rxBufferSize;

    volatile uint8 * SPI_txBuffer;
    uint8  SPI_txDataBits;
    uint32 SPI_txBufferSize;

    /* EZI2C config vars */
    uint8 SPI_numberOfAddr;
    uint8 SPI_subAddrSize;
#endif /* (SPI_SCB_MODE_UNCONFIG_CONST_CFG) */


/**********************************
*     Common SCB Vars
**********************************/

uint8 SPI_initVar = 0u;
cyisraddress SPI_customIntrHandler = NULL;


/***************************************
*    Private Function Prototypes
***************************************/

static void SPI_ScbEnableIntr(void);
static void SPI_ScbModeStop(void);


/*******************************************************************************
* Function Name: SPI_Init
********************************************************************************
*
* Summary:
*  Initializes SCB component to operate in one of selected configurations:
*  I2C, SPI, UART, EZI2C or EZSPI.
*  This function does not do any initialization when configuration is set to
*  Unconfigured SCB.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void SPI_Init(void)
{
#if(SPI_SCB_MODE_UNCONFIG_CONST_CFG)
    if(SPI_SCB_MODE_UNCONFIG_RUNTM_CFG)
    {
        SPI_initVar = 0u; /* Clear init var */
    }
    else
    {
        /* Initialization was done before call */
    }

#elif(SPI_SCB_MODE_I2C_CONST_CFG)
    SPI_I2CInit();

#elif(SPI_SCB_MODE_SPI_CONST_CFG)
    SPI_SpiInit();

#elif(SPI_SCB_MODE_UART_CONST_CFG)
    SPI_UartInit();

#elif(SPI_SCB_MODE_EZI2C_CONST_CFG)
    SPI_EzI2CInit();

#endif /* (SPI_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: SPI_Enable
********************************************************************************
*
* Summary:
*  Enables SCB component operation.
*  The SCB configuration should be not changed when the component is enabled.
*  Any configuration changes should be made after disabling the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void SPI_Enable(void)
{
#if(SPI_SCB_MODE_UNCONFIG_CONST_CFG)
    if(!SPI_SCB_MODE_UNCONFIG_RUNTM_CFG)
    {
        /* Enable SCB block, only if already configured */
        SPI_CTRL_REG |= SPI_CTRL_ENABLED;
        
        /* Enable interrupt */
        SPI_ScbEnableIntr();
    }
#else
    SPI_CTRL_REG |= SPI_CTRL_ENABLED; /* Enable SCB block */
    
    SPI_ScbEnableIntr();
#endif /* (SPI_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: SPI_Start
********************************************************************************
*
* Summary:
*  Invokes SCB_Init() and SCB_Enable().
*  After this function call the component is enabled and ready for operation.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  SPI_initVar - used to check initial configuration, modified
*  on first function call.
*
*******************************************************************************/
void SPI_Start(void)
{
    if(0u == SPI_initVar)
    {
        SPI_initVar = 1u; /* Component was initialized */
        SPI_Init();       /* Initialize component      */
    }

    SPI_Enable();
}


/*******************************************************************************
* Function Name: SPI_Stop
********************************************************************************
*
* Summary:
*  Disables the SCB component.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void SPI_Stop(void)
{
#if(SPI_SCB_IRQ_INTERNAL)
    SPI_SCB_IRQ_Disable();     /* Disable interrupt before block */
#endif /* (SPI_SCB_IRQ_INTERNAL) */

    SPI_CTRL_REG &= (uint32) ~SPI_CTRL_ENABLED;  /* Disable SCB block */

#if(SPI_SCB_IRQ_INTERNAL)
    SPI_SCB_IRQ_ClearPending(); /* Clear pending interrupt */
#endif /* (SPI_SCB_IRQ_INTERNAL) */
    
    SPI_ScbModeStop(); /* Calls scbMode specific Stop function */
}


/*******************************************************************************
* Function Name: SPI_SetCustomInterruptHandler
********************************************************************************
*
* Summary:
*  Registers a function to be called by the internal interrupt handler.
*  First the function that is registered is called, then the internal interrupt
*  handler performs any operations such as software buffer management functions
*  before the interrupt returns.  It is user's responsibility to not break the
*  software buffer operations. Only one custom handler is supported, which is
*  the function provided by the most recent call.
*  At initialization time no custom handler is registered.
*
* Parameters:
*  func: Pointer to the function to register.
*        The value NULL indicates to remove the current custom interrupt
*        handler.
*
* Return:
*  None
*
*******************************************************************************/
void SPI_SetCustomInterruptHandler(cyisraddress func)
{
    SPI_customIntrHandler = func; /* Register interrupt handler */
}


/*******************************************************************************
* Function Name: SPI_ScbModeEnableIntr
********************************************************************************
*
* Summary:
*  Enables interrupt for specific mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
static void SPI_ScbEnableIntr(void)
{
#if(SPI_SCB_IRQ_INTERNAL)
    #if(SPI_SCB_MODE_UNCONFIG_CONST_CFG)
        /* Enable interrupt source */
        if(0u != SPI_scbEnableIntr)
        {
            SPI_SCB_IRQ_Enable();
        }
    #else
        SPI_SCB_IRQ_Enable();
        
    #endif /* (SPI_SCB_MODE_UNCONFIG_CONST_CFG) */
#endif /* (SPI_SCB_IRQ_INTERNAL) */
}


/*******************************************************************************
* Function Name: SPI_ScbModeEnableIntr
********************************************************************************
*
* Summary:
*  Calls Stop function for specific operation mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
static void SPI_ScbModeStop(void)
{
#if(SPI_SCB_MODE_UNCONFIG_CONST_CFG)
    if(SPI_SCB_MODE_I2C_RUNTM_CFG)
    {
        SPI_I2CStop();
    }
    else if(SPI_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        SPI_EzI2CStop();
    }
    else
    {
        /* None of modes above */
    }
#elif(SPI_SCB_MODE_I2C_CONST_CFG)
    SPI_I2CStop();

#elif(SPI_SCB_MODE_EZI2C_CONST_CFG)
    SPI_EzI2CStop();

#endif /* (SPI_SCB_MODE_UNCONFIG_CONST_CFG) */
}


#if(SPI_SCB_MODE_UNCONFIG_CONST_CFG)
    /*******************************************************************************
    * Function Name: SPI_SetPins
    ********************************************************************************
    *
    * Summary:
    *  Sets pins settings accordingly to selected operation mode.
    *  Only available in Unconfigured operation mode. The mode specific
    *  initialization function calls it.
    *  Pins configuration is set by PSoC Creator when specific mode of operation
    *  selected in design time.
    *
    * Parameters:
    *  mode:      Mode of SCB operation.
    *  subMode:   Submode of SCB operation. It is only required for SPI and UART
    *             modes.
    *  uartTxRx:  Direction for UART.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void SPI_SetPins(uint32 mode, uint32 subMode, uint32 uartTxRx)
    {
        uint32 hsiomSel[SPI_SCB_PINS_NUMBER];
        uint32 pinsDm  [SPI_SCB_PINS_NUMBER];
        uint32 i;

        /* Make all unused */
        for(i = 0u; i < SPI_SCB_PINS_NUMBER; i++)
        {
            hsiomSel[i] = SPI_HSIOM_DEF_SEL;
            pinsDm[i]   = SPI_PIN_DM_ALG_HIZ;
        }

        /* Choice the Dm and HSIOM */
        if((SPI_SCB_MODE_I2C   == mode) ||
           (SPI_SCB_MODE_EZI2C == mode))
        {
            hsiomSel[SPI_MOSI_SCL_RX_PIN_INDEX] = SPI_HSIOM_I2C_SEL;
            hsiomSel[SPI_MISO_SDA_TX_PIN_INDEX] = SPI_HSIOM_I2C_SEL;

            pinsDm[SPI_MOSI_SCL_RX_PIN_INDEX] = SPI_PIN_DM_OD_LO;
            pinsDm[SPI_MISO_SDA_TX_PIN_INDEX] = SPI_PIN_DM_OD_LO;
        }
        else if(SPI_SCB_MODE_SPI == mode)
        {
            hsiomSel[SPI_MOSI_SCL_RX_PIN_INDEX] = SPI_HSIOM_SPI_SEL;
            hsiomSel[SPI_MISO_SDA_TX_PIN_INDEX] = SPI_HSIOM_SPI_SEL;
            hsiomSel[SPI_SCLK_PIN_INDEX]        = SPI_HSIOM_SPI_SEL;

            if(SPI_SPI_SLAVE == subMode)
            {
                /* Slave */
                pinsDm[SPI_MOSI_SCL_RX_PIN_INDEX] = SPI_PIN_DM_DIG_HIZ;
                pinsDm[SPI_MISO_SDA_TX_PIN_INDEX] = SPI_PIN_DM_STRONG;
                pinsDm[SPI_SCLK_PIN_INDEX]        = SPI_PIN_DM_DIG_HIZ;

            #if(SPI_SS0_PIN)
                /* Only SS0 is valid choice for Slave */
                hsiomSel[SPI_SS0_PIN_INDEX] = SPI_HSIOM_SPI_SEL;
                pinsDm  [SPI_SS0_PIN_INDEX] = SPI_PIN_DM_DIG_HIZ;
            #endif /* (SPI_SS1_PIN) */
            }
            else /* (Master) */
            {
                pinsDm[SPI_MOSI_SCL_RX_PIN_INDEX] = SPI_PIN_DM_STRONG;
                pinsDm[SPI_MISO_SDA_TX_PIN_INDEX] = SPI_PIN_DM_DIG_HIZ;
                pinsDm[SPI_SCLK_PIN_INDEX]        = SPI_PIN_DM_STRONG;

            #if(SPI_SS0_PIN)
                hsiomSel[SPI_SS0_PIN_INDEX] = SPI_HSIOM_SPI_SEL;
                pinsDm  [SPI_SS0_PIN_INDEX] = SPI_PIN_DM_STRONG;
            #endif /* (SPI_SS0_PIN) */

            #if(SPI_SS1_PIN)
                hsiomSel[SPI_SS1_PIN_INDEX] = SPI_HSIOM_SPI_SEL;
                pinsDm  [SPI_SS1_PIN_INDEX] = SPI_PIN_DM_STRONG;
            #endif /* (SPI_SS1_PIN) */

            #if(SPI_SS2_PIN)
                hsiomSel[SPI_SS2_PIN_INDEX] = SPI_HSIOM_SPI_SEL;
                pinsDm  [SPI_SS2_PIN_INDEX] = SPI_PIN_DM_STRONG;
            #endif /* (SPI_SS2_PIN) */

            #if(SPI_SS3_PIN)
                hsiomSel[SPI_SS3_PIN_INDEX] = SPI_HSIOM_SPI_SEL;
                pinsDm  [SPI_SS3_PIN_INDEX] = SPI_PIN_DM_STRONG;
            #endif /* (SPI_SS2_PIN) */
            }
        }
        else /* UART */
        {
            if(SPI_UART_MODE_SMARTCARD == subMode)
            {
                /* SmartCard */
                hsiomSel[SPI_MISO_SDA_TX_PIN_INDEX] = SPI_HSIOM_UART_SEL;
                pinsDm  [SPI_MISO_SDA_TX_PIN_INDEX] = SPI_PIN_DM_OD_LO;
            }
            else /* Standard or IrDA */
            {
                if(0u != (SPI_UART_RX & uartTxRx))
                {
                    hsiomSel[SPI_MOSI_SCL_RX_PIN_INDEX] = SPI_HSIOM_UART_SEL;
                    pinsDm  [SPI_MOSI_SCL_RX_PIN_INDEX] = SPI_PIN_DM_DIG_HIZ;
                }

                if(0u != (SPI_UART_TX & uartTxRx))
                {
                    hsiomSel[SPI_MISO_SDA_TX_PIN_INDEX] = SPI_HSIOM_UART_SEL;
                    pinsDm  [SPI_MISO_SDA_TX_PIN_INDEX] = SPI_PIN_DM_STRONG;
                }
            }
        }

        /* Condfigure pins: set HSIOM and DM */
        /* Condfigure pins: DR registers configuration remains unchanged for cyfitter_cfg() */

    #if(SPI_MOSI_SCL_RX_PIN)
        SPI_SET_HSIOM_SEL(SPI_MOSI_SCL_RX_HSIOM_REG,
                                       SPI_MOSI_SCL_RX_HSIOM_MASK,
                                       SPI_MOSI_SCL_RX_HSIOM_POS,
                                       hsiomSel[SPI_MOSI_SCL_RX_PIN_INDEX]);
    #endif /* (SPI_MOSI_SCL_RX_PIN) */

    #if(SPI_MOSI_SCL_RX_WAKE_PIN)
        SPI_SET_HSIOM_SEL(SPI_MOSI_SCL_RX_WAKE_HSIOM_REG,
                                       SPI_MOSI_SCL_RX_WAKE_HSIOM_MASK,
                                       SPI_MOSI_SCL_RX_WAKE_HSIOM_POS,
                                       hsiomSel[SPI_MOSI_SCL_RX_WAKE_PIN_INDEX]);
    #endif /* (SPI_MOSI_SCL_RX_WAKE_PIN) */

    #if(SPI_MISO_SDA_TX_PIN)
        SPI_SET_HSIOM_SEL(SPI_MISO_SDA_TX_HSIOM_REG,
                                       SPI_MISO_SDA_TX_HSIOM_MASK,
                                       SPI_MISO_SDA_TX_HSIOM_POS,
                                       hsiomSel[SPI_MISO_SDA_TX_PIN_INDEX]);
    #endif /* (SPI_MOSI_SCL_RX_PIN) */

    #if(SPI_SCLK_PIN)
        SPI_SET_HSIOM_SEL(SPI_SCLK_HSIOM_REG, SPI_SCLK_HSIOM_MASK,
                                       SPI_SCLK_HSIOM_POS, hsiomSel[SPI_SCLK_PIN_INDEX]);
    #endif /* (SPI_SCLK_PIN) */

    #if(SPI_SS0_PIN)
        SPI_SET_HSIOM_SEL(SPI_SS0_HSIOM_REG, SPI_SS0_HSIOM_MASK,
                                       SPI_SS0_HSIOM_POS, hsiomSel[SPI_SS0_PIN_INDEX]);
    #endif /* (SPI_SS1_PIN) */

    #if(SPI_SS1_PIN)
        SPI_SET_HSIOM_SEL(SPI_SS1_HSIOM_REG, SPI_SS1_HSIOM_MASK,
                                       SPI_SS1_HSIOM_POS, hsiomSel[SPI_SS1_PIN_INDEX]);
    #endif /* (SPI_SS1_PIN) */

    #if(SPI_SS2_PIN)
        SPI_SET_HSIOM_SEL(SPI_SS2_HSIOM_REG, SPI_SS2_HSIOM_MASK,
                                       SPI_SS2_HSIOM_POS, hsiomSel[SPI_SS2_PIN_INDEX]);
    #endif /* (SPI_SS2_PIN) */

    #if(SPI_SS3_PIN)
        SPI_SET_HSIOM_SEL(SPI_SS3_HSIOM_REG,  SPI_SS3_HSIOM_MASK,
                                       SPI_SS3_HSIOM_POS, hsiomSel[SPI_SS3_PIN_INDEX]);
    #endif /* (SPI_SS3_PIN) */



    #if(SPI_MOSI_SCL_RX_PIN)
        SPI_spi_mosi_i2c_scl_uart_rx_SetDriveMode((uint8)
                                                               pinsDm[SPI_MOSI_SCL_RX_PIN_INDEX]);
    #endif /* (SPI_MOSI_SCL_RX_PIN) */

    #if(SPI_MOSI_SCL_RX_WAKE_PIN)
    SPI_spi_mosi_i2c_scl_uart_rx_wake_SetDriveMode((uint8)
                                                               pinsDm[SPI_MOSI_SCL_RX_WAKE_PIN_INDEX]);

    /* Set interrupt on rising edge */
    SPI_SET_INCFG_TYPE(SPI_MOSI_SCL_RX_WAKE_INTCFG_REG,
                                    SPI_MOSI_SCL_RX_WAKE_INTCFG_TYPE_MASK,
                                    SPI_MOSI_SCL_RX_WAKE_INTCFG_TYPE_POS,
                                    SPI_INTCFG_TYPE_FALLING_EDGE);

    #endif /* (SPI_MOSI_SCL_RX_WAKE_PIN) */

    #if(SPI_MISO_SDA_TX_PIN)
        SPI_spi_miso_i2c_sda_uart_tx_SetDriveMode((uint8)
                                                                    pinsDm[SPI_MISO_SDA_TX_PIN_INDEX]);
    #endif /* (SPI_MOSI_SCL_RX_PIN) */

    #if(SPI_SCLK_PIN)
        SPI_spi_sclk_SetDriveMode((uint8) pinsDm[SPI_SCLK_PIN_INDEX]);
    #endif /* (SPI_SCLK_PIN) */

    #if(SPI_SS0_PIN)
        SPI_spi_ss0_SetDriveMode((uint8) pinsDm[SPI_SS0_PIN_INDEX]);
    #endif /* (SPI_SS0_PIN) */

    #if(SPI_SS1_PIN)
        SPI_spi_ss1_SetDriveMode((uint8) pinsDm[SPI_SS1_PIN_INDEX]);
    #endif /* (SPI_SS1_PIN) */

    #if(SPI_SS2_PIN)
        SPI_spi_ss2_SetDriveMode((uint8) pinsDm[SPI_SS2_PIN_INDEX]);
    #endif /* (SPI_SS2_PIN) */

    #if(SPI_SS3_PIN)
        SPI_spi_ss3_SetDriveMode((uint8) pinsDm[SPI_SS3_PIN_INDEX]);
    #endif /* (SPI_SS3_PIN) */
    }

#endif /* (SPI_SCB_MODE_UNCONFIG_CONST_CFG) */


/* [] END OF FILE */
