#include "stdio.h"
#include "config.h"
#include "util.h"
#include "w5200.h"
#include "socket.h"
#include "device_depend.h"
#include "loopback.h"

#include <string.h>
#include <stdlib.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

CONFIG_MSG Config_Msg;
CHCONFIG_TYPE_DEF Chconfig_Type_Def; 

// Configuration Network Information of W5200
uint8_t Enable_DHCP = OFF;
uint8_t MAC[6] = {0x00, 0x50, 0xC2, 0x97, 0x29, 0x04};//MAC Address
uint8_t IP[4] = {192, 168, 1, 70};//IP Address
uint8_t GateWay[4] = {192, 168, 1, 121};//Gateway Address
uint8_t SubNet[4] = {255, 255, 255, 0};//SubnetMask Address

//TX MEM SIZE- SOCKET 0:8KB, SOCKET 1:2KB, SOCKET2-7:1KB
//RX MEM SIZE- SOCKET 0:8KB, SOCKET 1:2KB, SOCKET2-7:1KB
uint8_t txsize[MAX_SOCK_NUM] = {2,2,2,2};
uint8_t rxsize[MAX_SOCK_NUM] = {2,2,2,2};

//FOR TCP Client
//Configuration Network Information of TEST PC
uint8_t Dest_IP[4] = {192, 168, 1, 100}; //DST_IP Address 
uint16_t Dest_PORT = 3000; //DST_IP port

uint8_t ch_status[MAX_SOCK_NUM] = { 0, };	/** 0:close, 1:ready, 2:connected */

uint8_t TX_BUF[TX_RX_MAX_BUF_SIZE]; // TX Buffer for applications
uint8_t RX_BUF[TX_RX_MAX_BUF_SIZE]; // RX Buffer for applications
   
void WIZ_Config(void)
{
  // Call Set_network(): Set network configuration, Init. TX/RX MEM SIZE., and  Set RTR/RCR
  Set_network(); // at util.c
}       
//__IO uint32_t Timer2_Counter;

uint32_t my_time;
uint32_t presentTime;

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void NVIC_Configuration(void);
void GPIO_Configuration(void);
void Timer_Configuration(void);


/* Private functions ---------------------------------------------------------*/

//void Timer2_ISR(void)
//{
//	if (Timer2_Counter++ > 1000) { // 1m x 1000 = 1sec
//		Timer2_Counter = 0;
//		my_time++;
//		
//	}
//	
//}


/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{  
//	RCC_Configuration(); // Configure the system clocks
//	NVIC_Configuration(); // NVIC Configuration
//	GPIO_Configuration();
//	USART1_Init();
//	Timer_Configuration();
        
	Reset_W5200();
	spi_master_init();	
	WIZ_Config(); // network config & Call Set_network ();      
        
        // LED3 and LED4 On!
	//LED3_onoff(ON);
	//LED4_onoff(ON);
        
        presentTime = my_time; // For TCP client's connection request delay
        
        // Start Application         
        // printf("\r\n\r\n------------------------------------------- \r\n");
	    // printf("TCP Server Loopback using W5200\r\n");  
        // printf("------------------------------------------- ");
	while(1) {
		loopback_tcps(0, 23);
//		loopback_tcps(1, 5001);
//		loopback_tcps(2, 5002);
//		loopback_tcps(3, 5003);		
//                loopback_tcpc(4, 5004);              
//		loopback_udp(5, 3000);
//		loopback_udp(6, 3001);
//		loopback_udp(7, 3002);                
	}
        
}

void GPIO_Configuration(void)
{
//	GPIO_InitTypeDef GPIO_InitStructure;
//
//    // Port A output
//    GPIO_InitStructure.GPIO_Pin = WIZ_SCS | LED3 | LED4; 
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);
//    GPIO_SetBits(GPIOA, WIZ_SCS);
//    GPIO_SetBits(GPIOA, LED3); // led off
//    GPIO_SetBits(GPIOA, LED4); // led off
//	
//
//    // Configure the GPIO ports( USART1 Transmit and Receive Lines)
//    // Configure the USART1_Tx as Alternate function Push-Pull 
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//    GPIO_InitStructure.GPIO_Pin =  USART1_TX;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);
//    
//    // Configure the USART1_Rx as input floating
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//    GPIO_InitStructure.GPIO_Pin = USART1_RX;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);
//  
//    // SPI 1
//    /* Configure SPIy pins: SCK, MISO and MOSI */
//    GPIO_InitStructure.GPIO_Pin = WIZ_SCLK | WIZ_MISO | WIZ_MOSI;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//
//    // Port B
//    GPIO_InitStructure.GPIO_Pin = WIZ_RESET|WIZ_PWDN; 
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//    GPIO_Init(GPIOB, &GPIO_InitStructure);
//    GPIO_SetBits(GPIOB, WIZ_RESET);
//    GPIO_ResetBits(GPIOB, WIZ_PWDN); 
//
//    // Port B input
//    GPIO_InitStructure.GPIO_Pin = WIZ_INT;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//    GPIO_Init(GPIOB, &GPIO_InitStructure);

}


/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration(void)
{
//ErrorStatus HSEStartUpStatus;
//  
// /* RCC system reset(for debug purpose) */
//  RCC_DeInit();
//
//  /* Enable HSE */
//  RCC_HSEConfig(RCC_HSE_ON);
//
//  /* Wait till HSE is ready */
//  HSEStartUpStatus = RCC_WaitForHSEStartUp();
//
//  if(HSEStartUpStatus == SUCCESS)
//  {
//    /* Enable Prefetch Buffer */
//    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
//
//    /* Flash 2 wait state */
//    FLASH_SetLatency(FLASH_Latency_2);
// 
//    /* HCLK = SYSCLK */
//    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
//  
//    /* PCLK2 = HCLK */
//    RCC_PCLK2Config(RCC_HCLK_Div1); 
//
//    /* PCLK1 = HCLK/2 */
//    RCC_PCLK1Config(RCC_HCLK_Div2);
//
//    /* PLLCLK = 8MHz * 9 = 72 MHz */
//    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
//
//    /* Enable PLL */ 
//    RCC_PLLCmd(ENABLE);
//
//    /* Wait till PLL is ready */
//    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
//    {
//    }
//
//    /* Select PLL as system clock source */
//    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
//
//    /* Wait till PLL is used as system clock source */
//    while(RCC_GetSYSCLKSource() != 0x08)
//    {
//    }
//  }
//   
//  /* TIM2 clock enable */
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
//
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC
//  											|RCC_APB2Periph_AFIO  | RCC_APB2Periph_USART1, ENABLE);
//


}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//void NVIC_Configuration(void)
//{
//  NVIC_InitTypeDef NVIC_InitStructure;
//
//#ifdef  VECT_TAB_RAM  
//  /* Set the Vector Table base location at 0x20000000 */ 
//  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
//
//#else  /* VECT_TAB_FLASH  */
//  /* Configure one bit for preemption priority */
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
//
//  /* Enable the USART1 Interrupt */
//  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//
//  /* Enable the USART2 Interrupt */
//  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//  
//
//  /* Enable the TIM2 global Interrupt */
//  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//
//   /* Enable the TIM2 global Interrupt */
//  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//  
//  /* Set the Vector Table base location at 0x08000000 */ 
//NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
//  
//  /* Set the Vector Table base location at 0x08002000 -> USE AIP*/ 
//  // NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x2000);  
//  // NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x4000);  
//#endif
//}



//void Timer_Configuration(void)
//{
//
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//
//  /* Time base configuration */	
//  TIM_TimeBaseStructure.TIM_Period = 1000;          
//  TIM_TimeBaseStructure.TIM_Prescaler = 0;      
//  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;    
//  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
//  
//  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
//
//  /* Prescaler configuration */
//  TIM_PrescalerConfig(TIM2, 71, TIM_PSCReloadMode_Immediate);
//
//  /* TIM enable counter */
//  TIM_Cmd(TIM2, ENABLE);
//
//  /* TIM IT enable */
//  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);  
//
//}





