/*
 * rdtReceiver.c
 * Systems and Networks II
 * Project 4
 * Christopher Schneider & Brett Rowberry
 * This file implements the functions used by the RDT receiver.
 */


#include <arpa/inet.h>
#include <pthread.h>


#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>

#include "rdtReceiver.h"
#include "common.h"

//Globals
int ServerSocket = 0;
struct hostent * HostByName = NULL;
struct sockaddr_in ServerAddress;

//Function prototypes
int validateAndPrintPort(int numberOfArgs, const char * nameOfProgram, const char * portString);
void ExitOnError(char * errorMessage);
void BindSocket();
void OpenSocket(int port);
void InitAddressStruct(int port);
void DisplayServerInfo();

int main(int argc, const char* argv[])
{
    unsigned int port = validateAndPrintPort(argc, argv[0], argv[1]);
//    if(port == -1) exit(1);

    OpenSocket(port);

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
 * Checks that string is a valid integer port number. If valid, it is printed and returned.
 * Otherwise, returns -1.
 *
 * numberOfArgs    - number of command line args
 * inputString     - hopefully, a valid integer port number
 */
int validateAndPrintPort(int numberOfArgs, const char *nameOfProgram, const char * portString)
{
    if (numberOfArgs != 2)
    {
        printf("Usage: %s <port number>\n", nameOfProgram);
        return -1;
    }

    int portNumber = atoi(portString);
    if( !isValidPort(portNumber) ) return -1;
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
void OpenSocket(int port)
{
    char hostname[MAX_HOSTNAME_LENGTH];

    if( ( ServerSocket =  socket(PF_INET, SOCK_DGRAM, 0) ) <  0)  //If socket fails
        ExitOnError("Error creating socket");

    if(gethostname(hostname, sizeof(hostname)) < 0)               //If getting hostname fails
        ExitOnError("Error acquiring hostname. Exiting");

    if( ( HostByName = gethostbyname(hostname) ) ==  NULL)        //If gethostbyname fails print error message, exit
    {
        herror("GetHostByName failed. Error: ");
        exit(1);
    }
    InitAddressStruct(port);
    BindSocket();
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   InitAddressStruct
    Initializes the ServerAddress structure with the IP, port, and protocol type.
    @param  port           --  Port number to open
    @return                --  void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void InitAddressStruct(int port)
{
    memset((void*)&ServerAddress, '0', (size_t)sizeof(ServerAddress));
    ServerAddress.sin_family = AF_INET;
    memcpy( (void *)&ServerAddress.sin_addr, (void *)HostByName->h_addr, HostByName->h_length);
    ServerAddress.sin_port = htons(port);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   BindSocket
    Binds the server socket
    @return           --    void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void BindSocket()
{
    if( ( bind( ServerSocket, (struct sockaddr *) &ServerAddress, sizeof(ServerAddress)) )  < 0)
        ExitOnError("Failed to bind socket"); //If binding of socket fails
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   ExitOnError
    Prints a message to stdout and exits
    @param  errorMessage           -- Error message to be printed
    @return                        -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ExitOnError(char* errorMessage)
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