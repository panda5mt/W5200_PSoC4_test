/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "device_depend.h"
#include <project.h>

void hardware_init(void){
	uart_init();
	SPI_CS_Write(1);
}

void uart_init(void){
	UART_1_Start();
}

void uart_print(char *ss){
	UART_1_UartPutString(ss);
}

void spi_master_init(void){
	SPI_Start();
}

char spi_master_send(char send_data){
	SPI_SpiUartWriteTxData(send_data);
	while( !SPI_SpiUartGetRxBufferSize() );
	
	return ((char)SPI_SpiUartReadRxData());
}

void spi_cs_low(void){
	SPI_CS_Write(0);
}

void spi_cs_high(void){
	SPI_CS_Write(1);
}

void wait_delay_ms(int time){
	CyDelay(time);
}
/* [] END OF FILE */
