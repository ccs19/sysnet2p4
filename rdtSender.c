/*
 * rdtSender.c
 * Systems and Networks II
 * Project 4
 * Christopher Schneider & Brett Rowberry
 * This file implements the functions used by the RDT sender.
 */

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "common.h"
#include "rdtSender.h"

//Private function prototypes
void getMessage(char * message);
void validatePort(int numberOfArgs, int proxyPort, const char *programName);

int main(int argc, const char* argv[])
{
    //from UDPmain.c
    int                sockfd;
    struct sockaddr_in servaddr;
    char               response[MAX_MESSAGE_SIZE];
    char               message[256];

    if (argc != 3) {
        fprintf (stderr, "Usage: client <hostname> <portnum>\n");
        exit (1);
    }

    // parse input parameter for port information
    int portNum = atoi (argv[2]);

    // create a streaming socket
    sockfd = createSocket(argv[1], portNum, &servaddr);
    if (sockfd < 0) {
        exit (1);
    }

    printf ("Enter a message: ");
    fflush(stdout);
    fgets (message, 256, stdin);
    // replace new line with null character
    message[strlen(message)-1] = '\0';

    // send request to server
    //if (sendRequest (sockfd, "<echo>Hello, World!</echo>", &servaddr) < 0) {
    if (sendRequest (sockfd, message, &servaddr) < 0) {
        closeSocket (sockfd);
        exit (1);
    }

    if (receiveResponse(sockfd, response, 256) < 0) {
        closeSocket (sockfd);
        exit (1);
    }
    closeSocket (sockfd);

    // display response from server
    printResponse(response);

    /////

    const char * proxyHostname = argv[1];
    int proxyPort = atoi(argv[2]);
    validatePort(argc, proxyPort, argv[0]);

    //state machine goes here
    getMessage(&message[MAX_MESSAGE_SIZE]);
    sendMessage(proxyHostname, proxyPort, message);

    printResponse(response);
    return 0;
}

void getMessage(char *message)
{
    printf ("Enter a message: ");
    fflush(stdout);
    fgets (message, SEGMENT_SIZE - 1, stdin);
    // replace new line with null character
    message[strlen(message)-1] = '\0';
}

/*
 * Sends a message to an RDT receiver on a specified host and port.
 *
 * destHost  - the name of the host on which the receiver runs
 * destPort  - the number of the port on which the receiver listens
 * message   - the entire text message to be sent to the receiver; the message is \0 terminated
 *
 * return 0, if no error; otherwise, a negative number indicating the error
 */
int sendMessage (char* desthost, int destPort, char* message)
{
    //1. sndpkt = makePacket(0, data, valid)
    //2. udtSend(sndpkt)
    //3. startTimer

    struct sockaddr_in * dest = NULL;
    int sockfd = createSocket(desthost, destPort, dest);
    if (sockfd < 0) exit (1);
    char * response = NULL;

    if (sendRequest (sockfd, message, dest) < 0)
    {
        closeSocket (sockfd);
        exit (1);
    }

    if (receiveResponse(sockfd, response, SEGMENT_SIZE) < 0)
    {
        closeSocket (sockfd);
        exit (1);
    }

    printResponse(response);
    closeSocket (sockfd);

    return 0;
}


//wait for :
// data to send - keyboard input
// timer to go off - handle timer event and arrival of ack with select() system call
// acks from network

/*
 * Checks that a command line argument is a valid integer port number. If valid, it is printed and returned.
 * Otherwise, the program exits.
 *
 * numberOfArgs    - number of command line args
 * inputString     - hopefully, a valid integer port number
 */
void validatePort(int numberOfArgs, int proxyPort, const char *programName)
{
    if (numberOfArgs != 3)
    {
        printf("Usage: %s <proxyHostname> <proxyPort>\n", programName);
        exit(1);
    }

    if( !isValidPort(proxyPort) ) exit(1);
}

/*
 * Checks that a command line argument is a valid integer port number. If valid, it is printed and returned.
 * Otherwise, the program exits.
 *
 * numberOfArgs    - number of command line args
 * inputString     - hopefully, a valid integer port number
 */
int getAndPrintHostName(int numberOfArgs, const char * proxyHostnameString, const char *proxyPortString)
{
    if (numberOfArgs != 3)
    {
        printf("Usage: ./sender <proxyHostname> <proxyPort>\n");
        exit(1);
    }

    int portPort = atoi(proxyPortString);

    if(portPort < 0 || portPort > 65535)
    {
        printf("Invalid port number.\n");
        exit(1);
    }

    printf("Proxy Port = %d\n", portPort);
    return portPort;
}

////////////////

/*
 * Creates a datagram socket and connects to a server.
 *
 * serverName - the ip address or hostname of the server given as a string
 * serverPort - the port number of the server
 * dest       - the server's address information; the structure will be initialized with information
 *              on the server (like port, address, and family) in this function call
 *
 * return value - the socket identifier or a negative number indicating the error if a connection could not be established
 */
int createSocket(char * serverName, int port, struct sockaddr_in * dest)
{
    /*~~~~~~~~~~~~~~~~~~~~~Local vars~~~~~~~~~~~~~~~~~~~~~*/
    int socketFD;
    struct hostent *hostptr;
    struct in_addr ipAddress;
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


    if( (socketFD = socket(AF_INET, SOCK_DGRAM, 0) ) < 0)
    {
        printf("Socket creation failed\n");
        return -1;
    }
    if( (hostptr = gethostbyname(serverName) ) == NULL)
    {
        perror("gethostbyname() failed, exit\n");
        return -1;
    }

    int i=0;
    printf("\nIP: " );
    while(hostptr->h_addr_list[i] != 0)
    {
        ipAddress.s_addr = *(u_long*)hostptr->h_addr_list[i++];
        printf("%s\n", inet_ntoa(ipAddress));
    }

    memset((void*)dest, 0, sizeof(struct sockaddr_in));    /* zero the struct */
    dest->sin_family = AF_INET;
    memcpy( (void *)&dest->sin_addr, (void *)hostptr->h_addr, hostptr->h_length);
    dest->sin_port = htons( (u_short)port );        /* set destination port number */
    printf("port: %d\n", htons(dest->sin_port));
    return socketFD;
}
/*
 * Sends a request for service to the server. This is an asynchronous call to the server,
 * so do not wait for a reply in this function.
 *
 * sockFD  - the socket identifier
 * request - the request to be sent encoded as a string
 * dest    - the server's address information
 *
 * return   - 0, if no error; otherwise, a negative number indicating the error
 */
int sendRequest(int socketFD, char * request, struct sockaddr_in * dest)
{
    socklen_t destSize = sizeof(struct sockaddr_in);
    int size = sendto(socketFD, request, strlen(request), 0, (struct sockaddr *) dest, destSize);
    return size;
}
/*
 * Receives the server's response formatted as an XML text string.
 *
 * sockfd    - the socket identifier
 * response  - the server's response as an XML formatted string to be filled in by this function into the specified string array
 *
 * return   - 0, if no error; otherwise, a negative number indicating the error
 */
int receiveResponse(int socketFD, char * response, int size)
{
    int length = 0;
    socklen_t bufSize = (socklen_t)size;
    length = recvfrom(socketFD, response, bufSize, 0, NULL, NULL);
    response[length] = '\0';
    return length;
}

/*
 * Prints the response to the screen in a formatted way.
 *
 * response - the server's response as an XML formatted string
 *
 */
void printResponse(char * response)
{
    printf("%s\n",response);
}

/*
 * Closes the specified socket
 *
 * sockFD - the ID of the socket to be closed
 *
 * return - 0, if no error; otherwise, a negative number indicating the error
 */
int closeSocket(int socketFD)
{
    close(socketFD);
    return 0;
}













