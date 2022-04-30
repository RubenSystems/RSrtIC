//
//  client.h
//  NetworkingTest
//
//  Created by Ruben Ticehurst-James on 11/04/2022.
//

#pragma once

#include "config.h"

#include <netinet/in.h>

/*
 MARK: - RAW DATA
 */
struct Packet {
	union {
		struct {
			PACKET_INDEX_TYPE index;
			PACKET_ID_TYPE packetID;
			char completion;
			char data[PACKET_SIZE];
		};

		struct {
			char rawdata[PACKET_SIZE];
		};
	};

	
	
	//not a part of the payload
	int size;


};

struct Computer {
	struct sockaddr_in address;
};

struct Socket {
	int fd;
};

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
	
	PACKET_ID_TYPE frameID;
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
struct Computer createComputer(const char *, int);
struct Socket createSocket(void);
struct Pool createPool(void);
void createFrameInPlace(struct Frame *, struct Packet *);

