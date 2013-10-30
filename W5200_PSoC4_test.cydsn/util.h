/*
*
@file		util.h
@brief	
*/

#ifndef _UTIL_H
#define _UTIL_H


#include "device_depend.h"

#define	MAX_SOCK_NUM		4	/**< Maxmium number of socket  */

/**
@brief	 __DEF_IINCHIP_MAP_xxx__ : define memory map for iinchip 
*/
#define __DEF_IINCHIP_MAP_BASE__ 0x0000
 #define COMMON_BASE 0x0000
#define __DEF_IINCHIP_MAP_TXBUF__ (COMMON_BASE + 0x8000) /* Internal Tx buffer address of the iinchip */
#define __DEF_IINCHIP_MAP_RXBUF__ (COMMON_BASE + 0xC000) /* Internal Rx buffer address of the iinchip */
#define __DEF_IINCHIP_PPP

#define IINCHIP_ISR_DISABLE()
#define IINCHIP_ISR_ENABLE()	

#ifndef NULL
#define NULL		((void *) 0)
#endif

//typedef enum { false, true } bool;

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned int size_t;
#endif

typedef volatile char vint8_t;
typedef volatile unsigned char vuint8_t;

typedef uint8_t     SOCKET;
    
    
void Set_network();
void Reset_W5200(void);

void LED3_onoff(uint8_t on_off);
void LED4_onoff(uint8_t on_off);

void USART1_Init(void);

//void Delay_us(uint8_t time_us);
//void wait_delay_ms(uint16_t time_ms);

//int putchar(int ch);
//int getchar(void);

uint32_t time_return(void);

#endif
