//
//  clientmanager.c
//  NetworkingTest
//
//  Created by Ruben Ticehurst-James on 1/05/2022.
//

#include "../clientmanager.h"

#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

struct ClientManager * createClientManager(int numberOfClients, void (*newclient)(struct Computer *, int)) {
	struct ClientManager * manager = (struct ClientManager *)malloc(sizeof(struct ClientManager));

	manager->numberOfClients = numberOfClients;
	manager->clients = (struct Computer **)calloc(numberOfClients, sizeof(struct Computer *));
	// memset(manager->clients, 0, sizeof(struct Computer *) * numberOfClients);
	manager->newclient = newclient;

	return manager;

}

void removeClient(struct ClientManager * manager, int id) {
	free(manager->clients[id]);
	manager->clients[id] = 0;
}

void * get_in_addr(struct sockaddr* sa) {
    if (sa->sa_family == AF_INET)
        return &(((struct sockaddr_in*)sa)->sin_addr);
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


//Will not malloc if the client is there. 
void addClient(struct ClientManager * manager, struct Computer * computer) {

	inet_ntop(computer->inferredaddress.ss_family, get_in_addr((struct sockaddr *)&computer->inferredaddress), computer->ip, INET6_ADDRSTRLEN);
	computer->timeout = TIMEOUT_AFTER;
	
	int firstEmptyIndex = -1;
	for(int i = 0; i < manager->numberOfClients; i ++) {
		if (!manager->clients[i] && firstEmptyIndex == -1) {
			firstEmptyIndex = i;
		} else if (manager->clients[i] && strcmp(manager->clients[i]->ip, computer->ip) == 0) {
			memcpy(manager->clients[i], computer, sizeof(struct Computer));
			return;
		}
	}
	if (firstEmptyIndex != -1) {
		// memcpy(manager->clients[firstEmptyIndex], computer, sizeof(struct Computer));
		manager->clients[firstEmptyIndex] = (struct Computer *)malloc(sizeof(struct Computer));
		memcpy(manager->clients[firstEmptyIndex], computer, sizeof(struct Computer));

		manager->newclient(computer, firstEmptyIndex);
	} else {
		printf("[ERROR] - addClient: client pool is full.\n");
	}
}

struct Computer * getClient(struct ClientManager * manager, int id) {
	return manager->clients[id];
}
