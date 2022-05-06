//
//  client.c
//  NetworkingTest
//
//  Created by Ruben Ticehurst-James on 11/04/2022.
//

#include "../models.h"


#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>


void terminate(void * object) {
	free(object);
	object = 0;
}

/**
 * danger functions should be freed to avoid memory leaks
 */

struct Computer * createComputer(const char * ip, const char * port) {
	struct Computer* computer = (struct Computer *)malloc(sizeof(struct Computer));
	// computer->address.sin_family = AF_INET;
	// computer->address.sin_port = htons(port);
	// computer->address.sin_addr = *((struct in_addr *)gethostbyname(ip)->h_addr);
	// memset(&(computer->address.sin_zero), '\0', 8);

	struct addrinfo hints, *servinfo, *p;
	int rv, sockfd;
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET; // set to AF_INET to use IPv4
	hints.ai_socktype = SOCK_DGRAM;


	if ((rv = getaddrinfo(ip, port, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return NULL;
	}

	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("talker: socket");
			continue;
		} else {
			computer->address = *p;
			computer->fd = sockfd;
			break;
		}
	}

	return computer;
}


//Will bind the socket!!
struct Computer * thisComputer(const char * port) {
	printf("%s -- port \n", port);
	struct Computer * computer = (struct Computer *)malloc(sizeof(struct Computer));
	int rv, sockfd;
	struct addrinfo hints, *servinfo, *p;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET; 
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; // use my IP



	if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return NULL;
	}



	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {

		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("listener: socket");
			continue;
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			
			close(sockfd);
			perror("listener: bind");
			continue;
		} else {
			computer->fd = sockfd;
			computer->address = *p;
			break;
		}

		
	}
	if (p == NULL){
		printf("FAILURE TO GET SOCKET\n");
	}

	

	freeaddrinfo(servinfo);
	return computer;
}

struct Computer * anyComputer() {
	struct Computer * computer = (struct Computer *)malloc(sizeof(struct Computer));
	memset(&computer->address, 0, sizeof(computer->address));
	if ((computer->fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	return computer;
}


// struct Socket * createSocket(){
// 	struct Socket * sock = (struct Socket *)malloc(sizeof(struct Socket));
// 	if ((sock->fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
// 		perror("socket");
// 		exit(1);
// 	}
// 	return sock;
// };



void createFrameInPlace(struct Frame * frame, struct Packet * packet) {
	frame->packetno = packet->completion == 1 ? packet->index : -1;
	frame->packetcount = 1;
	frame->frameID = packet->packetID;
	frame->packets[packet->index] = *packet;
	memmove(&(frame->packets), packet, sizeof(struct Packet));
}

struct Pool createPool() {
	struct Pool pool;
	pool.frames = (struct Frame *)malloc(sizeof(struct Frame) * FRAME_POOL_SIZE);
	return pool;
}

// void bindSocketToComputer(struct Socket * socket, struct Computer * computer) {

// 	int optval = 
// 	if((setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))) < 0) {
// 	  perror("setsockopt");
// 	  return EXIT_FAILURE;
// 	}


// 	if ( bind(socket->fd, (const struct sockaddr *)&computer->address, sizeof(computer->address)) < 0 ) {
// 		perror("bind failed");
// 		exit(EXIT_FAILURE);
// 	}
// } 







int insertPacketIntoPool(struct Pool * pool, struct Packet * packet) {
	int lastEmptySpot = -1;
	for (int i = 0; i < FRAME_POOL_SIZE; i ++) {
		if (pool->frames[i].frameID != packet->packetID && lastEmptySpot == -1) {
			lastEmptySpot = i;
		} else if (pool->frames[i].frameID == packet->packetID && pool->frames[i].frameID == packet->packetID) {
			if (pool->frames[i].packetno == -1 && packet->completion == 1) {
				pool->frames[i].packetno = packet->index;
			}
			memmove(&(pool->frames[i].packets[packet->index]), packet, sizeof(struct Packet));
			return (++pool->frames[i].packetcount == pool->frames[i].packetno + 1 && pool->frames[i].packetno != -1) ? i : -1;
		}
	}

	// TODO: - don't malloc every time dummy
	// TODO: - add init code elsewhere
//	struct Frame * frame = (struct Frame*)malloc(sizeof(struct Frame));
//	frame->packetno = packet->completion == 1 ? packet->index : -1;
//	frame->packetcount = 1;
//	frame->frameID = packet->packetID;
//	frame->packets[packet->index] = *packet;
//	memmove(&(frame->packets), packet, sizeof(struct Packet));
//	pool->frames[lastEmptySpot] = frame;
	createFrameInPlace(&(pool->frames[lastEmptySpot]), packet);
	return -1;
}

void joinFrame(struct ContentBuffer * buffer, struct Frame * frame){
	
	for(int i = 0; i < frame->packetno + 1; i ++) {
		memmove(&(buffer->data[buffer->latestPosition ]), &(frame->packets[i].data), frame->packets[i].size);
		buffer->latestPosition += frame->packets[i].size;
	}
}



