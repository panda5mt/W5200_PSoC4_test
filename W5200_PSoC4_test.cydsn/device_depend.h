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
#include <Cytypes.h>

void hardware_init(void);
void uart_init(void);
void uart_print(char *ss);
void spi_master_init(void);
char spi_master_send(char send_data);
void wait_delay_ms(int time);
void spi_cs_low(void);
void spi_cs_high(void);
/* [] END OF FILE */
