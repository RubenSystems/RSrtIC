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
#include <time.h>



void observeWithContext(struct Computer * fdComputer, struct ClientManager * manager, const void * context, void (*completion)(const void * context, const char *, int) ) {
	struct Client * client = anyClient();
	struct Packet temp;
	struct Pool pool = createPool();
	struct ContentBuffer contentBuffer;
	
	struct Buffer intermediateBuffer;


	contentBuffer.latestPosition = 0;
	
	int prevIndex = 0;
	int index = 0;
	
	while (1) {
		switch (recieveOnce(fdComputer, client, &temp, &intermediateBuffer)) {
			case OPEN:
				addClient(manager, client);
				break;
			case PING:
				// HANDLE PING
				break;
			case DATA:
				prevIndex = temp.index;
				index = insertPacketIntoPool(&pool, &temp);
				if (index >= 0) {
					joinFrame(&contentBuffer, &(pool.frames[index]));
					completion(context, contentBuffer.data, contentBuffer.latestPosition);
					contentBuffer.latestPosition = 0;
				}
				break;
		}
	}
}

void observe(struct Computer * computer, struct ClientManager * manager, void (*completion)(const void * context, const char *, int) ) {
	observeWithContext(computer, manager, 0, completion);
}


//Will return 0 if it is a ping packet and 1 if it is a data packet 
enum MessageTypes recieveOnce(struct Computer * computer, struct Client * client, struct Packet * packet, struct Buffer * intermediateBuffer) {
//	memset(GlobalPacketBuffer.data, 0, PACKET_SIZE * sizeof(char));
	socklen_t addr_len = sizeof(struct sockaddr);
	long numbytes;
	
	if ((numbytes = recvfrom(computer->fd, intermediateBuffer->data, PACKET_SIZE , 0, (struct sockaddr *)&client->address, &addr_len)) < 0) {
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

	memmove(&(packet->rawdata), &(intermediateBuffer->data), numbytes);
	packet->size = (int)(numbytes - 3);
	return DATA;
}
