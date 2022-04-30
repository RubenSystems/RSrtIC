

#include "../transmit.h"

#include <stdlib.h>
#include <string.h>

int PACKET_ID_COUNTER = 0;

void transmitPacket(struct Socket * socket, struct Computer * computer, struct Packet packet) {
	long numbytes;
	if ((numbytes = (int)sendto(socket->fd, packet.rawdata, packet.size, 0, (struct sockaddr *)&computer->address, sizeof(struct sockaddr))) < 0) {
		perror("sendto");
		exit(1);
	}
}

void transmit(struct Socket * socket, struct Computer * computer, const char * data, int size) {
	/*
		Add size checks here
	*/
	const int packetSize = 1475;

	PACKET_ID_COUNTER = (PACKET_ID_COUNTER + 1) % 65535;

	//Copy to prevent conflicts between two different clients
	int currentPacketID = PACKET_ID_COUNTER;


	for (int i = 0; i < size / packetSize; i ++) {
		struct Packet currentPacket;
		int packetSize = packetSize * (i + 1) < size ? packetSize : size - (packetSize * i);
		currentPacket.index = i;
		currentPacket.packetID = currentPacketID;
		currentPacket.size = packetSize;
		currentPacket.completion = 0;

		memmove(currentPacket.data, &(data[i * packetSize]),  packetSize);

		transmitPacket(socket, computer, currentPacket);


	}
}
//3x + x = x(3+1)