//
//  clientmanager.h
//  NetworkingTest
//
//  Created by Ruben Ticehurst-James on 30/04/2022.
//


#include <stdio.h>
#include "models.h"


struct ClientManager {
	struct Computer ** clients;
	int numberOfClients;

	void (*newclient)(struct Computer *, int);
};

//callback with the computer and the id you can easily reference the client to. 
struct ClientManager * createClientManager(int, void (*newclient)(struct Computer *, int));

void removeClient(struct ClientManager *, int);

void addClient(struct ClientManager *, struct Computer *);

struct Computer * getClient(struct ClientManager * manager, int); 


//for converting sockaddr to ip. 
void *get_in_addr(struct sockaddr *);
