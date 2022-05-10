//
//  client.h
//  NetworkingTest
//
//  Created by Ruben Ticehurst-James on 11/04/2022.
//

#pragma once

#include "config.h"

// #include <sys/types.h>
// #include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

/*
 MARK: - RAW DATA
 */
struct Packet {
	union {
		struct {
			PACKET_DATA_TYPE index;
			PACKET_DATA_TYPE packetID;
			PACKET_DATA_TYPE completion;
			PACKET_DATA_TYPE data[PACKET_DATA_SIZE];
		};

		struct {
			PACKET_DATA_TYPE rawdata[PACKET_SIZE];
		};
	};
	//not a part of the payload
	int size;


};

enum ClientType {
	INFERRED,
	PRESET
};

struct Computer {
	struct addrinfo address;
	struct sockaddr_storage inferredaddress;
	
	int fd;
	enum ClientType type;
	char ip[INET6_ADDRSTRLEN];
};




// struct Socket {
// 	int fd;
// };

/*
 MARK: - BUFFERS
 */
struct Buffer {
	char data [1500];
};

struct ContentBuffer {
	char data [MAX_CONTENT_SIZE];
	int latestPosition;
};


/*
 MARK: - SYNCHRONISATION FUNCTIONS
 */
struct Frame {
	struct Packet packets [MAX_PACKET_NO];
	
	///The number of packets the frame has
	int packetcount;
	
	///The number of packets the frame should have
	int packetno;
	
	///Indicates wether or not 
	char ready;
	
	PACKET_DATA_TYPE frameID;
};

struct Pool {
	struct Frame * frames;
};


/*
 MARK: -MUTATORS
 */
int insertPacketIntoPool(struct Pool *, struct Packet *);
void joinFrame(struct ContentBuffer *, struct Frame *);


/*
 MARK: -INITALISERS
 */
struct Computer * createComputer(const char *, const char *);
struct Computer * thisComputer(const char * port);
struct Computer * anyComputer(void);

struct Socket * createSocket(void);

struct Pool createPool(void);

void terminate(void *);

// void bindSocketToComputer(struct Socket *, struct Computer *);
void createFrameInPlace(struct Frame *, struct Packet *);

