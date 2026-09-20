/*
 * sb_recvfile.h
 *
 *  Created on: 04/07/2012
 *      Author: jester
 *
 * Licensed under the terms of the GNU GPL, version 2
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 */

#ifndef _SB_RECVFILE_H_
#define _SB_RECVFILE_H_

#include "types.h"

#define SYN  0x61
#define ACK  0x62
#define E0F  0x63
#define CHK  0x64

int handshake();
int fileReceive(u8 *dest, int destsize);

#endif
