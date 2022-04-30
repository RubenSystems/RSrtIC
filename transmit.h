//
//  observe.h
//  NetworkingTest
//
//  Created by Ruben Ticehurst-James on 29/04/2022.
//



#include <stdio.h>
#include "models.h"


int PACKET_ID_COUNTER;

void transmitPacket(struct Socket, struct Computer, struct Packet);


void transmit(struct Socket, struct Computer, const char *, int);