

#include "../transmit.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>



void transmitData(struct Computer * computer, const unsigned char * data, int numbytes) {
	int sentbytes;
	if ((sentbytes = (int)sendto(computer->fd, data, numbytes, 0, computer->sendaddress.ai_addr, computer->sendaddress.ai_addrlen)) < 0) {
		perror("sendto");
		exit(1);
	}
	
}

void transmitPacket(struct Computer * computer, struct Packet * packet) {
	transmitData(computer, packet->rawdata, packet->size + 3);
}

void transmit(struct Computer * computer, const char * data, int size) {
	static char PACKET_ID_COUNTER = 0;
	//Warning, you add three bytes on for the header!! this is just for the DATA payload
	const int maxPacketSize = 1000;
	
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
		transmitPacket(computer, &currentPacket);
	}	
}
