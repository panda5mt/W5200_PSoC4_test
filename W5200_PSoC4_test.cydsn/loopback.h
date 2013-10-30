#ifndef _LOOPBACK_H
#define _LOOPBACK_H

#include "util.h"

void loopback_tcps(SOCKET s, uint16_t port);
void loopback_tcpc(SOCKET s, uint16_t port);
void loopback_udp(SOCKET s, uint16_t port);

#endif
