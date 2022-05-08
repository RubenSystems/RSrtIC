//
//  clientmanager.h
//  NetworkingTest
//
//  Created by Ruben Ticehurst-James on 30/04/2022.
//


#include <stdio.h>
#include "models.h"


struct ClientManager {
	struct Client ** clients;
	int numberOfClients;

	void (*newclient)(struct Client *, int);
};

//callback with the computer and the id you can easily reference the client to. 
struct ClientManager * createClientManager(int, void (*newclient)(struct Client *, int));

void removeClient(struct ClientManager *, int);

void addClient(struct ClientManager *, struct Client *);

struct Client * getClient(struct ClientManager * manager, int); 

