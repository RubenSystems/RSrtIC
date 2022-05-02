

#include "../transmit.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

char PACKET_ID_COUNTER = 0;
char buffer [1500] = {0};

void transmitPacket(struct Socket * socket, struct Computer * computer, struct Packet * packet) {
	int numbytes;


	memmove(&(buffer[0]), &(packet->index), 1);//moving index
	memmove(&(buffer[1]), &(packet->packetID), 1);//moving id
	memmove(&(buffer[2]), &(packet->completion), 1);//moving id
	memmove(&(buffer[3]), &(packet->data), packet->size );//moving id

	if ((numbytes = (int)sendto(socket->fd, buffer, packet->size + 3, 0, (struct sockaddr *)&computer->address, sizeof(struct sockaddr))) < 0) {
		perror("sendto");
		exit(1);
	}
}

void transmit(struct Socket * socket, struct Computer * computer, char * data, int size) {
	/*
		Add size checks here 
	*/

	//Warning, you add three bytes on for the header!! this is just for the DATA payload
	const int maxPacketSize = 1475;
	
	PACKET_ID_COUNTER = (PACKET_ID_COUNTER + 1) % 255;

	//Copy to prevent conflicts between two different clients
	int currentPacketID = PACKET_ID_COUNTER;

	for (int i = 0; i < ceil(size / maxPacketSize) + 1; i ++) {
		struct Packet currentPacket;
		int packetSize = maxPacketSize * (i + 1) < size ? maxPacketSize : size - ((maxPacketSize) * i);
		currentPacket.index = i;
		currentPacket.packetID = currentPacketID;
		currentPacket.size = packetSize;
		currentPacket.completion = (i == ceil(size / maxPacketSize)) ? 1 : 0;

		memmove(&(currentPacket.data), &(data[i * maxPacketSize]), packetSize );
		transmitPacket(socket, computer, &currentPacket);
	}
}
