//
//  observe.h
//  NetworkingTest
//
//  Created by Ruben Ticehurst-James on 28/04/2022.
//


#include <stdio.h>
#include "models.h"
#include "clientmanager.h"

void observe(
			 struct Computer * computer,
			 struct ClientManager *,
			 void (*completion)(const void *, const char *, int)
);

// //This one provides context which is useful for callbacks which do not allow you to access outside scope
void observeWithContext(
						struct Computer * computer,
						struct ClientManager *,
						const void *,
						void (*completion)(const void *, const char *, int)
);

enum MessageTypes recieveOnce(
							  struct Computer *,
							  struct Computer *,
							  struct Packet *,
							  struct Buffer *
);


enum MessageTypes {
	OPEN, 
	PING,
	DATA
};
