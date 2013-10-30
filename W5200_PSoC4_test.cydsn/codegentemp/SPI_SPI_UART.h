/*******************************************************************************
* File Name: SPI_SPI_UART.h
* Version 1.10
*
* Description:
*  This file provides constants and parameter values for the SCB Component in
*  SPI and UART modes.
*
* Note:
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_SPI_UART_SPI_H)
#define CY_SCB_SPI_UART_SPI_H

#include "SPI.h"


/***************************************
*   SPI Initial Parameter Constants
****************************************/

#define SPI_SPI_MODE                   (1u)
#define SPI_SPI_SUB_MODE               (0u)
#define SPI_SPI_CLOCK_MODE             (0u)
#define SPI_SPI_OVS_FACTOR             (4u)
#define SPI_SPI_MEDIAN_FILTER_ENABLE   (0u)
#define SPI_SPI_LATE_MISO_SAMPLE_ENABLE (1u)
#define SPI_SPI_RX_DATA_BITS_NUM       (8u)
#define SPI_SPI_TX_DATA_BITS_NUM       (8u)
#define SPI_SPI_WAKE_ENABLE            (0u)
#define SPI_SPI_BITS_ORDER             (1u)
#define SPI_SPI_TRANSFER_SEPARATION    (1u)
#define SPI_SPI_NUMBER_OF_SS_LINES     (1u)
#define SPI_SPI_RX_BUFFER_SIZE         (8u)
#define SPI_SPI_TX_BUFFER_SIZE         (8u)

#define SPI_SPI_INTERRUPT_MODE         (0u)

#define SPI_SPI_INTR_RX_MASK           (0u)
#define SPI_SPI_INTR_TX_MASK           (0u)

#define SPI_SPI_RX_TRIGGER_LEVEL       (7u)
#define SPI_SPI_TX_TRIGGER_LEVEL       (0u)


/***************************************
*   UART Initial Parameter Constants
****************************************/

#define SPI_UART_SUB_MODE              (0u)
#define SPI_UART_DIRECTION             (3u)
#define SPI_UART_DATA_BITS_NUM         (8u)
#define SPI_UART_PARITY_TYPE           (2u)
#define SPI_UART_STOP_BITS_NUM         (2u)
#define SPI_UART_OVS_FACTOR            (12u)
#define SPI_UART_IRDA_LOW_POWER        (0u)
#define SPI_UART_MEDIAN_FILTER_ENABLE  (0u)
#define SPI_UART_RETRY_ON_NACK         (0u)
#define SPI_UART_IRDA_POLARITY         (0u)
#define SPI_UART_DROP_ON_FRAME_ERR     (0u)
#define SPI_UART_DROP_ON_PARITY_ERR    (0u)
#define SPI_UART_WAKE_ENABLE           (0u)
#define SPI_UART_RX_BUFFER_SIZE        (8u)
#define SPI_UART_TX_BUFFER_SIZE        (8u)
#define SPI_UART_MP_MODE_ENABLE        (0u)
#define SPI_UART_MP_ACCEPT_ADDRESS     (0u)
#define SPI_UART_MP_RX_ADDRESS         (2u)
#define SPI_UART_MP_RX_ADDRESS_MASK    (255u)

#define SPI_UART_INTERRUPT_MODE        (0u)

#define SPI_UART_INTR_RX_MASK          (0u)
#define SPI_UART_INTR_TX_MASK          (0u)

#define SPI_UART_RX_TRIGGER_LEVEL      (7u)
#define SPI_UART_TX_TRIGGER_LEVEL      (0u)

/* Sources of RX errors */
#define SPI_INTR_RX_ERR        (SPI_INTR_RX_OVERFLOW    | \
                                             SPI_INTR_RX_UNDERFLOW   | \
                                             SPI_INTR_RX_FRAME_ERROR | \
                                             SPI_INTR_RX_PARITY_ERROR)

/* UART direction enum */
#define SPI_UART_RX    (1u)
#define SPI_UART_TX    (2u)
#define SPI_UART_TX_RX (3u)


/***************************************
*   Conditional Compilation Parameters
****************************************/

#if(SPI_SCB_MODE_UNCONFIG_CONST_CFG)

    /* Direction */
    #define SPI_RX_DIRECTION           (1u)
    #define SPI_TX_DIRECTION           (1u)
    #define SPI_UART_RX_DIRECTION      (1u)
    #define SPI_UART_TX_DIRECTION      (1u)

    /* Only external RX and TX buffer for uncofigured mode */
    #define SPI_INTERNAL_RX_SW_BUFFER   (0u)
    #define SPI_INTERNAL_TX_SW_BUFFER   (0u)

    /* Get RX and TX buffer size */
    #define SPI_RX_BUFFER_SIZE (SPI_rxBufferSize)
    #define SPI_TX_BUFFER_SIZE (SPI_txBufferSize)

    /* Return true if buffer is provided */
    #define SPI_CHECK_RX_SW_BUFFER (NULL != SPI_rxBuffer)
    #define SPI_CHECK_TX_SW_BUFFER (NULL != SPI_txBuffer)

    /* Alwasy provde global variables to support RX and TX buffers */
    #define SPI_INTERNAL_RX_SW_BUFFER_CONST    (1u)
    #define SPI_INTERNAL_TX_SW_BUFFER_CONST    (1u)

    /* Get wakeup enable option */
    #define SPI_SPI_WAKE_ENABLE_CONST  (1u)
    #define SPI_CHECK_SPI_WAKE_ENABLE  (0u != SPI_scbEnableWake)
    #define SPI_UART_WAKE_ENABLE_CONST (1u)

#else

    /* SPI internal RX and TX buffers */
    #define SPI_INTERNAL_SPI_RX_SW_BUFFER  (SPI_SPI_RX_BUFFER_SIZE > \
                                                                                            SPI_FIFO_SIZE)
    #define SPI_INTERNAL_SPI_TX_SW_BUFFER  (SPI_SPI_TX_BUFFER_SIZE > \
                                                                                            SPI_FIFO_SIZE)

    /* UART internal RX and TX buffers */
    #define SPI_INTERNAL_UART_RX_SW_BUFFER  (SPI_UART_RX_BUFFER_SIZE > \
                                                                                            SPI_FIFO_SIZE)
    #define SPI_INTERNAL_UART_TX_SW_BUFFER  (SPI_UART_TX_BUFFER_SIZE > \
                                                                                            SPI_FIFO_SIZE)

    /* SPI Direction */
    #define SPI_SPI_RX_DIRECTION (1u)
    #define SPI_SPI_TX_DIRECTION (1u)

    /* UART Direction */
    #define SPI_UART_RX_DIRECTION (0u != (SPI_UART_DIRECTION & SPI_UART_RX))
    #define SPI_UART_TX_DIRECTION (0u != (SPI_UART_DIRECTION & SPI_UART_TX))

    /* Direction */
    #define SPI_RX_DIRECTION ((SPI_SCB_MODE_SPI_CONST_CFG) ? \
                                            (SPI_SPI_RX_DIRECTION) : (SPI_UART_RX_DIRECTION))

    #define SPI_TX_DIRECTION ((SPI_SCB_MODE_SPI_CONST_CFG) ? \
                                            (SPI_SPI_TX_DIRECTION) : (SPI_UART_TX_DIRECTION))

    /* Internal RX and TX buffer: for SPI or UART */
    #if(SPI_SCB_MODE_SPI_CONST_CFG)

        /* Internal SPI RX and TX buffer */
        #define SPI_INTERNAL_RX_SW_BUFFER  (SPI_INTERNAL_SPI_RX_SW_BUFFER)
        #define SPI_INTERNAL_TX_SW_BUFFER  (SPI_INTERNAL_SPI_TX_SW_BUFFER)

        /* Internal SPI RX and TX buffer size */
        #define SPI_RX_BUFFER_SIZE         (SPI_SPI_RX_BUFFER_SIZE + 1u)
        #define SPI_TX_BUFFER_SIZE         (SPI_SPI_TX_BUFFER_SIZE)
        
        /* Get wakeup enable option */
        #define SPI_SPI_WAKE_ENABLE_CONST  (0u != SPI_SPI_WAKE_ENABLE)
        #define SPI_UART_WAKE_ENABLE_CONST (0u)

    #else

        /* Internal UART RX and TX buffer */
        #define SPI_INTERNAL_RX_SW_BUFFER  (SPI_INTERNAL_UART_RX_SW_BUFFER)
        #define SPI_INTERNAL_TX_SW_BUFFER  (SPI_INTERNAL_UART_TX_SW_BUFFER)

        /* Internal UART RX and TX buffer size */
        #define SPI_RX_BUFFER_SIZE         (SPI_UART_RX_BUFFER_SIZE + 1u)
        #define SPI_TX_BUFFER_SIZE         (SPI_UART_TX_BUFFER_SIZE)
        
        /* Get wakeup enable option */
        #define SPI_SPI_WAKE_ENABLE_CONST  (0u)
        #define SPI_UART_WAKE_ENABLE_CONST (0u != SPI_UART_WAKE_ENABLE)
    #endif /* (SPI_SCB_MODE_SPI_CONST_CFG) */

    /* Internal RX and TX buffer: for SPI or UART. Used in conditional compilation check */
    #define SPI_CHECK_RX_SW_BUFFER (SPI_INTERNAL_RX_SW_BUFFER)
    #define SPI_CHECK_TX_SW_BUFFER (SPI_INTERNAL_TX_SW_BUFFER)

    /* Provide global variables to support RX and TX buffers */
    #define SPI_INTERNAL_RX_SW_BUFFER_CONST    (SPI_INTERNAL_RX_SW_BUFFER)
    #define SPI_INTERNAL_TX_SW_BUFFER_CONST    (SPI_INTERNAL_TX_SW_BUFFER)
    
    /* Wakeup for SPI */
    #define SPI_CHECK_SPI_WAKE_ENABLE  (SPI_SPI_WAKE_ENABLE_CONST)

#endif /* End (SPI_SCB_MODE_UNCONFIG_CONST_CFG) */

/* Bootloader communication interface enable: NOT supported yet */
#define SPI_SPI_BTLDR_COMM_ENABLED   ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_SPI) || \
                                                    (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))

#define SPI_UART_BTLDR_COMM_ENABLED   ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_SPI) || \
                                                    (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))


/***************************************
*       Type Definitions
***************************************/

/* SPI_SPI_INIT_STRUCT */
typedef struct
{
    uint32 mode;
    uint32 submode;
    uint32 sclkMode;
    uint32 oversample;
    uint32 enableMedianFilter;
    uint32 enableLateSampling;
    uint32 enableWake;
    uint32 rxDataBits;
    uint32 txDataBits;
    uint32 bitOrder;
    uint32 transferSeperation;
    uint32 rxBufferSize;
    uint8* rxBuffer;
    uint32 txBufferSize;
    uint8* txBuffer;
    uint32 enableInterrupt;
    uint32 rxInterruptMask;
    uint32 rxTriggerLevel;
    uint32 txInterruptMask;
    uint32 txTriggerLevel;
} SPI_SPI_INIT_STRUCT;

/* SPI_UART_INIT_STRUCT */
typedef struct
{
    uint32 mode;
    uint32 direction;
    uint32 dataBits;
    uint32 parity;
    uint32 stopBits;
    uint32 oversample;
    uint32 enableIrdaLowPower;
    uint32 enableMedianFilter;
    uint32 enableRetryNack;
    uint32 enableInvertedRx;
    uint32 dropOnParityErr;
    uint32 dropOnFrameErr;
    uint32 enableWake;
    uint32 rxBufferSize;
    uint8* rxBuffer;
    uint32 txBufferSize;
    uint8* txBuffer;
    uint32 enableMultiproc;
    uint32 multiprocAcceptAddr;
    uint32 multiprocAddr;
    uint32 multiprocAddrMask;
    uint32 enableInterrupt;
    uint32 rxInterruptMask;
    uint32 rxTriggerLevel;
    uint32 txInterruptMask;
    uint32 txTriggerLevel;
} SPI_UART_INIT_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

/* SPI specific functions */
#if(SPI_SCB_MODE_UNCONFIG_CONST_CFG)
    void SPI_SpiInit(const SPI_SPI_INIT_STRUCT *config);
#endif /* (SPI_SCB_MODE_UNCONFIG_CONST_CFG) */

#if(SPI_SCB_MODE_SPI_INC)
    void SPI_SpiSetActiveSlaveSelect(uint32 activeSelect);
#endif /* (SPI_SCB_MODE_SPI_INC) */

/* UART specific functions */
#if(SPI_SCB_MODE_UNCONFIG_CONST_CFG)
    void SPI_UartInit(const SPI_UART_INIT_STRUCT *config);
#endif /* (SPI_SCB_MODE_UNCONFIG_CONST_CFG) */

#if(SPI_SCB_MODE_UART_INC)
    void SPI_UartSetRxAddress(uint32 address);
    void SPI_UartSetRxAddressMask(uint32 addressMask);
#endif /* (SPI_SCB_MODE_UART_INC) */

/* UART RX direction APIs */
#if(SPI_UART_RX_DIRECTION)
    uint32 SPI_UartGetChar(void);
    uint32 SPI_UartGetByte(void);
#endif /* (SPI_UART_RX_DIRECTION) */

/* UART TX direction APIs */
#if(SPI_UART_TX_DIRECTION)
    #define SPI_UartPutChar(ch)    SPI_SpiUartWriteTxData((uint32)(ch))
    void SPI_UartPutString(const char8 string[]);
    void SPI_UartPutCRLF(uint32 txDataByte);
#endif /* (SPI_UART_TX_DIRECTION) */

/* Common APIs Rx direction */
#if(SPI_RX_DIRECTION)
    uint32 SPI_SpiUartReadRxData(void);
    uint32 SPI_SpiUartGetRxBufferSize(void);
    void   SPI_SpiUartClearRxBuffer(void);
#endif /* (SPI_RX_DIRECTION) */

/* Common APIs Tx direction */
#if(SPI_TX_DIRECTION)
    void   SPI_SpiUartWriteTxData(uint32 txDataByte);
    void   SPI_SpiUartPutArray(const uint8 wrBuf[], uint32 count);
    void   SPI_SpiUartClearTxBuffer(void);
    uint32 SPI_SpiUartGetTxBufferSize(void);
#endif /* (SPI_TX_DIRECTION) */

CY_ISR_PROTO(SPI_SPI_UART_ISR);

#if(SPI_UART_RX_WAKEUP_IRQ)
    CY_ISR_PROTO(SPI_UART_WAKEUP_ISR);
#endif /* (SPI_UART_RX_WAKEUP_IRQ) */

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (SPI_SPI_BTLDR_COMM_ENABLED)
    /* SPI Bootloader physical layer functions */
    void SPI_SpiCyBtldrCommStart(void);
    void SPI_SpiCyBtldrCommStop (void);
    void SPI_SpiCyBtldrCommReset(void);
    cystatus SPI_SpiCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus SPI_SpiCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (SPI_SPI_BTLDR_COMM_ENABLED) */

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (SPI_UART_BTLDR_COMM_ENABLED)
    /* UART Bootloader physical layer functions */
    void SPI_UartCyBtldrCommStart(void);
    void SPI_UartCyBtldrCommStop (void);
    void SPI_UartCyBtldrCommReset(void);
    cystatus SPI_UartCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus SPI_UartCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (SPI_UART_BTLDR_COMM_ENABLED) */


/***************************************
*     Buffer Access Macro Definitions
***************************************/

#if(SPI_SCB_MODE_UNCONFIG_CONST_CFG)
    /* RX direction */
    void   SPI_PutWordInRxBuffer  (uint32 idx, uint32 rxDataByte);
    uint32 SPI_GetWordFromRxBuffer(uint32 idx);

    /* TX direction */
    void   SPI_PutWordInTxBuffer  (uint32 idx, uint32 txDataByte);
    uint32 SPI_GetWordFromTxBuffer(uint32 idx);

#else

    /* RX direction */
    #if(SPI_INTERNAL_RX_SW_BUFFER_CONST)
        #define SPI_PutWordInRxBuffer(idx, rxDataByte) \
                do{                                                 \
                    SPI_rxBufferInternal[(idx)] = ((uint8) (rxDataByte)); \
                }while(0)

        #define SPI_GetWordFromRxBuffer(idx) SPI_rxBufferInternal[(idx)]

    #endif /* (SPI_INTERNAL_RX_SW_BUFFER_CONST) */

    /* TX direction */
    #if(SPI_INTERNAL_TX_SW_BUFFER_CONST)
        #define SPI_PutWordInTxBuffer(idx, txDataByte) \
                    do{                                             \
                        SPI_txBufferInternal[(idx)] = ((uint8) (txDataByte)); \
                    }while(0)

        #define SPI_GetWordFromTxBuffer(idx) SPI_txBufferInternal[(idx)]

    #endif /* (SPI_INTERNAL_TX_SW_BUFFER_CONST) */

#endif /* (SPI_TX_SW_BUFFER_ENABLE) */


/***************************************
*         SPI API Constants
***************************************/

/* SPI mode enum */
#define SPI_SPI_SLAVE  (0u)
#define SPI_SPI_MASTER (1u)

/* SPI sub mode enum */
#define SPI_SPI_MODE_MOTOROLA      (0x00u)
#define SPI_SPI_MODE_TI_COINCIDES  (0x01u)
#define SPI_SPI_MODE_TI_PRECEDES   (0x11u)
#define SPI_SPI_MODE_NATIONAL      (0x02u)
#define SPI_SPI_MODE_MASK          (0x03u)
#define SPI_SPI_MODE_TI_PRECEDES_MASK  (0x10u)

/* SPI phase and polarity mode enum */
#define SPI_SPI_SCLK_CPHA0_CPOL0   (0x00u)
#define SPI_SPI_SCLK_CPHA0_CPOL1   (0x02u)
#define SPI_SPI_SCLK_CPHA1_CPOL0   (0x01u)
#define SPI_SPI_SCLK_CPHA1_CPOL1   (0x03u)

/* SPI bits order enum */
#define SPI_BITS_ORDER_LSB_FIRST   (0u)
#define SPI_BITS_ORDER_MSB_FIRST   (1u)

/* SPI transfer separation enum */
#define SPI_SPI_TRANSFER_SEPARATED     (0u)
#define SPI_SPI_TRANSFER_CONTINUOUS    (1u)

/* "activeSS" constants for SpiSetActiveSlaveSelect() function */
#define SPI_SPIM_ACTIVE_SS0    (0x00u)
#define SPI_SPIM_ACTIVE_SS1    (0x01u)
#define SPI_SPIM_ACTIVE_SS2    (0x02u)
#define SPI_SPIM_ACTIVE_SS3    (0x03u)


/***************************************
*         UART API Constants
***************************************/

/* UART sub-modes enum */
#define SPI_UART_MODE_STD          (0u)
#define SPI_UART_MODE_SMARTCARD    (1u)
#define SPI_UART_MODE_IRDA         (2u)

/* UART direction enum */
#define SPI_UART_RX    (1u)
#define SPI_UART_TX    (2u)
#define SPI_UART_TX_RX (3u)

/* UART parity enum */
#define SPI_UART_PARITY_EVEN   (0u)
#define SPI_UART_PARITY_ODD    (1u)
#define SPI_UART_PARITY_NONE   (2u)

/* UART stop bits enum */
#define SPI_UART_STOP_BITS_1   (1u)
#define SPI_UART_STOP_BITS_1_5 (2u)
#define SPI_UART_STOP_BITS_2   (3u)

/* UART IrDA low power OVS enum */
#define SPI_UART_IRDA_LP_OVS16     (16u)
#define SPI_UART_IRDA_LP_OVS32     (32u)
#define SPI_UART_IRDA_LP_OVS48     (48u)
#define SPI_UART_IRDA_LP_OVS96     (96u)
#define SPI_UART_IRDA_LP_OVS192    (192u)
#define SPI_UART_IRDA_LP_OVS768    (768u)
#define SPI_UART_IRDA_LP_OVS1536   (1536u)

/* Uart MP: mark (address) and space (data) bit definitions */
#define SPI_UART_MP_MARK       (0x100u)
#define SPI_UART_MP_SPACE      (0x000u)


/***************************************
*     Vars with External Linkage
***************************************/

#if(SPI_SCB_MODE_UNCONFIG_CONST_CFG)
    extern const SPI_SPI_INIT_STRUCT  SPI_configSpi;
    extern const SPI_UART_INIT_STRUCT SPI_configUart;
#endif /* (SPI_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*    Specific SPI Macro Definitions
***************************************/

#define SPI_GET_SPI_INTR_SLAVE_MASK(sourceMask)  ((sourceMask) & SPI_INTR_SLAVE_SPI_BUS_ERROR)
#define SPI_GET_SPI_INTR_MASTER_MASK(sourceMask) ((sourceMask) & SPI_INTR_MASTER_SPI_DONE)
#define SPI_GET_SPI_INTR_RX_MASK(sourceMask) \
                                             ((sourceMask) & (uint32) ~SPI_INTR_SLAVE_SPI_BUS_ERROR)

#define SPI_GET_SPI_INTR_TX_MASK(sourceMask) \
                                             ((sourceMask) & (uint32) ~SPI_INTR_MASTER_SPI_DONE)


/***************************************
*    Specific UART Macro Definitions
***************************************/

#define SPI_UART_GET_CTRL_OVS_IRDA_LP(oversample) \
        ((SPI_UART_IRDA_LP_OVS16   == (oversample)) ? SPI_CTRL_OVS_IRDA_LP_OVS16 : \
         ((SPI_UART_IRDA_LP_OVS32   == (oversample)) ? SPI_CTRL_OVS_IRDA_LP_OVS32 : \
          ((SPI_UART_IRDA_LP_OVS48   == (oversample)) ? SPI_CTRL_OVS_IRDA_LP_OVS48 : \
           ((SPI_UART_IRDA_LP_OVS96   == (oversample)) ? SPI_CTRL_OVS_IRDA_LP_OVS96 : \
            ((SPI_UART_IRDA_LP_OVS192  == (oversample)) ? SPI_CTRL_OVS_IRDA_LP_OVS192 : \
             ((SPI_UART_IRDA_LP_OVS768  == (oversample)) ? SPI_CTRL_OVS_IRDA_LP_OVS768 : \
              ((SPI_UART_IRDA_LP_OVS1536 == (oversample)) ? SPI_CTRL_OVS_IRDA_LP_OVS1536 : \
                                                                          SPI_CTRL_OVS_IRDA_LP_OVS16)))))))

#define SPI_GET_UART_RX_CTRL_ENABLED(direction) ((0u != (SPI_UART_RX & (direction))) ? \
                                                                    (SPI_RX_CTRL_ENABLED) : (0u))

#define SPI_GET_UART_TX_CTRL_ENABLED(direction) ((0u != (SPI_UART_TX & (direction))) ? \
                                                                    (SPI_TX_CTRL_ENABLED) : (0u))


/***************************************
*        SPI Register Settings
***************************************/

#define SPI_CTRL_SPI      (SPI_CTRL_MODE_SPI)
#define SPI_SPI_RX_CTRL   (SPI_RX_CTRL_ENABLED)
#define SPI_SPI_TX_CTRL   (SPI_TX_CTRL_ENABLED)


/***************************************
*       SPI Init Register Settings
***************************************/

#if(SPI_SCB_MODE_SPI_CONST_CFG)

    /* SPI Configuration */
    #define SPI_SPI_DEFAULT_CTRL \
                    (SPI_GET_CTRL_OVS(SPI_SPI_OVS_FACTOR)         | \
                     SPI_GET_CTRL_EC_AM_MODE(SPI_SPI_WAKE_ENABLE) | \
                     SPI_CTRL_SPI)

    #define SPI_SPI_DEFAULT_SPI_CTRL \
                    (SPI_GET_SPI_CTRL_CONTINUOUS    (SPI_SPI_TRANSFER_SEPARATION)       | \
                     SPI_GET_SPI_CTRL_SELECT_PRECEDE(SPI_SPI_SUB_MODE &                   \
                                                                  SPI_SPI_MODE_TI_PRECEDES_MASK)     | \
                     SPI_GET_SPI_CTRL_SCLK_MODE     (SPI_SPI_CLOCK_MODE)                | \
                     SPI_GET_SPI_CTRL_LATE_MISO_SAMPLE(SPI_SPI_LATE_MISO_SAMPLE_ENABLE) | \
                     SPI_GET_SPI_CTRL_SUB_MODE      (SPI_SPI_SUB_MODE)                  | \
                     SPI_GET_SPI_CTRL_MASTER_MODE   (SPI_SPI_MODE))

    /* RX direction */
    #define SPI_SPI_DEFAULT_RX_CTRL \
                    (SPI_GET_RX_CTRL_DATA_WIDTH(SPI_SPI_RX_DATA_BITS_NUM)     | \
                     SPI_GET_RX_CTRL_BIT_ORDER (SPI_SPI_BITS_ORDER)           | \
                     SPI_GET_RX_CTRL_MEDIAN    (SPI_SPI_MEDIAN_FILTER_ENABLE) | \
                     SPI_SPI_RX_CTRL)

    #define SPI_SPI_DEFAULT_RX_FIFO_CTRL \
                    SPI_GET_RX_FIFO_CTRL_TRIGGER_LEVEL(SPI_SPI_RX_TRIGGER_LEVEL)

    /* TX direction */
    #define SPI_SPI_DEFAULT_TX_CTRL \
                    (SPI_GET_TX_CTRL_DATA_WIDTH(SPI_SPI_TX_DATA_BITS_NUM) | \
                     SPI_GET_TX_CTRL_BIT_ORDER (SPI_SPI_BITS_ORDER)       | \
                     SPI_SPI_TX_CTRL)

    #define SPI_SPI_DEFAULT_TX_FIFO_CTRL \
                    SPI_GET_TX_FIFO_CTRL_TRIGGER_LEVEL(SPI_SPI_TX_TRIGGER_LEVEL)

    /* Interrupt sources */
    #define SPI_SPI_DEFAULT_INTR_SPI_EC_MASK   (SPI_NO_INTR_SOURCES)

    #define SPI_SPI_DEFAULT_INTR_I2C_EC_MASK   (SPI_NO_INTR_SOURCES)
    #define SPI_SPI_DEFAULT_INTR_SLAVE_MASK \
                    (SPI_SPI_INTR_RX_MASK & SPI_INTR_SLAVE_SPI_BUS_ERROR)

    #define SPI_SPI_DEFAULT_INTR_MASTER_MASK \
                    (SPI_SPI_INTR_TX_MASK & SPI_INTR_MASTER_SPI_DONE)

    #define SPI_SPI_DEFAULT_INTR_RX_MASK \
                    (SPI_SPI_INTR_RX_MASK & (uint32) ~SPI_INTR_SLAVE_SPI_BUS_ERROR)

    #define SPI_SPI_DEFAULT_INTR_TX_MASK \
                    (SPI_SPI_INTR_TX_MASK & (uint32) ~SPI_INTR_MASTER_SPI_DONE)

#endif /* (SPI_SCB_MODE_SPI_CONST_CFG) */


/***************************************
*        UART Register Settings
***************************************/

#define SPI_CTRL_UART      (SPI_CTRL_MODE_UART)
#define SPI_UART_RX_CTRL   (SPI_RX_CTRL_LSB_FIRST) /* LSB for UART goes first */
#define SPI_UART_TX_CTRL   (SPI_TX_CTRL_LSB_FIRST) /* LSB for UART goes first */


/***************************************
*      UART Init Register Settings
***************************************/

#if(SPI_SCB_MODE_UART_CONST_CFG)

    /* UART configuration */
    #if(SPI_UART_MODE_IRDA == SPI_UART_SUB_MODE)

        #define SPI_DEFAULT_CTRL_OVS   ((0u != SPI_UART_IRDA_LOW_POWER) ?              \
                                (SPI_UART_GET_CTRL_OVS_IRDA_LP(SPI_UART_OVS_FACTOR)) : \
                                (SPI_CTRL_OVS_IRDA_OVS16))

    #else

        #define SPI_DEFAULT_CTRL_OVS   SPI_GET_CTRL_OVS(SPI_UART_OVS_FACTOR)

    #endif /* (SPI_UART_MODE_IRDA == SPI_UART_SUB_MODE) */

    #define SPI_UART_DEFAULT_CTRL \
                                (SPI_GET_CTRL_ADDR_ACCEPT(SPI_UART_MP_ACCEPT_ADDRESS) | \
                                 SPI_DEFAULT_CTRL_OVS                                              | \
                                 SPI_CTRL_UART)

    #define SPI_UART_DEFAULT_UART_CTRL \
                                    (SPI_GET_UART_CTRL_MODE(SPI_UART_SUB_MODE))

    /* RX direction */
    #define SPI_UART_DEFAULT_RX_CTRL_PARITY \
                                ((SPI_UART_PARITY_NONE != SPI_UART_PARITY_TYPE) ?      \
                                  (SPI_GET_UART_RX_CTRL_PARITY(SPI_UART_PARITY_TYPE) | \
                                   SPI_UART_RX_CTRL_PARITY_ENABLED) : (0u))

    #define SPI_UART_DEFAULT_UART_RX_CTRL \
                    (SPI_GET_UART_RX_CTRL_MODE(SPI_UART_STOP_BITS_NUM)                    | \
                     SPI_GET_UART_RX_CTRL_POLARITY(SPI_UART_IRDA_POLARITY)                | \
                     SPI_GET_UART_RX_CTRL_MP_MODE(SPI_UART_MP_MODE_ENABLE)                | \
                     SPI_GET_UART_RX_CTRL_DROP_ON_PARITY_ERR(SPI_UART_DROP_ON_PARITY_ERR) | \
                     SPI_GET_UART_RX_CTRL_DROP_ON_FRAME_ERR(SPI_UART_DROP_ON_FRAME_ERR)   | \
                     SPI_UART_DEFAULT_RX_CTRL_PARITY)

    #define SPI_UART_DEFAULT_RX_CTRL \
                                (SPI_GET_RX_CTRL_DATA_WIDTH(SPI_UART_DATA_BITS_NUM)        | \
                                 SPI_GET_RX_CTRL_MEDIAN    (SPI_UART_MEDIAN_FILTER_ENABLE) | \
                                 SPI_GET_UART_RX_CTRL_ENABLED(SPI_UART_DIRECTION))

    #define SPI_UART_DEFAULT_RX_FIFO_CTRL \
                                SPI_GET_RX_FIFO_CTRL_TRIGGER_LEVEL(SPI_UART_RX_TRIGGER_LEVEL)

    #define SPI_UART_DEFAULT_RX_MATCH_REG  ((0u != SPI_UART_MP_MODE_ENABLE) ?          \
                                (SPI_GET_RX_MATCH_ADDR(SPI_UART_MP_RX_ADDRESS) | \
                                 SPI_GET_RX_MATCH_MASK(SPI_UART_MP_RX_ADDRESS_MASK)) : (0u))

    /* TX direction */
    #define SPI_UART_DEFAULT_TX_CTRL_PARITY (SPI_UART_DEFAULT_RX_CTRL_PARITY)

    #define SPI_UART_DEFAULT_UART_TX_CTRL \
                                (SPI_GET_UART_TX_CTRL_MODE(SPI_UART_STOP_BITS_NUM)       | \
                                 SPI_GET_UART_TX_CTRL_RETRY_NACK(SPI_UART_RETRY_ON_NACK) | \
                                 SPI_UART_DEFAULT_TX_CTRL_PARITY)

    #define SPI_UART_DEFAULT_TX_CTRL \
                                (SPI_GET_TX_CTRL_DATA_WIDTH(SPI_UART_DATA_BITS_NUM) | \
                                 SPI_GET_UART_TX_CTRL_ENABLED(SPI_UART_DIRECTION))

    #define SPI_UART_DEFAULT_TX_FIFO_CTRL \
                                SPI_GET_TX_FIFO_CTRL_TRIGGER_LEVEL(SPI_UART_TX_TRIGGER_LEVEL)

    /* Interrupt sources */
    #define SPI_UART_DEFAULT_INTR_I2C_EC_MASK  (SPI_NO_INTR_SOURCES)
    #define SPI_UART_DEFAULT_INTR_SPI_EC_MASK  (SPI_NO_INTR_SOURCES)
    #define SPI_UART_DEFAULT_INTR_SLAVE_MASK   (SPI_NO_INTR_SOURCES)
    #define SPI_UART_DEFAULT_INTR_MASTER_MASK  (SPI_NO_INTR_SOURCES)
    #define SPI_UART_DEFAULT_INTR_RX_MASK      (SPI_UART_INTR_RX_MASK)
    #define SPI_UART_DEFAULT_INTR_TX_MASK      (SPI_UART_INTR_TX_MASK)

#endif /* (SPI_SCB_MODE_UART_CONST_CFG) */

#endif /* CY_SCB_SPI_UART_SPI_H */


/* [] END OF FILE */
