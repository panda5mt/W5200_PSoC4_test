/*******************************************************************************
* File Name: SPI_PM.c
* Version 1.10
*
* Description:
*  This file provides the source code to the Power Management support for
*  the SCB Component.
*
* Note:
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "SPI.h"
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


/***************************************
*   Backup Structure declaration
***************************************/

SPI_BACKUP_STRUCT SPI_backup =
{
    0u, /* enableState */
};


/*******************************************************************************
* Function Name: SPI_Sleep
********************************************************************************
*
* Summary:
*  Calls SaveConfig function fucntion for selected mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void SPI_Sleep(void)
{
#if(SPI_SCB_MODE_UNCONFIG_CONST_CFG)

    if(0u != SPI_scbEnableWake)
    {
        if(SPI_SCB_MODE_I2C_RUNTM_CFG)
        {
            SPI_I2CSaveConfig();
        }
        else if(SPI_SCB_MODE_SPI_RUNTM_CFG)
        {
            SPI_SpiSaveConfig();
        }
        else if(SPI_SCB_MODE_UART_RUNTM_CFG)
        {
            SPI_UartSaveConfig();
        }
        else if(SPI_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            SPI_EzI2CSaveConfig();
        }
        else
        {
            /* Unknown mode: do nothing */
        }
    }
    else
    {
        SPI_backup.enableState = (uint8) SPI_GET_CTRL_ENABLED;
        
        if(0u != SPI_backup.enableState)
        {
            SPI_Stop();
        }
    }
    
    SPI_DisableTxPinsInputBuffer();
    
#else
    
    #if defined (SPI_I2C_WAKE_ENABLE_CONST) && (SPI_I2C_WAKE_ENABLE_CONST)
        SPI_I2CSaveConfig();
        
    #elif defined (SPI_SPI_WAKE_ENABLE_CONST) && (SPI_SPI_WAKE_ENABLE_CONST)
        SPI_SpiSaveConfig();
        
    #elif defined (SPI_UART_WAKE_ENABLE_CONST) && (SPI_UART_WAKE_ENABLE_CONST)
        SPI_UartSaveConfig();
        
    #elif defined (SPI_EZI2C_WAKE_ENABLE_CONST) && (SPI_EZI2C_WAKE_ENABLE_CONST)
        SPI_EzI2CSaveConfig();
    
    #else
        
        SPI_backup.enableState = (uint8) SPI_GET_CTRL_ENABLED;
        
        /* Check enable state */
        if(0u != SPI_backup.enableState)
        {
            SPI_Stop();
        }
        
    #endif /* defined (SPI_SCB_MODE_I2C_CONST_CFG) && (SPI_I2C_WAKE_ENABLE_CONST) */
    
    SPI_DisableTxPinsInputBuffer();
    
#endif /* (SPI_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: SPI_Wakeup
********************************************************************************
*
* Summary:
*  Calls RestoreConfig function fucntion for selected mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void SPI_Wakeup(void)
{
#if(SPI_SCB_MODE_UNCONFIG_CONST_CFG)

    SPI_EnableTxPinsInputBuffer();
        
    if(0u != SPI_scbEnableWake)
    {
        if(SPI_SCB_MODE_I2C_RUNTM_CFG)
        {
            SPI_I2CRestoreConfig();
        }
        else if(SPI_SCB_MODE_SPI_RUNTM_CFG)
        {
            SPI_SpiRestoreConfig();
        }
        else if(SPI_SCB_MODE_UART_RUNTM_CFG)
        {
            SPI_UartRestoreConfig();
        }
        else if(SPI_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            SPI_EzI2CRestoreConfig();
        }
        else
        {
            /* Unknown mode: do nothing */
        }
    }
    else
    {    
        /* Restore enable state */
        if(0u != SPI_backup.enableState)
        {
            SPI_Enable();
        }
    }

#else
    
    SPI_EnableTxPinsInputBuffer();
        
    #if defined (SPI_I2C_WAKE_ENABLE_CONST) && (SPI_I2C_WAKE_ENABLE_CONST)
        SPI_I2CRestoreConfig();
        
    #elif defined (SPI_SPI_WAKE_ENABLE_CONST) && (SPI_SPI_WAKE_ENABLE_CONST)
        SPI_SpiRestoreConfig();
        
    #elif defined (SPI_UART_WAKE_ENABLE_CONST) && (SPI_UART_WAKE_ENABLE_CONST)
        SPI_UartRestoreConfig();
        
    #elif defined (SPI_EZI2C_WAKE_ENABLE_CONST) && (SPI_EZI2C_WAKE_ENABLE_CONST)
        SPI_EzI2CRestoreConfig();
    
    #else
        /* Check enable state */
        if(0u != SPI_backup.enableState)
        {
            SPI_Enable();
        }
        
    #endif /* (SPI_I2C_WAKE_ENABLE_CONST) */

#endif /* (SPI_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: SPI_DisableTxPinsInputBuffer
********************************************************************************
*
* Summary:
*  Disables input buffers for TX pins. This action removes leakage current while
*  low power mode (Cypress ID 149635).
*   SCB mode is I2C and EZI2C: bus is pulled-up. Leave pins as it is.
*   SCB mode SPI:
*     Slave  - disable input buffer for MISO pin.
*     Master - disable input buffer for all pins.
*   SCB mode SmartCard: 
*     Standard and IrDA - disable input buffer for TX pin.
*     SmartCard - RX_TX pin is pulled-up. Leave pin as it is.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void SPI_DisableTxPinsInputBuffer(void)
{
#if(SPI_SCB_MODE_UNCONFIG_CONST_CFG)
    
    /* SCB mode is I2C and EZI2C: bus is pulled-up. Does nothing */
       
    if(SPI_SCB_MODE_SPI_RUNTM_CFG)
    {
        if(0u != (SPI_SPI_CTRL_REG & SPI_SPI_CTRL_MASTER))
        /* SPI Master */
        {
        #if(SPI_MOSI_SCL_RX_WAKE_PIN)
            SPI_spi_mosi_i2c_scl_uart_rx_wake_INP_DIS |= \
                                                                SPI_spi_mosi_i2c_scl_uart_rx_wake_MASK;
        #endif /* (SPI_MOSI_SCL_RX_WAKE_PIN) */

        #if(SPI_MOSI_SCL_RX_PIN)
            SPI_spi_mosi_i2c_scl_uart_rx_INP_DIS |= SPI_spi_mosi_i2c_scl_uart_rx_MASK;
        #endif /* (SPI_MOSI_SCL_RX_PIN) */

        #if(SPI_MISO_SDA_TX_PIN)
            SPI_spi_miso_i2c_sda_uart_tx_INP_DIS |= SPI_spi_miso_i2c_sda_uart_tx_MASK;
        #endif /* (SPI_MISO_SDA_TX_PIN_PIN) */

        #if(SPI_SCLK_PIN)
            SPI_spi_sclk_INP_DIS |= SPI_spi_sclk_MASK;
        #endif /* (SPI_SCLK_PIN) */

        #if(SPI_SS0_PIN)
            SPI_spi_ss0_INP_DIS |= SPI_spi_ss0_MASK;
        #endif /* (SPI_SS1_PIN) */

        #if(SPI_SS1_PIN)
            SPI_spi_ss1_INP_DIS |= SPI_spi_ss1_MASK;
        #endif /* (SPI_SS1_PIN) */

        #if(SPI_SS2_PIN)
            SPI_spi_ss2_INP_DIS |= SPI_spi_ss2_MASK;
        #endif /* (SPI_SS2_PIN) */

        #if(SPI_SS3_PIN)
            SPI_spi_ss3_INP_DIS |= SPI_spi_ss3_MASK;
        #endif /* (SPI_SS3_PIN) */
        }
        else
        /* SPI Slave */
        {
        #if(SPI_MISO_SDA_TX_PIN)
            SPI_spi_miso_i2c_sda_uart_tx_INP_DIS |= SPI_spi_miso_i2c_sda_uart_tx_MASK;
        #endif /* (SPI_MISO_SDA_TX_PIN_PIN) */
        }
    }
    else if (SPI_SCB_MODE_UART_RUNTM_CFG)
    {
        if(SPI_UART_CTRL_MODE_UART_SMARTCARD != 
            (SPI_UART_CTRL_REG & SPI_UART_CTRL_MODE_MASK))
        /* UART Standard or IrDA */
        {
        #if(SPI_MISO_SDA_TX_PIN)
            SPI_spi_miso_i2c_sda_uart_tx_INP_DIS |= SPI_spi_miso_i2c_sda_uart_tx_MASK;
        #endif /* (SPI_MISO_SDA_TX_PIN_PIN) */
        }
    }
    else
    {
        /* Does nothing */
    }
    
#else
    
    /* SCB mode is I2C and EZI2C: bus is pulled-up. Does nothing */
        
    /* SCB mode is SPI Master */
    #if(SPI_SPI_MASTER_PINS)
        SPI_sclk_m_INP_DIS |= SPI_sclk_m_MASK;
        SPI_mosi_m_INP_DIS |= SPI_mosi_m_MASK;
        SPI_miso_m_INP_DIS |= SPI_miso_m_MASK;
    #endif /* (SPI_SPI_MASTER_PINS) */

    #if(SPI_SPI_MASTER_SS0_PIN)
        SPI_ss0_m_INP_DIS |= SPI_ss0_m_MASK;
    #endif /* (SPI_SPI_MASTER_SS0_PIN) */

    #if(SPI_SPI_MASTER_SS1_PIN)
        SPI_ss1_m_INP_DIS |= SPI_ss1_m_MASK;
    #endif /* (SPI_SPI_MASTER_SS1_PIN) */

    #if(SPI_SPI_MASTER_SS2_PIN)
        SPI_ss2_m_INP_DIS |= SPI_ss2_m_MASK;
    #endif /* (SPI_SPI_MASTER_SS2_PIN) */

    #if(SPI_SPI_MASTER_SS3_PIN)
        SPI_ss3_m_INP_DIS |= SPI_ss3_m_MASK;
    #endif /* (SPI_SPI_MASTER_SS3_PIN) */
    
    /* SCB mode is SPI Slave */
    #if(SPI_SPI_SLAVE_PINS)
        SPI_miso_s_INP_DIS |= SPI_miso_s_MASK;
    #endif /* (SPI_SPI_SLAVE_PINS) */

    /* SCB mode is UART */
    #if(SPI_UART_TX_PIN)
        SPI_tx_INP_DIS |= SPI_tx_MASK;
    #endif /* (SPI_UART_TX_PIN) */

#endif /* (SPI_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: SPI_EnableTxPinsInputBuffer
********************************************************************************
*
* Summary:
*  Enables input buffers for TX pins. Restore changes done byte
*SPI_DisableTxPinsInputBuffer.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void SPI_EnableTxPinsInputBuffer(void)
{
#if(SPI_SCB_MODE_UNCONFIG_CONST_CFG)
    if(SPI_SCB_MODE_SPI_RUNTM_CFG)
    {
        if(0u != (SPI_SPI_CTRL_REG & SPI_SPI_CTRL_MASTER))
        /* SPI Master */
        {
        #if(SPI_MOSI_SCL_RX_WAKE_PIN)
            SPI_spi_mosi_i2c_scl_uart_rx_wake_INP_DIS &= \
                                            (uint32) ~((uint32) SPI_spi_mosi_i2c_scl_uart_rx_wake_MASK);
        #endif /* (SPI_MOSI_SCL_RX_WAKE_PIN) */

        #if(SPI_MOSI_SCL_RX_PIN)
            SPI_spi_mosi_i2c_scl_uart_rx_INP_DIS &= \
                                            (uint32) ~((uint32) SPI_spi_mosi_i2c_scl_uart_rx_MASK);
        #endif /* (SPI_MOSI_SCL_RX_PIN) */

        #if(SPI_MISO_SDA_TX_PIN)
            SPI_spi_miso_i2c_sda_uart_tx_INP_DIS &= \
                                            (uint32) ~((uint32) SPI_spi_miso_i2c_sda_uart_tx_MASK);
        #endif /* (SPI_MISO_SDA_TX_PIN_PIN) */

        #if(SPI_SCLK_PIN)
            SPI_spi_sclk_INP_DIS &= (uint32) ~((uint32) SPI_spi_sclk_MASK);
        #endif /* (SPI_SCLK_PIN) */

        #if(SPI_SS0_PIN)
            SPI_spi_ss0_INP_DIS &= (uint32) ~((uint32) SPI_spi_ss0_MASK);
        #endif /* (SPI_SS1_PIN) */

        #if(SPI_SS1_PIN)
            SPI_spi_ss1_INP_DIS &= (uint32) ~((uint32) SPI_spi_ss1_MASK);
        #endif /* (SPI_SS1_PIN) */

        #if(SPI_SS2_PIN)
            SPI_spi_ss2_INP_DIS &= (uint32) ~((uint32) SPI_spi_ss2_MASK);
        #endif /* (SPI_SS2_PIN) */

        #if(SPI_SS3_PIN)
            SPI_spi_ss3_INP_DIS &= (uint32) ~((uint32) SPI_spi_ss3_MASK);
        #endif /* (SPI_SS3_PIN) */
        }
        else
        /* SPI Slave */
        {
        #if(SPI_MISO_SDA_TX_PIN)
            SPI_spi_miso_i2c_sda_uart_tx_INP_DIS &= \
                                                (uint32) ~((uint32) SPI_spi_miso_i2c_sda_uart_tx_MASK);
        #endif /* (SPI_MISO_SDA_TX_PIN_PIN) */
        }
    }
    else if (SPI_SCB_MODE_UART_RUNTM_CFG)
    {
        if(SPI_UART_CTRL_MODE_UART_SMARTCARD != 
                (SPI_UART_CTRL_REG & SPI_UART_CTRL_MODE_MASK))
        /* UART Standard or IrDA */
        {
        #if(SPI_MISO_SDA_TX_PIN)
            SPI_spi_miso_i2c_sda_uart_tx_INP_DIS &= \
                                                (uint32) ~((uint32) SPI_spi_miso_i2c_sda_uart_tx_MASK);
        #endif /* (SPI_MISO_SDA_TX_PIN_PIN) */
        }
    }
    else
    {
        /* Does nothing */
    }
    
#else
        
    /* SCB mode is SPI Master */
    #if(SPI_SPI_MASTER_PINS)
        SPI_sclk_m_INP_DIS &= (uint32) ~((uint32) SPI_sclk_m_MASK);
        SPI_mosi_m_INP_DIS &= (uint32) ~((uint32) SPI_mosi_m_MASK);
        SPI_miso_m_INP_DIS &= (uint32) ~((uint32) SPI_miso_m_MASK);
    #endif /* (SPI_SPI_MASTER_PINS) */

    #if(SPI_SPI_MASTER_SS0_PIN)
        SPI_ss0_m_INP_DIS &= (uint32) ~((uint32) SPI_ss0_m_MASK);
    #endif /* (SPI_SPI_MASTER_SS0_PIN) */

    #if(SPI_SPI_MASTER_SS1_PIN)
        SPI_ss1_m_INP_DIS &= (uint32) ~((uint32) SPI_ss1_m_MASK);
    #endif /* (SPI_SPI_MASTER_SS1_PIN) */

    #if(SPI_SPI_MASTER_SS2_PIN)
        SPI_ss2_m_INP_DIS &= (uint32) ~((uint32) SPI_ss2_m_MASK);
    #endif /* (SPI_SPI_MASTER_SS2_PIN) */

    #if(SPI_SPI_MASTER_SS3_PIN)
        SPI_ss3_m_INP_DIS &= (uint32) ~((uint32) SPI_ss3_m_MASK);
    #endif /* (SPI_SPI_MASTER_SS3_PIN) */
    
    /* SCB mode is SPI Slave */
    #if(SPI_SPI_SLAVE_PINS)
        SPI_miso_s_INP_DIS &= (uint32) ~((uint32) SPI_miso_s_MASK);
    #endif /* (SPI_SPI_SLAVE_PINS) */

    /* SCB mode is UART */
    #if(SPI_UART_TX_PIN)
        SPI_tx_INP_DIS &= (uint32) ~((uint32) SPI_tx_MASK);
    #endif /* (SPI_UART_TX_PIN) */

#endif /* (SPI_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/* [] END OF FILE */
