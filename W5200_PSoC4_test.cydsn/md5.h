/**
 @file		md5.h
 */

#ifndef __MD5_H
#define __MD5_H

#include "util.h"
/**
 @brief	MD5 context. 
 */
typedef struct {
	uint32_t state[4];    /**< state (ABCD)                            */
	uint32_t count[2];    /**< number of bits, modulo 2^64 (lsb first) */
	uint8_t  buffer[64];  /**< input buffer                            */
} md5_ctx;

extern void md5_init(md5_ctx *context);
extern void md5_update(md5_ctx *context, uint8_t *buffer, uint32_t length);
extern void md5_final(uint8_t result[16], md5_ctx *context);

#endif	// __md5_H
