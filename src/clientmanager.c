//
//  clientmanager.c
//  NetworkingTest
//
//  Created by Ruben Ticehurst-James on 1/05/2022.
//

#include "../clientmanager.h"

#include <stdlib.h>
#include <string.h>

struct ClientManager * createClientManager(int numberOfClients, void (*newclient)(struct Client *, int)) {
	struct ClientManager * manager = (struct ClientManager *)malloc(sizeof(struct ClientManager));

	manager->numberOfClients = numberOfClients;
	manager->clients = (struct Client **)calloc(numberOfClients, sizeof(struct Computer *));
	// memset(manager->clients, 0, sizeof(struct Computer *) * numberOfClients);
	manager->newclient = newclient;

	return manager;

}

void removeClient(struct ClientManager * manager, int id) {
	free(manager->clients[id]);
	manager->clients[id] = 0;
}

void addClient(struct ClientManager * manager, struct Client * computer) {
	for(int i = 0; i < manager->numberOfClients; i ++) {
		if (!manager->clients[i]) {
			manager->clients[i] = computer;
			manager->newclient(computer, i);
			return;
		}
	}
	printf("[ERROR] - addClient: client pool is full.\n");
}

struct Client * getClient(struct ClientManager * manager, int id) {
	return manager->clients[id];
}
