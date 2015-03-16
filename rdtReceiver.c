/*
 * rdtReceiver.c
 * Systems and Networks II
 * Project 4
 * Christopher Schneider & Brett Rowberry
 * This file implements the functions used by the RDT receiver.
 */

#include <stdio.h>
#include <stdlib.h>
#include "rdtReceiver.h"
#include "commondefinitions.h"

//Function prototypes
int getAndPrintPort(int numberOfArgs, const char *inputString);


int main(int argc, const char* argv[])
{
    int port = getAndPrintPort(argc, argv[1]);

    int socketFD = 5;

    char message[SEGMENT_SIZE];
    sprintf(message, "%s", receiveMessage(socketFD));
    //bind socket to a user-specified port and print the port number on the screen
    //seq and ack numbers 1 or 0
    //buffering only required for last message
    //deliver message to stdout
    //segment size if 10B

    return 0;
}

/*
 * Checks that a command line argument is a valid integer port number. If valid, it is printed and returned.
 * Otherwise, the program exits.
 *
 * numberOfArgs    - number of command line args
 * inputString     - hopefully, a valid integer port number
 */
int getAndPrintPort(int numberOfArgs, const char *inputString)
{
    if (numberOfArgs != 2)
    {
        printf("Usage: ./receiver <port number>\n");
        exit(1);
    }

    int portNumber = atoi(inputString);

    if(portNumber < 0 || portNumber > 65535)
    {
        printf("Invalid port number.\n");
        exit(1);
    }

    printf("Port = %d\n", portNumber);
    return portNumber;
}

/*
 * Receives a message from an RDT sender on a specified port.
 *
 * port - the number of the port on the host that receives messages from the sender
 *
 * returns the text message received by a sender or NULL if an error occurred
 */
char * receiveMessage (int sockFD)
{
    return NULL;
}