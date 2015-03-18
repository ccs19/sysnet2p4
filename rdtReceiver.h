/*
 * rdtReceiver.h
 * Systems and Networks II
 * Project 4
 *
 * This file describes the functions to be implemented by an RDT receiver.
 */

#ifndef _RDT_RECEIVER_H
#define _RDT_RECEIVER_H

/*
 * Receives a message from an RDT sender on a specified port.
 *
 * port - the number of the port on the host that receives messages from the sender
 *
 * returns the text message received by a sender or NULL if an error occurred
 */
char* receiveMessage (int sockFD);


//Other function prototypes

//Returns success or failure
//Checks if corrupt, returns success or failure
int udtReceive(int expectedSequence);

//Throws away first byte, returns data
byte* extractData(byte* packet);

//Adds data to a buffer. Need more params
void reassemble(byte* packet);

//Makes and returns the ack packet
byte* ackPacket(int ackNum, char* data, int byteCount);

//Sends the ack back
int udtSend(byte* ackPacket);

#endif //_RDT_RECEIVER_H