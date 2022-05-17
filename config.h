//
//  config.h
//  NetworkingTest
//
//  Created by Ruben Ticehurst-James on 28/04/2022.
//

#ifndef config_h
#define config_h

#define PACKET_DATA_TYPE unsigned char

#define PACKET_SIZE 1500
#define PACKET_HEADER_SIZE 3
#define PACKET_DATA_SIZE PACKET_SIZE - PACKET_HEADER_SIZE
#define MAX_PACKET_DATA_SIZE 1420

#define TIMEOUT_AFTER 200
#define PING_AFTER 500


#define MAX_PACKET_NO 200
#define MAX_CONTENT_SIZE PACKET_SIZE * MAX_PACKET_NO
#define FRAME_POOL_SIZE 5



#define OPEN_MESSAGE "open"
#define PING_MESSAGE "ping"

#endif /* config_h */
