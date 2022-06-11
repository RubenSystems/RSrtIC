# RSrtIC

RubenSystems realtime internet communication. 

## What is RSrtIC?
RSrtIC is a library for realtime comunication between computers. An example application for this is:

- Video calling
- Real time stock quotes
- Sending Autocomplete results for search 

**RSrtIC does not have gaurenteed delivery.**


Since it is in c, it can be used in most environments. It has been used with Python (using cTypes) and Swift.

## Features 

- Client/Server architecture, but with the same codebase 
- realtime send and recieve 
- (serverside) client eviction 
- (serverside) client management
- connection management (for example, handling timeouts and reconnecting if the client/server is unable to connect)

## Protocol 

RSrtIC the RStp (RubenSystems transfer protocol) to help data splitting and reconstruction  

The data sent is called a "frame". A frame is split into packets in order to be sent. 
Each packet consists of 

byte: 
- 1: index (the index in the packet)
- 2: id (the id of the frame)
- 3: completion (if the packet is the last one in the frame)
- 4...end: data.  
