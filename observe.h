//
//  observe.h
//  NetworkingTest
//
//  Created by Ruben Ticehurst-James on 28/04/2022.
//


#include <stdio.h>
#include "models.h"
#include "clientmanager.h"


//First callback is for a new client, second callback is for a data recieve
void observe(struct Socket *, struct ClientManager *, void (*completion)(const void *, const char *, int));

// //This one provides context which is useful for callbacks which do not allow you to access outside scope
void observeWithContext(struct Socket *, struct ClientManager *, const void *, void (*completion)(const void *, const char *, int));

enum MessageTypes recieveOnce(struct Socket *, struct Computer *, struct Packet *, struct Buffer *);


enum MessageTypes {
	OPEN, 
	PING,
	DATA
};
