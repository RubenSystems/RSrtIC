

#include "../transmit.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>



void transmitData(struct Computer * computer, struct Computer * client, const unsigned char * data, int numbytes) {
	int sentbytes;
	socklen_t addr_len = sizeof(struct sockaddr);
	switch (client->type) {
		case PRESET:
			sentbytes = (int)sendto(computer->fd, data, numbytes, 0, client->address.ai_addr, client->address.ai_addrlen);
			break;
		case INFERRED:
			sentbytes = (int)sendto(computer->fd, data, numbytes, 0, (struct sockaddr *)&client->inferredaddress, addr_len);
	}
	
	
	if (sentbytes < 0){
		perror("sendto");
		exit(1);
	}
	
}


void transmitPacket(struct Computer * computer, struct Computer * client, struct Packet * packet) {
	transmitData(computer, client, packet->rawdata, packet->size + 3);
}


enum TransmitResponses transmit(struct Computer * computer, struct Computer * client, const char * data, int size) {
	static char PACKET_ID_COUNTER = 0;
	//Warning, you add three bytes on for the header!! this is just for the DATA payload

	if (--client->timeout <= 0) {
		return EVICT;
	}
	
	PACKET_ID_COUNTER = (PACKET_ID_COUNTER + 1) % 255;
	if (PACKET_ID_COUNTER == 0) {
		PACKET_ID_COUNTER ++;
	}

	//Copy to prevent conflicts between two different clients
	int currentPacketID = PACKET_ID_COUNTER;
	
	for (int i = 0; i < ceil(size / MAX_PACKET_DATA_SIZE) + 1; i ++) {
		struct Packet currentPacket;
		int packetSize = MAX_PACKET_DATA_SIZE * (i + 1) < size ? MAX_PACKET_DATA_SIZE : size - ((MAX_PACKET_DATA_SIZE) * i);
		currentPacket.index = i;
		currentPacket.packetID = currentPacketID;
		currentPacket.size = packetSize;

		currentPacket.completion = (i == ceil(size / MAX_PACKET_DATA_SIZE)) ? 1 : 0;

		memmove(&(currentPacket.data), &(data[i * MAX_PACKET_DATA_SIZE]), packetSize );
		transmitPacket(computer, client, &currentPacket);
	}	
	return SUCCESS;
}
