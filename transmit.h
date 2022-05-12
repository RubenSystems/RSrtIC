//
//  observe.h
//  NetworkingTest
//
//  Created by Ruben Ticehurst-James on 29/04/2022.
//

#include "models.h"

/**
 This transmits data in the from of a char array from one computer to another.
 
 - parameter Computer: The computer to send from. If operating as a client, should be created by createComputer. If operating as a server should be thisComputer.
 - parameter Client: the computer to send the data to. This can be taken from a client manager or be created from createComputer.
 - parameter Data: The data as a const unsigned char .
 - parameter Data Size: The size of the data.
 - returns: void
 
 # Notes: #
 1. The data size should not be larger then 255 * the packet size due to the indexing process. if it is, change the index size to an unsigned short or int in the config
 */
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
