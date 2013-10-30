/*******************************************************************************
* File Name: SPI_SPI_UART.c
* Version 1.10
*
* Description:
*  This file provides the source code to the API for the SCB Component in
*  SPI and UART modes.
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

/***************************************
*        SPI/UART Private Vars
***************************************/

#if(SPI_INTERNAL_RX_SW_BUFFER_CONST)
    volatile uint32 SPI_rxBufferHead;
    volatile uint32 SPI_rxBufferTail;
    volatile uint8  SPI_rxBufferOverflow;
#endif /* (SPI_INTERNAL_RX_SW_BUFFER_CONST) */

#if(SPI_INTERNAL_TX_SW_BUFFER_CONST)
    volatile uint32 SPI_txBufferHead;
    volatile uint32 SPI_txBufferTail;
#endif /* (SPI_INTERNAL_TX_SW_BUFFER_CONST) */

#if(SPI_INTERNAL_RX_SW_BUFFER)
    /* Add one element to the buffer to receive full packet. One byte in receive buffer is always empty */
    volatile uint8 SPI_rxBufferInternal[SPI_RX_BUFFER_SIZE];
#endif /* (SPI_INTERNAL_RX_SW_BUFFER) */

#if(SPI_INTERNAL_TX_SW_BUFFER)
    volatile uint8 SPI_txBufferInternal[SPI_TX_BUFFER_SIZE];
#endif /* (SPI_INTERNAL_TX_SW_BUFFER) */


#if(SPI_RX_DIRECTION)

    /*******************************************************************************
    * Function Name: SPI_SpiUartReadRxData
    ********************************************************************************
    *
    * Summary:
    *  Retrieves the next data element from the receive buffer. Undefined data will
    *  be returned if the RX buffer is empty.
    *  Call SPI_SpiUartGetRxBufferSize() to return buffer size.
    *   - RX software buffer disabled: Returns data element retrieved from RX FIFO.
    *   - RX software buffer enabled: Returns data element from the software
    *     receive buffer.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Next data element from the receive buffer.
    *
    * Global Variables:
    *  Look into SPI_SpiInit for description.
    *
    *******************************************************************************/
    uint32 SPI_SpiUartReadRxData(void)
    {
        uint32 rxData = 0u;

        #if(SPI_INTERNAL_RX_SW_BUFFER_CONST)
            uint32 locTail;
        #endif /* (SPI_INTERNAL_RX_SW_BUFFER_CONST) */

        #if(SPI_CHECK_RX_SW_BUFFER)
        {
            if(SPI_rxBufferHead != SPI_rxBufferTail)
            {
                /* There is data in RX software buffer */

                /* Calculate index to read from */
                locTail = (SPI_rxBufferTail + 1u);

                if(SPI_RX_BUFFER_SIZE == locTail)
                {
                    locTail = 0u;
                }

                /* Get data fron RX software buffer */
                rxData = SPI_GetWordFromRxBuffer(locTail);

                /* Change index in the buffer */
                SPI_rxBufferTail = locTail;
            }
        }
        #else
        {
            rxData = SPI_RX_FIFO_RD_REG; /* Read data from RX FIFO */
        }
        #endif

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: SPI_SpiUartGetRxBufferSize
    ********************************************************************************
    *
    * Summary:
    *  Returns the number of received data elements in the receive buffer.
    *   - RX software buffer disabled: returns the number of used entries in
    *     RX FIFO.
    *   - RX software buffer enabled: returns the number of elements which were
    *     placed in receive buffer.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Number of received data elements
    *
    *******************************************************************************/
    uint32 SPI_SpiUartGetRxBufferSize(void)
    {
        uint32 size;
        #if(SPI_INTERNAL_RX_SW_BUFFER_CONST)
            uint32 locHead;
        #endif /* (SPI_INTERNAL_RX_SW_BUFFER_CONST) */

        #if(SPI_CHECK_RX_SW_BUFFER)
        {
            locHead = SPI_rxBufferHead;

            if(locHead >= SPI_rxBufferTail)
            {
                size = (locHead - SPI_rxBufferTail);
            }
            else
            {
                size = (locHead + (SPI_RX_BUFFER_SIZE - SPI_rxBufferTail));
            }
        }
        #else
        {
            size = SPI_GET_RX_FIFO_ENTRIES;
        }
        #endif

        return(size);
    }


    /*******************************************************************************
    * Function Name: SPI_SpiUartClearRxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Clear the receive buffer and RX FIFO.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void SPI_SpiUartClearRxBuffer(void)
    {
        #if(SPI_INTERNAL_RX_SW_BUFFER_CONST)
            uint32 intSourceMask;
        #endif /* (SPI_INTERNAL_RX_SW_BUFFER_CONST) */

        #if(SPI_CHECK_RX_SW_BUFFER)
        {
            intSourceMask = SPI_SpiUartDisableIntRx();

            SPI_CLEAR_RX_FIFO;

            /* Flush RX software buffer */
            SPI_rxBufferHead     = SPI_rxBufferTail;
            SPI_rxBufferOverflow = 0u;

            /* End RX transfer */
            SPI_ClearRxInterruptSource(SPI_INTR_RX_ALL);

            SPI_SpiUartEnableIntRx(intSourceMask);
        }
        #else
        {
            SPI_CLEAR_RX_FIFO;
        }
        #endif
    }

#endif /* (SPI_RX_DIRECTION) */


#if(SPI_TX_DIRECTION)

    /*******************************************************************************
    * Function Name: SPI_SpiUartWriteTxData
    ********************************************************************************
    *
    * Summary:
    *  Places a data entry into the transmit buffer to be sent at the next available
    *  bus time.
    *  This function is blocking and waits until there is space available to put the
    *  requested data in the transmit buffer.
    *
    * Parameters:
    *  txDataByte: the data to be transmitted.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void SPI_SpiUartWriteTxData(uint32 txDataByte)
    {
        #if(SPI_INTERNAL_TX_SW_BUFFER_CONST)
            uint32 locHead;
            uint32 intSourceMask;
        #endif /* (SPI_INTERNAL_TX_SW_BUFFER_CONST) */

        #if(SPI_CHECK_TX_SW_BUFFER)
        {
            /* Head index to put data */
            locHead = (SPI_txBufferHead + 1u);

            /* Adjust TX software buffer index */
            if(SPI_TX_BUFFER_SIZE == locHead)
            {
                locHead = 0u;
            }

            while(locHead == SPI_txBufferTail)
            {
                /* Wait for space in the TX software buffer */
            }

            /* The TX software buffer has at least one room */

            if((SPI_txBufferHead == SPI_txBufferTail) &&
               (SPI_FIFO_SIZE != SPI_GET_TX_FIFO_ENTRIES))
            {
                /* TX software buffer is empty: put data directly in TX FIFO */
                SPI_TX_FIFO_WR_REG = txDataByte;
            }
            /* Put data in the TX software buffer */
            else
            {
                /* Clear old status of INTR_TX_EMPTY. It sets at the end of transfer: TX FIFO empty. */
                SPI_ClearTxInterruptSource(SPI_INTR_TX_NOT_FULL);

                SPI_PutWordInTxBuffer(locHead, txDataByte);

                SPI_txBufferHead = locHead;

                /* Enable interrupt to transmit */
                intSourceMask  = SPI_INTR_TX_NOT_FULL;
                intSourceMask |= SPI_GetTxInterruptMode();
                SPI_SpiUartEnableIntTx(intSourceMask);
            }
        }
        #else
        {
            while(SPI_FIFO_SIZE == SPI_GET_TX_FIFO_ENTRIES)
            {
                /* Block while TX FIFO is FULL */
            }

            SPI_TX_FIFO_WR_REG = txDataByte;
        }
        #endif
    }


    /*******************************************************************************
    * Function Name: SPI_SpiUartPutArray
    ********************************************************************************
    *
    * Summary:
    *  Places an array of data into the transmit buffer to be sent.
    *  This function is blocking and waits until there is a space available to put
    *  all the requested data in the transmit buffer. The array size can be greater
    *  than transmit buffer size.
    *
    * Parameters:
    *  wrBuf:  pointer to an array with data to be placed in transmit buffer.
    *  count:  number of data elements to be placed in the transmit buffer.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void SPI_SpiUartPutArray(const uint8 wrBuf[], uint32 count)
    {
        uint32 i;

        for(i=0u; i < count; i++)
        {
            SPI_SpiUartWriteTxData((uint32) wrBuf[i]);
        }
    }


    /*******************************************************************************
    * Function Name: SPI_SpiUartGetTxBufferSize
    ********************************************************************************
    *
    * Summary:
    *  Returns the number of elements currently in the transmit buffer.
    *  TX software buffer disabled: returns the number of used entries in TX FIFO.
    *  TX software buffer enabled: returns the number of elements currently used
    *  in the transmit buffer. This number does not include used entries in the
    *  TX FIFO. The transmit buffer size is zero until the TX FIFO is full.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Number of data elements ready to transmit.
    *
    *******************************************************************************/
    uint32 SPI_SpiUartGetTxBufferSize(void)
    {
        uint32 size;
        #if(SPI_INTERNAL_TX_SW_BUFFER_CONST)
            uint32 locTail;
        #endif /* (SPI_INTERNAL_TX_SW_BUFFER_CONST) */

        #if(SPI_CHECK_TX_SW_BUFFER)
        {
            /* Get current Tail index */
            locTail = SPI_txBufferTail;

            if(SPI_txBufferHead >= locTail)
            {
                size = (SPI_txBufferHead - locTail);
            }
            else
            {
                size = (SPI_txBufferHead + (SPI_TX_BUFFER_SIZE - locTail));
            }
        }
        #else
        {
            size = SPI_GET_TX_FIFO_ENTRIES;
        }
        #endif

        return(size);
    }


    /*******************************************************************************
    * Function Name: SPI_SpiUartClearTxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Clears the transmit buffer and TX FIFO.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void SPI_SpiUartClearTxBuffer(void)
    {
        #if(SPI_INTERNAL_TX_SW_BUFFER_CONST)
            uint32 intSourceMask;
        #endif /* (SPI_INTERNAL_TX_SW_BUFFER_CONST) */

        #if(SPI_CHECK_TX_SW_BUFFER)
        {
            intSourceMask = SPI_SpiUartDisableIntTx();

            SPI_CLEAR_TX_FIFO;

            /* Flush TX software buffer */
            SPI_txBufferHead = SPI_txBufferTail;

            /* End TX transfer if it is in progress */
            intSourceMask &= (uint32) ~SPI_INTR_TX_NOT_FULL;

            SPI_SpiUartEnableIntTx(intSourceMask);
        }
        #else
        {
            SPI_CLEAR_TX_FIFO;
        }
        #endif
    }

#endif /* (SPI_TX_DIRECTION) */


/*******************************************************************************
* Function Name: SPI_SpiUartDisableIntRx
********************************************************************************
*
* Summary:
*  Disables RX interrupt sources.
*
* Parameters:
*  None
*
* Return:
*  Returns RX interrupt soureces enabled before function call.
*
*******************************************************************************/
uint32 SPI_SpiUartDisableIntRx(void)
{
    uint32 intSource;

    intSource = SPI_GetRxInterruptMode();

    SPI_SetRxInterruptMode(SPI_NO_INTR_SOURCES);

    return(intSource);
}


/*******************************************************************************
* Function Name: SPI_SpiUartDisableIntTx
********************************************************************************
*
* Summary:
*  Disables TX interrupt sources.
*
* Parameters:
*  None
*
* Return:
*  Returns TX interrupt soureces enabled before function call.
*
*******************************************************************************/
uint32 SPI_SpiUartDisableIntTx(void)
{
    uint32 intSourceMask;

    intSourceMask = SPI_GetTxInterruptMode();

    SPI_SetTxInterruptMode(SPI_NO_INTR_SOURCES);

    return(intSourceMask);
}


#if(SPI_SCB_MODE_UNCONFIG_CONST_CFG)

    /*******************************************************************************
    * Function Name: SPI_PutWordInRxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Stores byte/word into the RX buffer.
    *  Only available in Unconfigured operation mode.
    *
    * Parameters:
    *  index:      index to store data byte/word in the RX buffer.
    *  rxDataByte: byte/word to store.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void SPI_PutWordInRxBuffer(uint32 idx, uint32 rxDataByte)
    {
        /* Put data in the buffer */
        if(SPI_ONE_BYTE_WIDTH == SPI_rxDataBits)
        {
            SPI_rxBuffer[idx] = ((uint8) rxDataByte);
        }
        else
        {
            SPI_rxBuffer[(uint32)(idx << 1u)]      = LO8(LO16(rxDataByte));
            SPI_rxBuffer[(uint32)(idx << 1u) + 1u] = HI8(LO16(rxDataByte));
        }
    }


    /*******************************************************************************
    * Function Name: SPI_GetWordFromRxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Reads byte/word from RX buffer.
    *  Only available in Unconfigured operation mode.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Returns byte/word read from RX buffer.
    *
    *******************************************************************************/
    uint32 SPI_GetWordFromRxBuffer(uint32 idx)
    {
        uint32 value;

        if(SPI_ONE_BYTE_WIDTH == SPI_rxDataBits)
        {
            value = SPI_rxBuffer[idx];
        }
        else
        {
            value  = (uint32) SPI_rxBuffer[(uint32)(idx << 1u)];
            value |= (uint32) ((uint32)SPI_rxBuffer[(uint32)(idx << 1u) + 1u] << 8u);
        }

        return(value);
    }


    /*******************************************************************************
    * Function Name: SPI_PutWordInTxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Stores byte/word into the TX buffer.
    * Only available in Unconfigured operation mode.
    *
    * Parameters:
    *  idx:        index to store data byte/word in the TX buffer.
    *  txDataByte: byte/word to store.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void SPI_PutWordInTxBuffer(uint32 idx, uint32 txDataByte)
    {
        /* Put data in the buffer */
        if(SPI_ONE_BYTE_WIDTH == SPI_txDataBits)
        {
            SPI_txBuffer[idx] = ((uint8) txDataByte);
        }
        else
        {
            SPI_txBuffer[(uint32)(idx << 1u)]      = LO8(LO16(txDataByte));
            SPI_txBuffer[(uint32)(idx << 1u) + 1u] = HI8(LO16(txDataByte));
        }
    }


    /*******************************************************************************
    * Function Name: SPI_GetWordFromTxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Reads byte/word from TX buffer.
    *  Only available in Unconfigured operation mode.
    *
    * Parameters:
    *  idx: index to get data byte/word from the TX buffer.
    *
    * Return:
    *  Returns byte/word read from TX buffer.
    *
    *******************************************************************************/
    uint32 SPI_GetWordFromTxBuffer(uint32 idx)
    {
        uint32 value;

        if(SPI_ONE_BYTE_WIDTH == SPI_txDataBits)
        {
            value = (uint32) SPI_txBuffer[idx];
        }
        else
        {
            value  = (uint32) SPI_txBuffer[(uint32)(idx << 1u)];
            value |= (uint32) ((uint32) SPI_txBuffer[(uint32)(idx << 1u) + 1u] << 8u);
        }

        return(value);
    }

#endif /* (SPI_SCB_MODE_UNCONFIG_CONST_CFG) */


/* [] END OF FILE */
