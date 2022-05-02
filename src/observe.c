//
//  observe.c
//  NetworkingTest
//
//  Created by Ruben Ticehurst-James on 28/04/2022.
//

#include "../observe.h"


#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>


void observe(struct Socket * socket, struct Computer * computer, struct ClientManager * clientManager, void (*completion)(const char *, int) ) {
	struct Packet temp;
	struct Pool pool = createPool();
	struct ContentBuffer contentBuffer;
	
	struct Buffer intermediateBuffer;


	contentBuffer.latestPosition = 0;
	
	int prevIndex = 0;
	int index = 0;
	
	while (1) {
		switch (recieveOnce(socket, computer, &temp, &intermediateBuffer)) {
			case OPEN:
				// HANDLE PING
				addClient(clientManager, computer);
				break;
			case PING:
				// HANDLE PING
				break; 
			case DATA:
				prevIndex = temp.index;
				index = insertPacketIntoPool(&pool, &temp);
				if (index >= 0) {
					joinFrame(&contentBuffer, &(pool.frames[index]));
					completion(contentBuffer.data, contentBuffer.latestPosition);
					contentBuffer.latestPosition = 0;
				}
				break;
		}
	}
}

// void observeWithContext(struct Socket * socket, struct Computer * computer, const void * context, void (*completion)(const void * context, const char *, int) ) {
// 	void callback(const char * data, int size) {
// 		completion(context, data, size);
// 	}

// 	observe(socket, computer, callback);
// }



//Will return 0 if it is a ping packet and 1 if it is a data packet 
enum MessageTypes recieveOnce(struct Socket * socket, struct Computer * computer, struct Packet * packet, struct Buffer * intermediateBuffer) {
//	memset(GlobalPacketBuffer.data, 0, PACKET_SIZE * sizeof(char));
	
	socklen_t addr_len = sizeof(struct sockaddr);
	long numbytes;
	
	if ((numbytes = recvfrom(socket->fd, intermediateBuffer->data, PACKET_SIZE , 0, (struct sockaddr *)&computer->address, &addr_len)) < 0) {
		perror("recvfrom");
		exit(1);
	}

	
	intermediateBuffer->data[numbytes] = '\0';

	const char * openString = "open";
	const char * pingString = "ping";
	if (strcmp(intermediateBuffer->data, openString) == 0 ) {
		return OPEN;
	} else if (strcmp(intermediateBuffer->data, pingString) == 0 ) {
		return PING;
	}
	/*
	 This code is rubbish and only a fool would have written it.
	 */
	
	memmove(&(packet->index), intermediateBuffer->data, 1);
	memmove(&(packet->packetID), &(intermediateBuffer->data[1]), 1);
	memmove(&(packet->completion), &(intermediateBuffer->data[2]), 1);
	memmove(&(packet->data), &(intermediateBuffer->data[4]), numbytes - 3);
	
	packet->size = (int)(numbytes - 3);
	return DATA;
}
