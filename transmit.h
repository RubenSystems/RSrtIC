//
//  observe.h
//  NetworkingTest
//
//  Created by Ruben Ticehurst-James on 29/04/2022.
//

#include "models.h"

void transmitData(
				  struct Computer *,
				  struct Computer *,
				  const unsigned char *,
				  int
);


void transmitPacket(
					struct Computer *,
					struct Computer *,
					struct Packet *
);

enum TransmitResponses transmit(
			  struct Computer *,
			  struct Computer *,
			  const char *,
			  int
);

enum TransmitResponses {
	SUCCESS = 0, 
	EVICT = -2, 
	FAIL = -1
};
