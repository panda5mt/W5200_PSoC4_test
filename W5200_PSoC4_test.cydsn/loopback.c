#include "config.h"
#include "w5200.h"
#include "socket.h"
#include "util.h"
#include "loopback.h"
#include <stdio.h>

#define tick_second 1

extern uint8_t ch_status[MAX_SOCK_NUM];
extern CHCONFIG_TYPE_DEF Chconfig_Type_Def; 
extern uint32_t presentTime;
uint16_t any_port = 1000;


void loopback_tcps(SOCKET s, uint16_t port)
{	        
    uint16_t RSR_len;
    uint16_t received_len;
    uint8_t * data_buf = TX_BUF;
    uint16_t sent_data_len = 0;
    uint8_t tmp_retry_cnt = 0;        
	
	switch (getSn_SR(s))
	{
	case SOCK_ESTABLISHED:					/* if connection is established */
		if(ch_status[s]==1)
		{
			// printf("\r\n%d : Connected",s);
                        // printf("\r\n Peer IP : %d.%d.%d.%d", IINCHIP_READ(Sn_DIPR0(s)+0),  IINCHIP_READ(Sn_DIPR0(s)+1), IINCHIP_READ(Sn_DIPR0(s)+2), IINCHIP_READ(Sn_DIPR0(s)+3));
                        // printf("\r\n Peer Port : %d", ( (uint16)(IINCHIP_READ(Sn_DPORT0(s)+0)<<8) +(uint16)IINCHIP_READ(Sn_DPORT0(s)+1)));
			ch_status[s] = 2;
		}
        
        if ((RSR_len = getSn_RX_RSR(s)) > 0) 			/* check Rx data */
		{
			if (RSR_len > TX_RX_MAX_BUF_SIZE) RSR_len = TX_RX_MAX_BUF_SIZE;	/* if Rx data size is lager than TX_RX_MAX_BUF_SIZE */
																		    /* the data size to read is MAX_BUF_SIZE. */
			received_len = recv(s, data_buf, RSR_len);   	/* read the received data */

            sent_data_len = send(s, data_buf, received_len, (bool)WINDOWFULL_FLAG_OFF);         /* sent the received data */
            if(sent_data_len != received_len) /* only assert when windowfull */
            {
                init_windowfull_retry_cnt(s);                              
                while(sent_data_len !=  received_len)
                {
                    // printf("ret :%d\r\n", sent_data_len);
                    tmp_retry_cnt = incr_windowfull_retry_cnt(s);
                        
                    if(tmp_retry_cnt <= WINDOWFULL_MAX_RETRY_NUM) 
                    {
                        sent_data_len += send(s, data_buf, received_len, (bool)WINDOWFULL_FLAG_ON); 
                        wait_delay_ms(WINDOWFULL_WAIT_TIME);
                    }else
                    {
                        // printf("WindowFull !!!\r\n");
                        close(s);
                        // printf("Socket Close !!!\r\n");
                        while(1);  // if the 'Windowfull' occers, socket close and sending process stop.
                                   // user can be customize(WINDOWFULL_MAX_RETRY_NUM, Windowfull handling ...)
                    }
                }                               
            }
            	
		}                             
		break;
        
	case SOCK_CLOSE_WAIT:                           	/* If the client request to close */
		// printf("\r\n%d : CLOSE_WAIT", s);
                if ((RSR_len = getSn_RX_RSR(s)) > 0) 		/* check Rx data */
		{
			if (RSR_len > TX_RX_MAX_BUF_SIZE) RSR_len = TX_RX_MAX_BUF_SIZE;	/* if Rx data size is lager than TX_RX_MAX_BUF_SIZE */
																												/* the data size to read is MAX_BUF_SIZE. */
			received_len = recv(s, data_buf, RSR_len);		/* read the received data */
		}
		disconnect(s);
		ch_status[s] = 0;
		break;
	case SOCK_CLOSED:                                       /* if a socket is closed */
		if(!ch_status[s]) 
		{
			// printf("\r\n%d : Loop-Back TCP Server Started. port : %d", s, port);
			ch_status[s] = 1;
		}                
		if(socket(s,Sn_MR_TCP,port,0x00) == 0)    /* reinitialize the socket */
		{
			// printf("\r\n%d : Fail to create socket.",s);
			ch_status[s] = 0;
		}                                			                
		break;
        case SOCK_INIT:   /* if a socket is initiated */
                listen(s); 
                break;
        default:
                break;                
	}
}

void loopback_tcpc(SOCKET s, uint16_t port)
{							
        uint16_t RSR_len;
        uint16_t received_len;
	uint8_t * data_buf = TX_BUF;
        uint16_t sent_data_len = 0;
        uint8_t tmp_retry_cnt = 0;              
      
	switch (getSn_SR(s))
	{
	case SOCK_ESTABLISHED:						/* if connection is established */                
		if(ch_status[s]==1)
		{
			// printf("\r\n%d : Connected",s);			
			ch_status[s] = 2;
		}
		if ((RSR_len = getSn_RX_RSR(s)) > 0) 			/* check Rx data */
		{
			if (RSR_len > TX_RX_MAX_BUF_SIZE) RSR_len = TX_RX_MAX_BUF_SIZE;	/* if Rx data size is lager than TX_RX_MAX_BUF_SIZE */
									/* the data size to read is MAX_BUF_SIZE. */
			received_len = recv(s, data_buf, RSR_len);			/* read the received data */
			
                        sent_data_len = send(s, data_buf, received_len, (bool)WINDOWFULL_FLAG_OFF);         /* sent the received data */
                        if(sent_data_len != received_len) /* ohly assert when windowfull */
                        {
                                  init_windowfull_retry_cnt(s);                              
                                  while(sent_data_len !=  received_len) 
                                  {
                                            // printf("ret :%d\r\n", sent_data_len);
                                            tmp_retry_cnt = incr_windowfull_retry_cnt(s);
                                            
                                            if(tmp_retry_cnt <= WINDOWFULL_MAX_RETRY_NUM) 
                                            {
                                                       sent_data_len += send(s, data_buf, received_len, (bool)WINDOWFULL_FLAG_ON); 
                                                       wait_delay_ms(WINDOWFULL_WAIT_TIME);
                                             } 
                                            else 
                                            {
                                                       // printf("WindowFull !!!\r\n");
                                                       close(s);
                                                       // printf("Socket Close !!!\r\n");
                                                       while(1);
                                            }
                                  }                               
                        }                        
		}
		break;
	case SOCK_CLOSE_WAIT:                           		/* If the client request to close */
		// printf("\r\n%d : CLOSE_WAIT", s);
		if ((RSR_len = getSn_RX_RSR(s)) > 0) 			/* check Rx data */
		{
			if (RSR_len > TX_RX_MAX_BUF_SIZE) RSR_len = TX_RX_MAX_BUF_SIZE;	/* if Rx data size is lager than TX_RX_MAX_BUF_SIZE */
																												/* the data size to read is MAX_BUF_SIZE. */
			received_len = recv(s, data_buf, RSR_len);			/* read the received data */
		}
		disconnect(s);
		ch_status[s] = 0;
		break;
	case SOCK_CLOSED:                                               /* if a socket is closed */               
		if(!ch_status[s])
		{
			// printf("\r\n%d : Loop-Back TCP Client Started. port: %d", s, port);
			ch_status[s] = 1;
		}		
		if(socket(s, Sn_MR_TCP, port, 0x00) == 0)    /* reinitialize the socket */
		{
			// printf("\a%d : Fail to create socket.",s);
			ch_status[s] = 0;
		}
		break;
        case SOCK_INIT:     /* if a socket is initiated */
                if(time_return() - presentTime >= (tick_second * 3)) {  /* For TCP client's connection request delay : 3 sec */
                        connect(s, Chconfig_Type_Def.destip, Chconfig_Type_Def.port); /* Try to connect to TCP server(Socket, DestIP, DestPort) */
                        presentTime = time_return();                       
                }                
                break;  
        default:
                break;        
        }
}

void loopback_udp(SOCKET s, uint16_t port)
{
	uint16_t RSR_len;
        uint16_t received_len;
	uint8_t * data_buf = TX_BUF;
	uint32 destip = 0;
	uint16_t destport;

	switch (getSn_SR(s))
	{
	case SOCK_UDP:
		if ((RSR_len = getSn_RX_RSR(s)) > 0) 			/* check Rx data */
		{
			if (RSR_len > TX_RX_MAX_BUF_SIZE) RSR_len = TX_RX_MAX_BUF_SIZE;	/* if Rx data size is lager than TX_RX_MAX_BUF_SIZE */
																												/* the data size to read is MAX_BUF_SIZE. */
			received_len = recvfrom(s, data_buf, RSR_len, (uint8*)&destip, &destport);			/* read the received data */
			if(sendto(s, data_buf, received_len,(uint8*)&destip, destport) == 0)	/* send the received data */
			{
				// printf("\a\a\a%d : System Fatal Error.", s);
			}
		}
		break;
	case SOCK_CLOSED:                                               /* if a socket is closed */
		// printf("\r\n%d : Loop-Back UDP Started. port :%d", s, port);
		if(socket(s,Sn_MR_UDP,port,0x00)== 0)    /* reinitialize the socket */
			// printf("\a%d : Fail to create socket.",s);
		break;
	}
}

