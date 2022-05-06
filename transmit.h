//
//  observe.h
//  NetworkingTest
//
//  Created by Ruben Ticehurst-James on 29/04/2022.
//

#include "models.h"

void transmitData(struct Computer *, const unsigned char *, int);

void transmitPacket(struct Computer *, struct Packet *);

void transmit(struct Computer *, const char *, int);
