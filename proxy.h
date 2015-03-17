#ifndef _PROXY_H
#define _PROXY_H


typedef enum{
RECEIVER,
SENDER
}Destination;


//Receives a packet
byte* receivePacket();

//Packet to send, destination
sendPacket(byte* packet, Destination dest);


//Implement as static inline in implementation
int lostPacket(int percentChance);

int delayPacket(int percentChance);

int errorPacket(int percentChance);


#endif