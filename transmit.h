//
//  observe.h
//  NetworkingTest
//
//  Created by Ruben Ticehurst-James on 29/04/2022.
//

#include "models.h"

char PACKET_ID_COUNTER;
char buffer [1500];


void transmitPacket(struct Socket *, struct Computer *, struct Packet *);

void transmit(struct Socket *, struct Computer *, const char *, int);