/*
 * (c)COPYRIGHT
 * ALL RIGHT RESERVED
 *
 * FileName : w5200.c
  * -----------------------------------------------------------------
 */
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "device_depend.h"		
#include "w5200.h"
#include "socket.h"

#ifdef __DEF_IINCHIP_PPP__
   #include "md5.h"
#endif

static uint8_t I_STATUS[MAX_SOCK_NUM];
static uint16_t SMASK[MAX_SOCK_NUM]; /**< Variable for Tx buffer MASK in each channel */
static uint16_t RMASK[MAX_SOCK_NUM]; /**< Variable for Rx buffer MASK in each channel */
static uint16_t SSIZE[MAX_SOCK_NUM]; /**< Max Tx buffer size by each channel */
static uint16_t RSIZE[MAX_SOCK_NUM]; /**< Max Rx buffer size by each channel */
static uint16_t SBUFBASEADDRESS[MAX_SOCK_NUM]; /**< Tx buffer base address by each channel */
static uint16_t RBUFBASEADDRESS[MAX_SOCK_NUM]; /**< Rx buffer base address by each channel */
static uint8_t SUBNET[4];
static uint8_t IPVAL[4];

uint8_t windowfull_retry_cnt[MAX_SOCK_NUM];

uint8_t incr_windowfull_retry_cnt(uint8_t s)
{
  return windowfull_retry_cnt[s]++;
}

void init_windowfull_retry_cnt(uint8_t s)
{
  windowfull_retry_cnt[s] = 0;
}

uint16_t pre_sent_ptr, sent_ptr;

uint8_t getISR(uint8_t s)
{
	return I_STATUS[s];
}
void putISR(uint8_t s, uint8_t val)
{
   I_STATUS[s] = val;
}
uint16_t getIINCHIP_RxMAX(uint8_t s)
{
   return RSIZE[s];
}
uint16_t getIINCHIP_TxMAX(uint8_t s)
{
   return SSIZE[s];
}
uint16_t getIINCHIP_RxMASK(uint8_t s)
{
   return RMASK[s];
}
uint16_t getIINCHIP_TxMASK(uint8_t s)
{
   return SMASK[s];
}
uint16_t getIINCHIP_RxBASE(uint8_t s)
{
   return RBUFBASEADDRESS[s];
}
uint16_t getIINCHIP_TxBASE(uint8_t s)
{
   return SBUFBASEADDRESS[s];
}
void IINCHIP_CSoff(void)
{
	spi_cs_low();
}
void IINCHIP_CSon(void)
{
	spi_cs_high();
}
uint8_t IINCHIP_SpiSendData(uint8_t dat)
{
	return(spi_master_send(dat));
}


 /**
@brief	This function writes the data into W5200 registers.
*/

uint8_t IINCHIP_WRITE(uint16_t addr,uint8_t data)
{
        IINCHIP_ISR_DISABLE();                      // Interrupt Service Routine Disable

	//SPI MODE I/F
	IINCHIP_CSoff();                            // CS=0, SPI start

	IINCHIP_SpiSendData((addr & 0xFF00) >> 8);  // Address byte 1
	IINCHIP_SpiSendData(addr & 0x00FF);         // Address byte 2
	IINCHIP_SpiSendData(0x80);                  // Data write command and Write data length 1
	IINCHIP_SpiSendData(0x01);                  // Write data length 2
	IINCHIP_SpiSendData(data);                  // Data write (write 1byte data)

	IINCHIP_CSon();                             // CS=1,  SPI end

        IINCHIP_ISR_ENABLE();                       // Interrupt Service Routine Enable
	return 1;
}
/**
@brief	This function reads the value from W5200 registers.
*/
uint8_t IINCHIP_READ(uint16_t addr)
{
	uint8_t data;
        
	IINCHIP_ISR_DISABLE();                       // Interrupt Service Routine Disable
	
	IINCHIP_CSoff();                             // CS=0, SPI start
	
        IINCHIP_SpiSendData((addr & 0xFF00) >> 8);   // Address byte 1
	IINCHIP_SpiSendData(addr & 0x00FF);          // Address byte 2
	IINCHIP_SpiSendData(0x00);                   // Data read command and Read data length 1
	IINCHIP_SpiSendData(0x01);                   // Read data length 2   	
	data = IINCHIP_SpiSendData(0x00);            // Data read (read 1byte data)
	
        IINCHIP_CSon();                              // CS=1,  SPI end
	
        IINCHIP_ISR_ENABLE();                        // Interrupt Service Routine Enable
	return data;
}

/**
@brief	This function writes into W5200 memory(Buffer)
*/ 
uint16_t wiz_write_buf(uint16_t addr,uint8_t* buf,uint16_t len)
{
	uint16_t idx = 0;
	
	IINCHIP_ISR_DISABLE();
        
        if(len == 0)
        {
          //// printf("len is '0'\r\n");
          return 0;
        }
     
	//SPI MODE I/F
	IINCHIP_CSoff();                                        // CS=0, SPI start 
	
        IINCHIP_SpiSendData(((addr+idx) & 0xFF00) >> 8);        // Address byte 1
	IINCHIP_SpiSendData((addr+idx) & 0x00FF);               // Address byte 2
	IINCHIP_SpiSendData((0x80 | ((len & 0x7F00) >> 8)));    // Data write command and Write data length 1
	IINCHIP_SpiSendData((len & 0x00FF));                    // Write data length 2
	for(idx = 0; idx < len; idx++)                          // Write data in loop
	{		
		IINCHIP_SpiSendData(buf[idx]);
	}
	
        IINCHIP_CSon();                                         // CS=1, SPI end 
        
        IINCHIP_ISR_ENABLE();                                   // Interrupt Service Routine Enable        
	return len;
}


/**
@brief	This function reads into W5200 memory(Buffer)
*/ 
uint16_t wiz_read_buf(uint16_t addr, uint8_t* buf,uint16_t len)
{
	uint16_t idx = 0;
        
	IINCHIP_ISR_DISABLE();                                  // Interrupt Service Routine Disable
        
	IINCHIP_CSoff();                                        // CS=0, SPI start 
        
	IINCHIP_SpiSendData(((addr+idx) & 0xFF00) >> 8);        // Address byte 1
	IINCHIP_SpiSendData((addr+idx) & 0x00FF);               // Address byte 2
	IINCHIP_SpiSendData((0x00 | ((len & 0x7F00) >> 8)));    // Data read command
	IINCHIP_SpiSendData((len & 0x00FF));		        

	for(idx = 0; idx < len; idx++)                          // Read data in loop
	{
	 	buf[idx] = IINCHIP_SpiSendData(0x00);
		
	}
        
	IINCHIP_CSon();                                         // CS=0, SPI end 	   	
        
	IINCHIP_ISR_ENABLE();                                   // Interrupt Service Routine Enable
	return len;
}


/**
@brief	This function is for resetting of the iinchip. Initializes the iinchip to work in whether DIRECT or INDIRECT mode
*/ 
void iinchip_init(void)
{	
	setMR( MR_RST );
#ifdef __DEF_IINCHIP_DBG__	
	// printf("MR value is %02x \r\n",IINCHIP_READ(MR));
#endif	
}


/**
@brief	This function set the transmit & receive buffer size as per the channels is used
Note for TMSR and RMSR bits are as follows\n
bit 1-0 : memory size of channel #0 \n
bit 3-2 : memory size of channel #1 \n
bit 5-4 : memory size of channel #2 \n
bit 7-6 : memory size of channel #3 \n
bit 9-8 : memory size of channel #4 \n
bit 11-10 : memory size of channel #5 \n
bit 12-12 : memory size of channel #6 \n
bit 15-14 : memory size of channel #7 \n
Maximum memory size for Tx, Rx in the W5200 is 16K Bytes,\n
In the range of 16KBytes, the memory size could be allocated dynamically by each channel.\n
Be attentive to sum of memory size shouldn't exceed 8Kbytes\n
and to data transmission and receiption from non-allocated channel may cause some problems.\n
If the 16KBytes memory is already  assigned to centain channel, \n
other 3 channels couldn't be used, for there's no available memory.\n
If two 4KBytes memory are assigned to two each channels, \n
other 2 channels couldn't be used, for there's no available memory.\n
*/ 
void sysinit( uint8_t * tx_size, uint8_t * rx_size	)
{
	int16_t i;
	int16_t ssum,rsum;

#ifdef __DEF_IINCHIP_DBG__
	// printf("sysinit()\r\n"); 
#endif

	ssum = 0;
	rsum = 0;
	
	SBUFBASEADDRESS[0] = (uint16_t)(__DEF_IINCHIP_MAP_TXBUF__);		/* Set base address of Tx memory for channel #0 */
	RBUFBASEADDRESS[0] = (uint16_t)(__DEF_IINCHIP_MAP_RXBUF__);		/* Set base address of Rx memory for channel #0 */

  for (i = 0 ; i < MAX_SOCK_NUM; i++)       // Set the size, masking and base address of Tx & Rx memory by each channel
	{
    IINCHIP_WRITE((Sn_TXMEM_SIZE(i)),tx_size[i]);
    IINCHIP_WRITE((Sn_RXMEM_SIZE(i)),rx_size[i]);

#ifdef __DEF_IINCHIP_DBG__
         // printf("Sn_TXMEM_SIZE = %d\r\n",IINCHIP_READ(Sn_TXMEM_SIZE(i)));
         // printf("Sn_RXMEM_SIZE = %d\r\n",IINCHIP_READ(Sn_RXMEM_SIZE(i)));
#endif

		SSIZE[i] = (int16_t)(0);
		RSIZE[i] = (int16_t)(0);

		if (ssum <= 16384)
		{
         switch( tx_size[i] )
			{
			case 1:
				SSIZE[i] = (int16_t)(1024);
				SMASK[i] = (uint16_t)(0x03FF);
				break;
			case 2:
				SSIZE[i] = (int16_t)(2048);
				SMASK[i] = (uint16_t)(0x07FF);
				break;
			case 4:
				SSIZE[i] = (int16_t)(4096);
				SMASK[i] = (uint16_t)(0x0FFF);
				break;
			case 8:
				SSIZE[i] = (int16_t)(8192);
				SMASK[i] = (uint16_t)(0x1FFF);
				break;
			case 16:
				SSIZE[i] = (int16_t)(16384);
				SMASK[i] = (uint16_t)(0x3FFF);
			break;
			}
		}

		if (rsum <= 16384)
		{
         switch( rx_size[i] )
			{
			case 1:
				RSIZE[i] = (int16_t)(1024);
				RMASK[i] = (uint16_t)(0x03FF);
				break;
			case 2:
				RSIZE[i] = (int16_t)(2048);
				RMASK[i] = (uint16_t)(0x07FF);
				break;
			case 4:
				RSIZE[i] = (int16_t)(4096);
				RMASK[i] = (uint16_t)(0x0FFF);
				break;
			case 8:
				RSIZE[i] = (int16_t)(8192);
				RMASK[i] = (uint16_t)(0x1FFF);
				break;
			case 16:
				RSIZE[i] = (int16_t)(16384);
				RMASK[i] = (uint16_t)(0x3FFF);
				break;
			}
		}
		ssum += SSIZE[i];
		rsum += RSIZE[i];

        if (i != 0)             // Sets base address of Tx and Rx memory for channel #1,#2,#3
		{
			SBUFBASEADDRESS[i] = SBUFBASEADDRESS[i-1] + SSIZE[i-1];
			RBUFBASEADDRESS[i] = RBUFBASEADDRESS[i-1] + RSIZE[i-1];
		}
#ifdef __DEF_IINCHIP_DBG__
		// printf("ch = %d\r\n",i);
		// printf("SBUFBASEADDRESS = %d\r\n",(uint16_t)SBUFBASEADDRESS[i]);
		// printf("RBUFBASEADDRESS = %d\r\n",(uint16_t)RBUFBASEADDRESS[i]);
		// printf("SSIZE = %d\r\n",SSIZE[i]);
		// printf("RSIZE = %d\r\n",RSIZE[i]);		
#endif
	}
}

// added

/**
@brief	This function sets up gateway IP address.
*/ 
void setGAR(
	uint8_t * addr	/**< a pointer to a 4 -byte array responsible to set the Gateway IP address. */
	)
{
	IINCHIP_WRITE((GAR0 + 0),addr[0]);
	IINCHIP_WRITE((GAR0 + 1),addr[1]);
	IINCHIP_WRITE((GAR0 + 2),addr[2]);
	IINCHIP_WRITE((GAR0 + 3),addr[3]);
}

/*
void getGWIP(uint8_t * addr)
{
	addr[0] = IINCHIP_READ((GAR0 + 0));
	addr[1] = IINCHIP_READ((GAR0 + 1));
	addr[2] = IINCHIP_READ((GAR0 + 2));
	addr[3] = IINCHIP_READ((GAR0 + 3));
}
*/

/**
@brief	It sets up SubnetMask address
*/ 
void saveSUBR(
	uint8_t * addr	/**< a pointer to a 4 -byte array responsible to set the SubnetMask address */
	)
{
	SUBNET[0] = addr[0];
	SUBNET[1] = addr[1];
	SUBNET[2] = addr[2];
	SUBNET[3] = addr[3];
}

void setSUBR(void)
{   
	IINCHIP_WRITE((SUBR0 + 0), SUBNET[0]);
	IINCHIP_WRITE((SUBR0 + 1), SUBNET[1]);
	IINCHIP_WRITE((SUBR0 + 2), SUBNET[2]);
	IINCHIP_WRITE((SUBR0 + 3), SUBNET[3]);
}

void clearSUBR(void)
{
	getSIPR(&IPVAL[0]);
	if(IPVAL[0] ==0 &&IPVAL[1] ==0 &&IPVAL[2] ==0&&IPVAL[3] ==0)
	{
		IINCHIP_WRITE((SUBR0 + 0), 0);
		IINCHIP_WRITE((SUBR0 + 1), 0);
		IINCHIP_WRITE((SUBR0 + 2), 0);
		IINCHIP_WRITE((SUBR0 + 3), 0);
	}
}


/**
@brief	This function sets up MAC address.
*/ 
void setSHAR(
	uint8_t * addr	/**< a pointer to a 6 -byte array responsible to set the MAC address. */
	)
{
	IINCHIP_WRITE((SHAR0 + 0),addr[0]);
	IINCHIP_WRITE((SHAR0 + 1),addr[1]);
	IINCHIP_WRITE((SHAR0 + 2),addr[2]);
	IINCHIP_WRITE((SHAR0 + 3),addr[3]);
	IINCHIP_WRITE((SHAR0 + 4),addr[4]);
	IINCHIP_WRITE((SHAR0 + 5),addr[5]);
}

/**
@brief	This function sets up Source IP address.
*/
void setSIPR(
	uint8_t * addr	/**< a pointer to a 4 -byte array responsible to set the Source IP address. */
	)
{
	IINCHIP_WRITE((SIPR0 + 0),addr[0]);
	IINCHIP_WRITE((SIPR0 + 1),addr[1]);
	IINCHIP_WRITE((SIPR0 + 2),addr[2]);
	IINCHIP_WRITE((SIPR0 + 3),addr[3]);
}

/**
@brief	This function sets up Source IP address.
*/
void getGAR(uint8_t * addr)
{
	addr[0] = IINCHIP_READ(GAR0);
	addr[1] = IINCHIP_READ(GAR0+1);
	addr[2] = IINCHIP_READ(GAR0+2);
	addr[3] = IINCHIP_READ(GAR0+3);
}
void getSUBR(uint8_t * addr)
{
	addr[0] = SUBNET[0];
	addr[1] = SUBNET[1];
	addr[2] = SUBNET[2];
	addr[3] = SUBNET[3];
}
void getSHAR(uint8_t * addr)
{
	addr[0] = IINCHIP_READ(SHAR0);
	addr[1] = IINCHIP_READ(SHAR0+1);
	addr[2] = IINCHIP_READ(SHAR0+2);
	addr[3] = IINCHIP_READ(SHAR0+3);
	addr[4] = IINCHIP_READ(SHAR0+4);
	addr[5] = IINCHIP_READ(SHAR0+5);
}
void getSIPR(uint8_t * addr)
{
	addr[0] = IINCHIP_READ(SIPR0);
	addr[1] = IINCHIP_READ(SIPR0+1);
	addr[2] = IINCHIP_READ(SIPR0+2);
	addr[3] = IINCHIP_READ(SIPR0+3);
}

void setMR(uint8_t val)
{
	IINCHIP_WRITE(MR,val);
}

/**
@brief	This function gets Interrupt register in common register.
 */
uint8_t getIR( void )
{
   return IINCHIP_READ(IR);
}


/**
 Retransmittion 
 **/
 
/**
@brief	This function sets up Retransmission time.

If there is no response from the peer or delay in response then retransmission 
will be there as per RTR (Retry Time-value Register)setting
*/
void setRTR(uint16_t timeout)
{
	IINCHIP_WRITE(RTR,(uint8_t)((timeout & 0xff00) >> 8));
	IINCHIP_WRITE((RTR + 1),(uint8_t)(timeout & 0x00ff));
}

/**
@brief	This function set the number of Retransmission.

If there is no response from the peer or delay in response then recorded time 
as per RTR & RCR register seeting then time out will occur.
*/
void setRCR(uint8_t retry)
{
	IINCHIP_WRITE(RCR,retry);
}




/**
@brief	This function set the interrupt mask Enable/Disable appropriate Interrupt. ('1' : interrupt enable)

If any bit in IMR is set as '0' then there is not interrupt signal though the bit is
set in IR register.
*/
void setIMR(uint8_t mask)
{
	IINCHIP_WRITE(IMR,mask); // must be setted 0x10.
}

/**
@brief	This sets the maximum segment size of TCP in Active Mode), while in Passive Mode this is set by peer
*/
void setSn_MSS(SOCKET s, uint16_t Sn_MSSR0)
{
	IINCHIP_WRITE(Sn_MSSR0(s),(uint8_t)((Sn_MSSR0 & 0xff00) >> 8));
	IINCHIP_WRITE((Sn_MSSR0(s) + 1),(uint8_t)(Sn_MSSR0 & 0x00ff));
}

void setSn_TTL(SOCKET s, uint8_t ttl)
{
   IINCHIP_WRITE(Sn_TTL(s), ttl);
}


/**
@brief	These below function is used to setup the Protocol Field of IP Header when
		executing the IP Layer RAW mode.
*/
void setSn_PROTO(SOCKET s, uint8_t proto)
{
	IINCHIP_WRITE(Sn_PROTO(s),proto);
}


/**
@brief	get socket interrupt status

These below functions are used to read the Interrupt & Soket Status register
*/
uint8_t getSn_IR(SOCKET s)
{
   return IINCHIP_READ(Sn_IR(s));
}


/**
@brief	 get socket status
*/
uint8_t getSn_SR(SOCKET s)
{
   return IINCHIP_READ(Sn_SR(s));
}


/**
@brief	get socket TX free buf size

This gives free buffer size of transmit buffer. This is the data size that user can transmit.
User shuold check this value first and control the size of transmitting data
*/
uint16_t getSn_TX_FSR(SOCKET s)
{
	uint16_t val=0,val1=0;
	do
	{
		val1 = IINCHIP_READ(Sn_TX_FSR0(s));
		val1 = (val1 << 8) + IINCHIP_READ(Sn_TX_FSR0(s) + 1);
      if (val1 != 0)
		{
   			val = IINCHIP_READ(Sn_TX_FSR0(s));
   			val = (val << 8) + IINCHIP_READ(Sn_TX_FSR0(s) + 1);
		}
	} while (val != val1);
   return val;
}


/**
@brief	 get socket RX recv buf size

This gives size of received data in receive buffer. 
*/
uint16_t getSn_RX_RSR(SOCKET s)
{
	uint16_t val=0,val1=0;
	do
	{
		val1 = IINCHIP_READ(Sn_RX_RSR0(s));
		val1 = (val1 << 8) + IINCHIP_READ(Sn_RX_RSR0(s) + 1);
      if(val1 != 0)
		{
   			val = IINCHIP_READ(Sn_RX_RSR0(s));
   			val = (val << 8) + IINCHIP_READ(Sn_RX_RSR0(s) + 1);
		}
	} while (val != val1);
   return val;
}


/**
@brief	 This function is being called by send() and sendto() function also. 

This function read the Tx write pointer register and after copy the data in buffer update the Tx write pointer
register. User should read upper byte first and lower byte later to get proper value.
*/
void send_data_processing(SOCKET s, uint8_t *data, uint16_t len)
{
	
	uint16_t ptr;
	ptr = IINCHIP_READ(Sn_TX_WR0(s));
	ptr = (ptr << 8) + IINCHIP_READ(Sn_TX_WR0(s) + 1);
	write_data(s, data, (uint8_t *)(ptr), len);
	ptr += len;

	IINCHIP_WRITE(Sn_TX_WR0(s),(uint8_t)((ptr & 0xff00) >> 8));
	IINCHIP_WRITE((Sn_TX_WR0(s) + 1),(uint8_t)(ptr & 0x00ff));
	
}


/**
@brief	This function is being called by recv() also.

This function read the Rx read pointer register
and after copy the data from receive buffer update the Rx write pointer register.
User should read upper byte first and lower byte later to get proper value.
*/
void recv_data_processing(SOCKET s, uint8_t *data, uint16_t len)
{
	uint16_t ptr;
	ptr = IINCHIP_READ(Sn_RX_RD0(s));
	ptr = ((ptr & 0x00ff) << 8) + IINCHIP_READ(Sn_RX_RD0(s) + 1);
	
#ifdef __DEF_IINCHIP_DBG__
	// printf(" ISR_RX: rd_ptr : %.4x\r\n", ptr);
#endif

	read_data(s, (uint8_t *)ptr, data, len); // read data
	ptr += len;
	IINCHIP_WRITE(Sn_RX_RD0(s),(uint8_t)((ptr & 0xff00) >> 8));
	IINCHIP_WRITE((Sn_RX_RD0(s) + 1),(uint8_t)(ptr & 0x00ff));
}


/**
@brief	for copy the data form application buffer to Transmite buffer of the chip.

This function is being used for copy the data form application buffer to Transmite
buffer of the chip. It calculate the actual physical address where one has to write
the data in transmite buffer. Here also take care of the condition while it exceed
the Tx memory uper-bound of socket.
*/
void write_data(SOCKET s, vuint8_t * src, vuint8_t * dst, uint16_t len)
{
	uint16_t size;
	uint16_t dst_mask;
	uint8_t * dst_ptr;

	dst_mask = (uint32_t)dst & getIINCHIP_TxMASK(s);
	dst_ptr = (uint8_t *)(getIINCHIP_TxBASE(s) + dst_mask);
	
	if (dst_mask + len > getIINCHIP_TxMAX(s)) 
	{
		size = getIINCHIP_TxMAX(s) - dst_mask;
		wiz_write_buf((uint32_t)dst_ptr, (uint8_t*)src, size);
		src += size;
		size = len - size;
		dst_ptr = (uint8_t *)(getIINCHIP_TxBASE(s));
		wiz_write_buf((uint32_t)dst_ptr, (uint8_t*)src, size);
	} 
	else
	{
		wiz_write_buf((uint32_t)dst_ptr, (uint8_t*)src, len);
	}
}


/**
@brief	This function is being used for copy the data form Receive buffer of the chip to application buffer.

It calculate the actual physical address where one has to read
the data from Receive buffer. Here also take care of the condition while it exceed
the Rx memory uper-bound of socket.
*/
void read_data(SOCKET s, vuint8_t * src, vuint8_t * dst, uint16_t len)
{
	uint16_t size;
	uint16_t src_mask;
	uint8_t * src_ptr;

	src_mask = (uint32_t)src & getIINCHIP_RxMASK(s);
	src_ptr = (uint8_t *)(getIINCHIP_RxBASE(s) + src_mask);
	
	if( (src_mask + len) > getIINCHIP_RxMAX(s) ) 
	{
		size = getIINCHIP_RxMAX(s) - src_mask;
		wiz_read_buf((uint32_t)src_ptr, (uint8_t*)dst,size);
		dst += size;
		size = len - size;
		src_ptr = (uint8_t *)(getIINCHIP_RxBASE(s));
		wiz_read_buf((uint32_t)src_ptr, (uint8_t*) dst,size);
	} 
	else
	{
		wiz_read_buf((uint32_t)src_ptr, (uint8_t*) dst,len);
	}
}


#ifdef __DEF_IINCHIP_PPP__
#define PPP_OPTION_BUF_LEN 64

uint8_t pppinit_in(uint8_t * id, uint8_t idlen, uint8_t * passwd, uint8_t passwdlen);


/**
@brief	make PPPoE connection
@return	1 => success to connect, 2 => Auth fail, 3 => timeout, 4 => Auth type not support

*/
uint8_t pppinit(uint8_t * id, uint8_t idlen, uint8_t * passwd, uint8_t passwdlen)
{
	uint8_t ret;
	uint8_t isr;
	
	// PHASE0. W5200 PPPoE(ADSL) setup
	// enable pppoe mode
	// printf("-- PHASE 0. W5200 PPPoE(ADSL) setup process --\r\n");
	// printf("\r\n");
	IINCHIP_WRITE(MR,IINCHIP_READ(MR) | MR_PPPOE);

	// open socket in pppoe mode
	isr = IINCHIP_READ(Sn_IR(0));// first clear isr(0), W5200 at present time
	IINCHIP_WRITE(Sn_IR(0),isr);
	
	IINCHIP_WRITE(PTIMER,200); // 5sec timeout
	IINCHIP_WRITE(PMAGIC,0x01); // magic number
	IINCHIP_WRITE(Sn_MR(0),Sn_MR_PPPOE);
	IINCHIP_WRITE(Sn_CR(0),Sn_CR_OPEN);
	
	/* wait to process the command... */
	while( IINCHIP_READ(Sn_CR(0)) ) 
		;
	/* ------- */
	
	ret = pppinit_in(id, idlen, passwd, passwdlen);

	// close ppp connection socket
	/* +200801 (hwkim) */
	close(0);
	/* ------- */
	
	return ret;
}


uint8_t pppinit_in(uint8_t * id, uint8_t idlen, uint8_t * passwd, uint8_t passwdlen)
{
	uint8_t loop_idx = 0;
	uint8_t isr = 0;
	uint8_t buf[PPP_OPTION_BUF_LEN];
	uint16_t len;
	uint8_t str[PPP_OPTION_BUF_LEN];
	uint8_t str_idx,dst_idx;

   // PHASE1. PPPoE Discovery
	// start to connect pppoe connection
	// printf("-- PHASE 1. PPPoE Discovery process --");
	// printf(" ok\r\n");
	// printf("\r\n");
	IINCHIP_WRITE(Sn_CR(0),Sn_CR_PCON);
	/* wait to process the command... */
	while( IINCHIP_READ(Sn_CR(0)) ) 
		;
	/* ------- */
	
	wait_10ms(100);

	loop_idx = 0;
	//check whether PPPoE discovery end or not
	while (!(IINCHIP_READ(Sn_IR(0)) & Sn_IR_PNEXT))
	{
		// printf(".");
		if (loop_idx++ == 10) // timeout
		{
			// printf("timeout before LCP\r\n"); 
			return 3;
		}
		wait_10ms(100);
	}

   /* clear interrupt value*/
   IINCHIP_WRITE(Sn_IR(0), 0xff);
   /*---*/

   // PHASE2. LCP process
	// printf("-- PHASE 2. LCP process --");
		
	// send LCP Request
	{
		// Magic number option
		// option format (type value + length value + data)
	   // write magic number value
		buf[0] = 0x05; // type value
		buf[1] = 0x06; // length value
		buf[2] = 0x01; buf[3] = 0x01; buf[4] = 0x01; buf[5]= 0x01; // data
		// for MRU option, 1492 0x05d4  
		// buf[6] = 0x01; buf[7] = 0x04; buf[8] = 0x05; buf[9] = 0xD4;
	}
	send_data_processing(0, buf, 0x06);
	IINCHIP_WRITE(Sn_CR(0),Sn_CR_PCR); // send request 
	/* wait to process the command... */
	while( IINCHIP_READ(Sn_CR(0)) ) 
		;
	/* ------- */
		
	wait_10ms(100);

	while (!((isr = IINCHIP_READ(Sn_IR(0))) & Sn_IR_PNEXT))
	{
		if (isr & Sn_IR_PRECV) // Not support option
		{
   /* clear interrupt value*/
         IINCHIP_WRITE(Sn_IR(0), Sn_IR_PRECV);
   /*---*/
			len = getSn_RX_RSR(0);
			if ( len > 0 )
			{
				recv_data_processing(0, str, len);
				IINCHIP_WRITE(Sn_CR(0),Sn_CR_RECV);
				/* wait to process the command... */
				while( IINCHIP_READ(Sn_CR(0)) ) 
					;
				/* ------- */
				
				// for debug
				//// printf("LCP proc\r\n"); for (i = 0; i < len; i++) // printf ("%02x ", str[i]); // printf("\r\n");
				// get option length
				len = str[4]; len = ((len & 0x00ff) << 8) + str[5];
				len += 2;
				str_idx = 6; dst_idx = 0; // ppp header is 6 byte, so starts at 6.
				do 
				{
					if ((str[str_idx] == 0x01) || (str[str_idx] == 0x02) || (str[str_idx] == 0x03) || (str[str_idx] == 0x05))
					{
						// skip as length of support option. str_idx+1 is option's length.
						str_idx += str[str_idx+1];
					}
					else
					{
						// not support option , REJECT
						memcpy((uint8_t *)(buf+dst_idx), (uint8_t *)(str+str_idx), str[str_idx+1]);
						dst_idx += str[str_idx+1]; str_idx += str[str_idx+1];
					}
				} while (str_idx != len);
	   			// for debug
	   			// // printf("LCP dst proc\r\n"); for (i = 0; i < dst_idx; i++) // printf ("%02x ", dst[i]); // printf("\r\n");
	   
	   			// send LCP REJECT packet
	   			send_data_processing(0, buf, dst_idx);
	   			IINCHIP_WRITE(Sn_CR(0),Sn_CR_PCJ);
				/* wait to process the command... */
				while( IINCHIP_READ(Sn_CR(0)) ) 
					;
				/* ------- */
  			}
		}
		// printf(".");
		if (loop_idx++ == 10) // timeout
		{
			// printf("timeout after LCP\r\n");
			return 3;
		}
		wait_10ms(100);
	}
	// printf(" ok\r\n");
	// printf("\r\n");

   /* clear interrupt value*/
   IINCHIP_WRITE(Sn_IR(0), 0xff);
   /*---*/

	// printf("-- PHASE 3. PPPoE(ADSL) Authentication mode --\r\n");
	// printf("Authentication protocol : %.2x %.2x, ", IINCHIP_READ(PATR0), IINCHIP_READ(PATR0+1));

	loop_idx = 0;
	if (IINCHIP_READ(PATR0) == 0xc0 && IINCHIP_READ(PATR0+1) == 0x23)
	{
		// printf("PAP\r\n"); // in case of adsl normally supports PAP.
		// send authentication data
		// copy (idlen + id + passwdlen + passwd)
		buf[loop_idx] = idlen; loop_idx++;
		memcpy((uint8_t *)(buf+loop_idx), (uint8_t *)(id), idlen); loop_idx += idlen;
		buf[loop_idx] = passwdlen; loop_idx++;
		memcpy((uint8_t *)(buf+loop_idx), (uint8_t *)(passwd), passwdlen); loop_idx += passwdlen;
		send_data_processing(0, buf, loop_idx);
		IINCHIP_WRITE(Sn_CR(0),Sn_CR_PCR);
		/* wait to process the command... */
		while( IINCHIP_READ(Sn_CR(0)) ) 
			;
		/* ------- */		
                wait_delay_ms(1000);
	}	
	else if (IINCHIP_READ(PATR0) == 0xc2 && IINCHIP_READ(PATR0+1) == 0x23)
	{
		uint8_t chal_len;
		md5_ctx context;
		uint8_t  digest[16];

		len = getSn_RX_RSR(0);
		if ( len > 0 )
		{
			recv_data_processing(0, str, len);
			IINCHIP_WRITE(Sn_CR(0),Sn_CR_RECV);
			/* wait to process the command... */
			while( IINCHIP_READ(Sn_CR(0)) ) 
				;
			/* ------- */
#ifdef __DEF_IINCHIP_DBG__
			// printf("recv CHAP\r\n");
			{
				int16_t i;
				
				for (i = 0; i < 32; i++) 
					// printf ("%02x ", str[i]);
			}
			// printf("\r\n");
#endif
// str is C2 23 xx CHAL_ID xx xx CHAP_LEN CHAP_DATA
// index  0  1  2  3       4  5  6        7 ...

			memset(buf,0x00,64);
			buf[loop_idx] = str[3]; loop_idx++; // chal_id
			memcpy((uint8_t *)(buf+loop_idx), (uint8_t *)(passwd), passwdlen); loop_idx += passwdlen; //passwd
			chal_len = str[6]; // chal_id
			memcpy((uint8_t *)(buf+loop_idx), (uint8_t *)(str+7), chal_len); loop_idx += chal_len; //challenge
			buf[loop_idx] = 0x80;
#ifdef __DEF_IINCHIP_DBG__
			// printf("CHAP proc d1\r\n");
			{
				int16_t i;
				for (i = 0; i < 64; i++) 
					// printf ("%02x ", buf[i]);
			}
			// printf("\r\n");
#endif

			md5_init(&context);
			md5_update(&context, buf, loop_idx);
			md5_final(digest, &context);

#ifdef __DEF_IINCHIP_DBG__
			// printf("CHAP proc d1\r\n");
			{
				int16_t i;				
				for (i = 0; i < 16; i++) 
					// printf ("%02x", digest[i]);
			}
			// printf("\r\n");
#endif
			loop_idx = 0;
			buf[loop_idx] = 16; loop_idx++; // hash_len
			memcpy((uint8_t *)(buf+loop_idx), (uint8_t *)(digest), 16); loop_idx += 16; // hashed value
			memcpy((uint8_t *)(buf+loop_idx), (uint8_t *)(id), idlen); loop_idx += idlen; // id
			send_data_processing(0, buf, loop_idx);
			IINCHIP_WRITE(Sn_CR(0),Sn_CR_PCR);
			/* wait to process the command... */
			while( IINCHIP_READ(Sn_CR(0)) ) 
				;
			/* ------- */			
                        wait_delay_ms(1000);
		}
	}
	else
	{
		// printf("Not support\r\n");
#ifdef __DEF_IINCHIP_DBG__
		// printf("Not support PPP Auth type: %.2x%.2x\r\n",IINCHIP_READ(PATR0), IINCHIP_READ(PATR0+1));
#endif
		return 4;
	}
	// printf("\r\n");

	// printf("-- Waiting for PPPoE server's admission --");
	loop_idx = 0;
	while (!((isr = IINCHIP_READ(Sn_IR(0))) & Sn_IR_PNEXT))
	{
		if (isr & Sn_IR_PFAIL)
		{
   /* clear interrupt value*/
   IINCHIP_WRITE(Sn_IR(0), 0xff);
   /*---*/
			// printf("failed\r\nReinput id, password..\r\n");
			return 2;
		}
		// printf(".");
		if (loop_idx++ == 10) // timeout
		{
   /* clear interrupt value*/
   IINCHIP_WRITE(Sn_IR(0), 0xff);
   /*---*/
			// printf("timeout after PAP\r\n");
			return 3;
		}		
                wait_delay_ms(1000);
	}
   /* clear interrupt value*/
   IINCHIP_WRITE(Sn_IR(0), 0xff);
   /*---*/
	// printf("ok\r\n");
	// printf("\r\n");
	// printf("-- PHASE 4. IPCP process --");
	// IP Address
	buf[0] = 0x03; buf[1] = 0x06; buf[2] = 0x00; buf[3] = 0x00; buf[4] = 0x00; buf[5] = 0x00;
	send_data_processing(0, buf, 6);
	IINCHIP_WRITE(Sn_CR(0),Sn_CR_PCR);
	/* +20071122[chungs]:wait to process the command... */
	while( IINCHIP_READ(Sn_CR(0)) ) 
		;
	/* ------- */
	wait_delay_ms(1000);

	loop_idx = 0;
	while (1)
	{
		if (IINCHIP_READ(Sn_IR(0)) & Sn_IR_PRECV)
		{
   /* clear interrupt value*/
   IINCHIP_WRITE(Sn_IR(0), 0xff);
   /*---*/
			len = getSn_RX_RSR(0);
			if ( len > 0 )
			{
				recv_data_processing(0, str, len);
				IINCHIP_WRITE(Sn_CR(0),Sn_CR_RECV);
				/* wait to process the command... */
				while( IINCHIP_READ(Sn_CR(0)) ) 
					;
				/* ------- */
	   			//for debug
	   			//// printf("IPCP proc\r\n"); for (i = 0; i < len; i++) // printf ("%02x ", str[i]); // printf("\r\n");
	   			str_idx = 6; dst_idx = 0;
	   			if (str[2] == 0x03) // in case of NAK
	   			{
	   				do 
	   				{
	   					if (str[str_idx] == 0x03) // request only ip information
	   					{
	   						memcpy((uint8_t *)(buf+dst_idx), (uint8_t *)(str+str_idx), str[str_idx+1]);
	   						dst_idx += str[str_idx+1]; str_idx += str[str_idx+1];
	   					}
	   					else
	   					{
	   						// skip byte
	   						str_idx += str[str_idx+1];
	   					}
	   					// for debug
	   					//// printf("s: %d, d: %d, l: %d", str_idx, dst_idx, len);
	   				} while (str_idx != len);
	   				send_data_processing(0, buf, dst_idx);
	   				IINCHIP_WRITE(Sn_CR(0),Sn_CR_PCR); // send ipcp request
	   				/* wait to process the command... */
					while( IINCHIP_READ(Sn_CR(0)) ) 
						;
					/* ------- */
	   				wait_delay_ms(1000);
	   				break;
	   			}
			}
		}
		// printf(".");
		if (loop_idx++ == 10) // timeout
		{
			// printf("timeout after IPCP\r\n");
			return 3;
		}
		wait_delay_ms(1000);
		send_data_processing(0, buf, 6);
		IINCHIP_WRITE(Sn_CR(0),Sn_CR_PCR); //ipcp re-request
		/* wait to process the command... */
		while( IINCHIP_READ(Sn_CR(0)) ) 
			;
		/* ------- */
	}

	loop_idx = 0;
	while (!(IINCHIP_READ(Sn_IR(0)) & Sn_IR_PNEXT))
	{
		// printf(".");
		if (loop_idx++ == 10) // timeout
		{
			// printf("timeout after IPCP NAK\r\n");
			return 3;
		}
		wait_delay_ms(1000);
		IINCHIP_WRITE(Sn_CR(0),Sn_CR_PCR); // send ipcp request
		/* wait to process the command... */
		while( IINCHIP_READ(Sn_CR(0)) ) 
			;
		/* ------- */
	}
   /* clear interrupt value*/
   IINCHIP_WRITE(Sn_IR(0), 0xff);
   /*---*/
	// printf("ok\r\n");
	// printf("\r\n");
	return 1;
	// after this function, User must save the pppoe server's mac address and pppoe session id in current connection
}


/**
@brief	terminate PPPoE connection
*/
uint8_t pppterm(uint8_t * mac, uint8_t * sessionid)
{
	uint16_t i;
	uint8_t isr;
#ifdef __DEF_IINCHIP_DBG__
	// printf("pppterm()\r\n");
#endif
	/* Set PPPoE bit in MR(Common Mode Register) : enable socket0 pppoe */
	IINCHIP_WRITE(MR,IINCHIP_READ(MR) | MR_PPPOE);
	
	// write pppoe server's mac address and session id 
	// must be setted these value.
	for (i = 0; i < 6; i++) IINCHIP_WRITE((Sn_DHAR0(0)+i),mac[i]);
	for (i = 0; i < 2; i++) IINCHIP_WRITE((Sn_DPORT0(0)+i),sessionid[i]);
	isr = IINCHIP_READ(Sn_IR(0));
	IINCHIP_WRITE(Sn_IR(0),isr);
	
	//open socket in pppoe mode
	IINCHIP_WRITE(Sn_MR(0),Sn_MR_PPPOE);
	IINCHIP_WRITE(Sn_CR(0),Sn_CR_OPEN);
	/* wait to process the command... */
	while( IINCHIP_READ(Sn_CR(0)) ) 
		;
	/* ------- */
	wait_1us(1);
	// close pppoe connection
	IINCHIP_WRITE(Sn_CR(0),Sn_CR_PDISCON);
	/* wait to process the command... */
	while( IINCHIP_READ(Sn_CR(0)) ) 
		;
	/* ------- */
	wait_delay_ms(1000);
	// close socket	
	close(0);
	/* ------- */
	

#ifdef __DEF_IINCHIP_DBG__
	// printf("pppterm() end ..\r\n");
#endif

	return 1;
}
#endif
