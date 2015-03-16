/*
 * rdtReceiver.c
 * Systems and Networks II
 * Project 4
 * Christopher Schneider & Brett Rowberry
 * This file implements the functions used by the RDT receiver.
 */

#include <stdio.h>
#include <stdlib.h>

#include <netdb.h> //For gethostbyname()
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <AddressBook/AddressBook.h>
#include <string.h> //for memset
#include <sys/types.h>

#include "rdtReceiver.h"
#include "common.h"

//Function prototypes
int getAndPrintPort(int numberOfArgs, const char *inputString);
void OpenSocket(int port, int * socketFD, struct sockaddr_in * address, struct hostent * hostByName);
void InitAddressStruct(int port, struct sockaddr_in * address, struct hostent * hostByName);
void BindSocket(int socket, struct sockaddr_in * address);
void ExitOnError(char * errorMessage);

int main(int argc, const char* argv[])
{
    int port = getAndPrintPort(argc, argv[1]);
    int socketFD = 0;
    struct hostent * hostByName = NULL;
    struct sockaddr_in * address = (struct sockaddr_in *)malloc(sizeof(address));

    OpenSocket(port, &socketFD, address, hostByName);

//    char message[SEGMENT_SIZE];
//    sprintf(message, "%s", receiveMessage(socketFD));
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
    if( !isValidPort(portNumber) ) exit(1);
    printf("Port = %d\n", portNumber);
    return portNumber;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   OpenSocket
    Opens the listening socket and resolves the host name.
    @param  port           -- The port number to bind the listen socket
    @return                -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void OpenSocket(int port, int * socketFD, struct sockaddr_in * address, struct hostent * hostByName)
{
    char hostname[MAX_HOSTNAME_LENGTH];

    if( ( *socketFD =  socket(PF_INET, SOCK_DGRAM, 0) ) <  0)  //If socket fails
        ExitOnError("Error creating socket");

    if(gethostname(hostname, sizeof(hostname)) < 0)               //If getting hostname fails
        ExitOnError("Error acquiring hostname. Exiting");

    if( ( hostByName = gethostbyname(hostname) ) ==  NULL)        //If gethostbyname fails print error message, exit
    {
        herror("GetHostByName failed. Error: ");
        exit(1);
    }
    InitAddressStruct(port, address, hostByName);
    BindSocket(*socketFD, address);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   InitAddressStruct
    Initializes the ServerAddress structure with the IP, port, and protocol type.
    @param  port           --  Port number to open
    @return                --  void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void InitAddressStruct(int port, struct sockaddr_in * address, struct hostent * hostByName)
{
//    memset((void*)&address, '0', (size_t)sizeof(address));
    address->sin_family = AF_INET;
    memcpy( (void *)&address->sin_addr, (void *)hostByName->h_addr, hostByName->h_length);
    address->sin_port = htons(port);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   BindSocket
    Binds the server socket
    @return           --    void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void BindSocket(int socket, struct sockaddr_in * address)
{
    if( ( bind( socket, (struct sockaddr *) &address, (socklen_t)sizeof(address)) )  < 0)
        ExitOnError("Failed to bind socket"); //If binding of socket fails
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   ExitOnError
    Prints a message to stdout and exits
    @param  errorMessage           -- Error message to be printed
    @return                        -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ExitOnError(char * errorMessage)
{
    printf("%s\n", errorMessage);
    exit(1);
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