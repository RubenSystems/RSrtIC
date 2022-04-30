//
//  observe.h
//  NetworkingTest
//
//  Created by Ruben Ticehurst-James on 28/04/2022.
//


#include <stdio.h>
#include "models.h"

void observe(struct Socket * socket, struct Computer * computer, const void *, void (*completion)(const void *, const char *, int) );
void recieveOnce(struct Socket * socket, struct Computer * computer, struct Packet *, struct Buffer * intermediateBuffer);

