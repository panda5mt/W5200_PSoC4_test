/*******************************************************************************
* File Name: SPI_PINS.h
* Version 1.10
*
* Description:
*  This file provides constants and parameter values for the pin components
*  buried into SCB Component.
*
* Note:
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_PINS_SPI_H)
#define CY_SCB_PINS_SPI_H

#include "cydevice_trm.h"
#include "cyfitter.h"
#include "cytypes.h"


/***************************************
*   Conditional Compilation Parameters
****************************************/

/* Unconfigured pins */
#define SPI_REMOVE_MOSI_SCL_RX_WAKE_PIN    (1u)
#define SPI_REMOVE_MOSI_SCL_RX_PIN         (1u)
#define SPI_REMOVE_MISO_SDA_TX_PIN         (1u)
#define SPI_REMOVE_SCLK_PIN                (1u)
#define SPI_REMOVE_SS0_PIN                 (1u)
#define SPI_REMOVE_SS1_PIN                 (1u)
#define SPI_REMOVE_SS2_PIN                 (1u)
#define SPI_REMOVE_SS3_PIN                 (1u)

/* Mode defined pins */
#define SPI_REMOVE_I2C_PINS                (1u)
#define SPI_REMOVE_SPI_MASTER_PINS         (0u)
#define SPI_REMOVE_SPI_SLAVE_PINS          (1u)
#define SPI_REMOVE_SPI_MASTER_SS0_PIN      (0u)
#define SPI_REMOVE_SPI_MASTER_SS1_PIN      (1u)
#define SPI_REMOVE_SPI_MASTER_SS2_PIN      (1u)
#define SPI_REMOVE_SPI_MASTER_SS3_PIN      (1u)
#define SPI_REMOVE_UART_TX_PIN             (1u)
#define SPI_REMOVE_UART_RX_TX_PIN          (1u)
#define SPI_REMOVE_UART_RX_PIN             (1u)
#define SPI_REMOVE_UART_RX_WAKE_PIN        (1u)

/* Unconfigured pins */
#define SPI_MOSI_SCL_RX_WAKE_PIN   (0u == SPI_REMOVE_MOSI_SCL_RX_WAKE_PIN)
#define SPI_MOSI_SCL_RX_PIN        (0u == SPI_REMOVE_MOSI_SCL_RX_PIN)
#define SPI_MISO_SDA_TX_PIN        (0u == SPI_REMOVE_MISO_SDA_TX_PIN)
#define SPI_SCLK_PIN               (0u == SPI_REMOVE_SCLK_PIN)
#define SPI_SS0_PIN                (0u == SPI_REMOVE_SS0_PIN)
#define SPI_SS1_PIN                (0u == SPI_REMOVE_SS1_PIN)
#define SPI_SS2_PIN                (0u == SPI_REMOVE_SS2_PIN)
#define SPI_SS3_PIN                (0u == SPI_REMOVE_SS3_PIN)

/* Mode defined pins */
#define SPI_I2C_PINS               (0u == SPI_REMOVE_I2C_PINS)
#define SPI_SPI_MASTER_PINS        (0u == SPI_REMOVE_SPI_MASTER_PINS) 
#define SPI_SPI_SLAVE_PINS         (0u == SPI_REMOVE_SPI_SLAVE_PINS)
#define SPI_SPI_MASTER_SS0_PIN     (0u == SPI_REMOVE_SPI_MASTER_SS0_PIN)
#define SPI_SPI_MASTER_SS1_PIN     (0u == SPI_REMOVE_SPI_MASTER_SS1_PIN)
#define SPI_SPI_MASTER_SS2_PIN     (0u == SPI_REMOVE_SPI_MASTER_SS2_PIN)
#define SPI_SPI_MASTER_SS3_PIN     (0u == SPI_REMOVE_SPI_MASTER_SS3_PIN)
#define SPI_UART_TX_PIN            (0u == SPI_REMOVE_UART_TX_PIN)
#define SPI_UART_RX_TX_PIN         (0u == SPI_REMOVE_UART_RX_TX_PIN)
#define SPI_UART_RX_PIN            (0u == SPI_REMOVE_UART_RX_PIN)
#define SPI_UART_RX_WAKE_PIN       (0u == SPI_REMOVE_UART_RX_WAKE_PIN)


/***************************************
*             Includes
****************************************/

#if(SPI_MOSI_SCL_RX_WAKE_PIN)
    #include "SPI_spi_mosi_i2c_scl_uart_rx_wake.h"
#endif /* (SPI_MOSI_SCL_RX_WAKE_PIN) */

#if(SPI_MOSI_SCL_RX_PIN)
    #include "SPI_spi_mosi_i2c_scl_uart_rx.h"
#endif /* (SPI_MOSI_SCL_RX_PIN) */

#if(SPI_MISO_SDA_TX_PIN)
    #include "SPI_spi_miso_i2c_sda_uart_tx.h"
#endif /* (SPI_MISO_SDA_TX_PIN_PIN) */

#if(SPI_SCLK_PIN)
    #include "SPI_spi_sclk.h"
#endif /* (SPI_SCLK_PIN) */

#if(SPI_SS0_PIN)
    #include "SPI_spi_ss0.h"
#endif /* (SPI_SS1_PIN) */

#if(SPI_SS1_PIN)
    #include "SPI_spi_ss1.h"
#endif /* (SPI_SS1_PIN) */

#if(SPI_SS2_PIN)
    #include "SPI_spi_ss2.h"
#endif /* (SPI_SS2_PIN) */

#if(SPI_SS3_PIN)
    #include "SPI_spi_ss3.h"
#endif /* (SPI_SS3_PIN) */

#if(SPI_I2C_PINS)
    #include "SPI_scl.h"
    #include "SPI_sda.h"
#endif /* (SPI_I2C_PINS) */
    
#if(SPI_SPI_MASTER_PINS)
    #include "SPI_sclk_m.h"
    #include "SPI_mosi_m.h"
    #include "SPI_miso_m.h"
#endif /* (SPI_SPI_MASTER_PINS) */

#if(SPI_SPI_SLAVE_PINS)
    #include "SPI_sclk_s.h"
    #include "SPI_mosi_s.h"
    #include "SPI_miso_s.h"
    #include "SPI_ss_s.h"
#endif /* (SPI_SPI_SLAVE_PINS) */

#if(SPI_SPI_MASTER_SS0_PIN)
    #include "SPI_ss0_m.h"
#endif /* (SPI_SPI_MASTER_SS0_PIN) */

#if(SPI_SPI_MASTER_SS1_PIN)
    #include "SPI_ss1_m.h"
#endif /* (SPI_SPI_MASTER_SS1_PIN) */

#if(SPI_SPI_MASTER_SS2_PIN)
    #include "SPI_ss2_m.h"
#endif /* (SPI_SPI_MASTER_SS2_PIN) */

#if(SPI_SPI_MASTER_SS3_PIN)
    #include "SPI_ss3_m.h"
#endif /* (SPI_SPI_MASTER_SS3_PIN) */

#if(SPI_UART_TX_PIN)
    #include "SPI_tx.h"
#endif /* (SPI_UART_TX_PIN) */

#if(SPI_UART_RX_TX_PIN)
    #include "SPI_rx_tx.h"
#endif /* (SPI_UART_RX_TX_PIN) */

#if(SPI_UART_RX_PIN)
    #include "SPI_rx.h"
#endif /* (SPI_UART_RX_PIN) */

#if(SPI_UART_RX_WAKE_PIN)
    #include "SPI_rx_wake.h"
#endif /* (SPI_UART_RX_WAKE_PIN) */


/***************************************
*              Registers
***************************************/

#if(SPI_MOSI_SCL_RX_WAKE_PIN)
    #define SPI_MOSI_SCL_RX_WAKE_HSIOM_REG  \
                                                (*(reg32 *) SPI_spi_mosi_i2c_scl_uart_rx_wake__0__HSIOM)
    #define SPI_MOSI_SCL_RX_WAKE_HSIOM_PTR  \
                                                ( (reg32 *) SPI_spi_mosi_i2c_scl_uart_rx_wake__0__HSIOM)
    #define SPI_MOSI_SCL_RX_WAKE_HSIOM_MASK \
                                                (SPI_spi_mosi_i2c_scl_uart_rx_wake__0__HSIOM_MASK)
    #define SPI_MOSI_SCL_RX_WAKE_HSIOM_POS  \
                                                (SPI_spi_mosi_i2c_scl_uart_rx_wake__0__HSIOM_SHIFT)

    #define SPI_MOSI_SCL_RX_WAKE_INTCFG_REG    (*(reg32 *) \
                                                              SPI_spi_mosi_i2c_scl_uart_rx_wake__0__INTCFG)
    #define SPI_MOSI_SCL_RX_WAKE_INTCFG_PTR    ( (reg32 *) \
                                                              SPI_spi_mosi_i2c_scl_uart_rx_wake__0__INTCFG)

    #define SPI_INTCFG_TYPE_MASK                  (0x03u)
    #define SPI_MOSI_SCL_RX_WAKE_INTCFG_TYPE_POS  (SPI_spi_mosi_i2c_scl_uart_rx_wake__SHIFT)
    #define SPI_MOSI_SCL_RX_WAKE_INTCFG_TYPE_MASK ((uint32)                                           \
                                                                    ((uint32) SPI_INTCFG_TYPE_MASK << \
                                                                    SPI_MOSI_SCL_RX_WAKE_INTCFG_TYPE_POS))
#endif /* (SPI_MOSI_SCL_RX_WAKE_PIN) */

#if(SPI_MOSI_SCL_RX_PIN)
    #define SPI_MOSI_SCL_RX_HSIOM_REG      (*(reg32 *) SPI_spi_mosi_i2c_scl_uart_rx__0__HSIOM)
    #define SPI_MOSI_SCL_RX_HSIOM_PTR      ( (reg32 *) SPI_spi_mosi_i2c_scl_uart_rx__0__HSIOM)
    #define SPI_MOSI_SCL_RX_HSIOM_MASK     (SPI_spi_mosi_i2c_scl_uart_rx__0__HSIOM_MASK)
    #define SPI_MOSI_SCL_RX_HSIOM_POS      (SPI_spi_mosi_i2c_scl_uart_rx__0__HSIOM_SHIFT)
#endif /* (SPI_MOSI_SCL_RX_PIN) */

#if(SPI_MISO_SDA_TX_PIN)
    #define SPI_MISO_SDA_TX_HSIOM_REG      (*(reg32 *) SPI_spi_miso_i2c_sda_uart_tx__0__HSIOM)
    #define SPI_MISO_SDA_TX_HSIOM_PTR      ( (reg32 *) SPI_spi_miso_i2c_sda_uart_tx__0__HSIOM)
    #define SPI_MISO_SDA_TX_HSIOM_MASK     (SPI_spi_miso_i2c_sda_uart_tx__0__HSIOM_MASK)
    #define SPI_MISO_SDA_TX_HSIOM_POS      (SPI_spi_miso_i2c_sda_uart_tx__0__HSIOM_SHIFT)
#endif /* (SPI_MISO_SDA_TX_PIN_PIN) */

#if(SPI_SCLK_PIN)
    #define SPI_SCLK_HSIOM_REG     (*(reg32 *) SPI_spi_sclk__0__HSIOM)
    #define SPI_SCLK_HSIOM_PTR     ( (reg32 *) SPI_spi_sclk__0__HSIOM)
    #define SPI_SCLK_HSIOM_MASK    (SPI_spi_sclk__0__HSIOM_MASK)
    #define SPI_SCLK_HSIOM_POS     (SPI_spi_sclk__0__HSIOM_SHIFT)
#endif /* (SPI_SCLK_PIN) */

#if(SPI_SS0_PIN)
    #define SPI_SS0_HSIOM_REG      (*(reg32 *) SPI_spi_ss0__0__HSIOM)
    #define SPI_SS0_HSIOM_PTR      ( (reg32 *) SPI_spi_ss0__0__HSIOM)
    #define SPI_SS0_HSIOM_MASK     (SPI_spi_ss0__0__HSIOM_MASK)
    #define SPI_SS0_HSIOM_POS      (SPI_spi_ss0__0__HSIOM_SHIFT)
#endif /* (SPI_SS1_PIN) */

#if(SPI_SS1_PIN)
    #define SPI_SS1_HSIOM_REG      (*(reg32 *) SPI_spi_ss1__0__HSIOM)
    #define SPI_SS1_HSIOM_PTR      ( (reg32 *) SPI_spi_ss1__0__HSIOM)
    #define SPI_SS1_HSIOM_MASK     (SPI_spi_ss1__0__HSIOM_MASK)
    #define SPI_SS1_HSIOM_POS      (SPI_spi_ss1__0__HSIOM_SHIFT)
#endif /* (SPI_SS1_PIN) */

#if(SPI_SS2_PIN)
    #define SPI_SS2_HSIOM_REG     (*(reg32 *) SPI_spi_ss2__0__HSIOM)
    #define SPI_SS2_HSIOM_PTR     ( (reg32 *) SPI_spi_ss2__0__HSIOM)
    #define SPI_SS2_HSIOM_MASK    (SPI_spi_ss2__0__HSIOM_MASK)
    #define SPI_SS2_HSIOM_POS     (SPI_spi_ss2__0__HSIOM_SHIFT)
#endif /* (SPI_SS2_PIN) */

#if(SPI_SS3_PIN)
    #define SPI_SS3_HSIOM_REG     (*(reg32 *) SPI_spi_ss3__0__HSIOM)
    #define SPI_SS3_HSIOM_PTR     ( (reg32 *) SPI_spi_ss3__0__HSIOM)
    #define SPI_SS3_HSIOM_MASK    (SPI_spi_ss3__0__HSIOM_MASK)
    #define SPI_SS3_HSIOM_POS     (SPI_spi_ss3__0__HSIOM_SHIFT)
#endif /* (SPI_SS3_PIN) */

#if(SPI_I2C_PINS)
    #define SPI_SCL_HSIOM_REG     (*(reg32 *) SPI_scl__0__HSIOM)
    #define SPI_SCL_HSIOM_PTR     ( (reg32 *) SPI_scl__0__HSIOM)
    #define SPI_SCL_HSIOM_MASK    (SPI_scl__0__HSIOM_MASK)
    #define SPI_SCL_HSIOM_POS     (SPI_scl__0__HSIOM_SHIFT)

    #define SPI_SDA_HSIOM_REG     (*(reg32 *) SPI_sda__0__HSIOM)
    #define SPI_SDA_HSIOM_PTR     ( (reg32 *) SPI_sda__0__HSIOM)
    #define SPI_SDA_HSIOM_MASK    (SPI_sda__0__HSIOM_MASK)
    #define SPI_SDA_HSIOM_POS     (SPI_sda__0__HSIOM_SHIFT)
#endif /* (SPI_I2C_PINS) */


/***************************************
*        Registers Constants
***************************************/

/* Pins constanst */
#define SPI_HSIOM_DEF_SEL      (0x00u)
#define SPI_HSIOM_GPIO_SEL     (0x00u)
#define SPI_HSIOM_UART_SEL     (0x09u)
#define SPI_HSIOM_I2C_SEL      (0x0Eu)
#define SPI_HSIOM_SPI_SEL      (0x0Fu)

#define SPI_SCB_PINS_NUMBER            (7u)
#define SPI_MOSI_SCL_RX_PIN_INDEX      (0u) /* RX pins without interrupt */
#define SPI_MOSI_SCL_RX_WAKE_PIN_INDEX (0u) /* RX pin with interrupt     */
#define SPI_MISO_SDA_TX_PIN_INDEX      (1u)
#define SPI_SCLK_PIN_INDEX             (2u)
#define SPI_SS0_PIN_INDEX              (3u)
#define SPI_SS1_PIN_INDEX              (4u)
#define SPI_SS2_PIN_INDEX              (5u)
#define SPI_SS3_PIN_INDEX              (6u)

#define SPI_INTCFG_TYPE_FALLING_EDGE   (0x02u)

/* Pin DM defines */
#define SPI_PIN_DM_ALG_HIZ  (0u)
#define SPI_PIN_DM_DIG_HIZ  (1u)
#define SPI_PIN_DM_OD_LO    (4u)
#define SPI_PIN_DM_STRONG   (6u)


/***************************************
*          Macro Definitions
***************************************/

#define SPI_SET_REGISTER_BITS(reg, mask, pos, mode) \
                    do                                           \
                    {                                            \
                        /* Sets new bits-mask */                 \
                        (reg) = (((reg) & ((uint32) ~(uint32) (mask))) | ((uint32) ((uint32) (mode) << (pos)))); \
                    }while(0)

#define SPI_SET_HSIOM_SEL(reg, mask, pos, sel) SPI_SET_REGISTER_BITS(reg, mask, pos, sel)
#define SPI_SET_INCFG_TYPE(reg, mask, pos, intType) \
                                                        SPI_SET_REGISTER_BITS(reg, mask, pos, intType)


/* SPI_SET_I2C_SCL_DR(val) - Sets I2C SCL DR register.
*  SPI_SET_I2C_SCL_HSIOM_SEL(sel) - Sets I2C SCL HSIOM settings.
*/
/* Unconfigured SCB: scl singnal */
#if(SPI_MOSI_SCL_RX_WAKE_PIN)
    #define SPI_SET_I2C_SCL_DR(val) \
                            SPI_spi_mosi_i2c_scl_uart_rx_wake_Write(val)

    #define SPI_SET_I2C_SCL_HSIOM_SEL(sel) \
                    SPI_SET_HSIOM_SEL(SPI_MOSI_SCL_RX_WAKE_HSIOM_REG,  \
                                                   SPI_MOSI_SCL_RX_WAKE_HSIOM_MASK, \
                                                   SPI_MOSI_SCL_RX_WAKE_HSIOM_POS,  \
                                                   (sel))

#elif(SPI_MOSI_SCL_RX_PIN)
    #define SPI_SET_I2C_SCL_DR(val) \
                            SPI_spi_mosi_i2c_scl_uart_rx_Write(val)


    #define SPI_SET_I2C_SCL_HSIOM_SEL(sel) \
                            SPI_SET_HSIOM_SEL(SPI_MOSI_SCL_RX_HSIOM_REG,  \
                                                           SPI_MOSI_SCL_RX_HSIOM_MASK, \
                                                           SPI_MOSI_SCL_RX_HSIOM_POS,  \
                                                           (sel))
#else
    #if(!SPI_I2C_PINS)
        #define SPI_SET_I2C_SCL_DR(val) \
                                                 do{ ; }while(0)
        #define SPI_SET_I2C_SCL_HSIOM_SEL(sel) \
                                                        do{ ; }while(0)

    #endif /* (!SPI_I2C_PINS) */
#endif /* (SPI_MOSI_SCL_RX_PIN) */

/* SCB I2C: scl singal */
#if(SPI_I2C_PINS)
    #define SPI_SET_I2C_SCL_DR(val) SPI_scl_Write(val)

    #define SPI_SET_I2C_SCL_HSIOM_SEL(sel) \
                          SPI_SET_HSIOM_SEL(SPI_SCL_HSIOM_REG,  \
                                                         SPI_SCL_HSIOM_MASK, \
                                                         SPI_SCL_HSIOM_POS,  \
                                                         (sel))

#endif /* (SPI_I2C_PINS) */


#endif /* (CY_SCB_PINS_SPI_H) */


/* [] END OF FILE */
