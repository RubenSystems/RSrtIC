//
//  observe.c
//  NetworkingTest
//
//  Created by Ruben Ticehurst-James on 28/04/2022.
//

#include "../observe.h"
#include "../transmit.h"

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
#include <poll.h>

void observeWithContext(struct Computer * fdComputer, struct ClientManager * manager, enum ComputerType type, const void * context, void (*completion)(const void * context, const char *, int) ) {
	struct Computer * client = anyComputer();
	struct Packet temp;
	struct Pool pool = createPool();
	struct ContentBuffer contentBuffer;
	
	struct Buffer intermediateBuffer;


	contentBuffer.latestPosition = 0;
	
	int prevIndex = 0;
	int index = 0;
	int sendIndex = 0;
	
	while (1) {
		switch (recieveOnce(fdComputer, client, type, &temp, &intermediateBuffer)) {
			case OPEN:
				addClient(manager, client);
				break;
			case PING:
				addClient(manager, client);
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
			case TIMEOUT:
				if (type == CLIENT) {
					transmitData(fdComputer, fdComputer, (const unsigned char *)OPEN_MESSAGE, strlen(OPEN_MESSAGE));
				}
				break;
		}
		if (type == CLIENT && sendIndex ++ == PING_AFTER) {
			transmitData(fdComputer, fdComputer, (const unsigned char *)PING_MESSAGE, strlen(PING_MESSAGE));
			sendIndex = 0;
		}
	}
}

void observe(struct Computer * computer, struct ClientManager * manager, enum ComputerType type, void (*completion)(const void * context, const char *, int) ) {
	observeWithContext(computer, manager, type, 0, completion);
}


//Will return 0 if it is a ping packet and 1 if it is a data packet 
enum MessageTypes recieveOnce(struct Computer * computer, struct Computer * client, enum ComputerType type, struct Packet * packet, struct Buffer * intermediateBuffer) {
//	memset(GlobalPacketBuffer.data, 0, PACKET_SIZE * sizeof(char));
	

	
	socklen_t addr_len = sizeof(struct sockaddr);
	long numbytes;
	
	if (type == CLIENT) {
		fd_set readfds;
		struct timeval timeout = { .tv_sec = 1 };
		FD_ZERO(&readfds);
		FD_SET(computer->fd, &readfds);
		select(computer->fd + 1, &readfds, NULL, NULL, &timeout);
		if (!FD_ISSET(computer->fd, &readfds)) {
			return TIMEOUT;
		}
	}
		
	if ((numbytes = recvfrom(computer->fd, intermediateBuffer->data, PACKET_SIZE , 0, (struct sockaddr *)&client->inferredaddress, &addr_len)) < 0) {
		perror("recv");
		exit(1);
	}
	
	intermediateBuffer->data[numbytes] = '\0';


	if (strcmp(intermediateBuffer->data, OPEN_MESSAGE) == 0 ) {
		return OPEN;
	} else if (strcmp(intermediateBuffer->data, PING_MESSAGE) == 0 ) {
		return PING;
	}

	memmove(&(packet->rawdata), &(intermediateBuffer->data), numbytes);
	packet->size = (int)(numbytes - 3);
	return DATA;
}


