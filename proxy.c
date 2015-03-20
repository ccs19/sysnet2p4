/*
 * network.c
 * Systems and Networks II
 * Project 4
 * Christopher Schneider & Brett Rowberry
 * This file implements the proxy network.
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h> //For gethostbyname()
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h> //for memset

#include "proxy.h"

//Globals
int ServerSocket = 0;
struct hostent *HostByName = NULL;
struct sockaddr_in ServerAddress;

//Constants
const int HostNameMaxSize = 256;
const int BUFFERSIZE = 256;
const int PORT_UNDEFINED = 99999;
const int MAX_PORT = 65535;

//Private function prototypes
void checkCommandLineArgs(int numberOfArgs);
int getReceivingPort(const char * portString);
int getPercent(const char * percentString);

int main(int argc, const char* argv[])
{
//    checkCommandLineArgs(argc);
//
//    char receivingHostname[MAX_HOSTNAME_LENGTH];
//    int receivingPort = getReceivingPort(argv[2]);
//    int lostPercent = getPercent(argv[3]);
//    int delayedPercent = getPercent(argv[4]);
//    int errorPercent = getPercent(argv[5]);


    ProxyInfo *pInfo = malloc(sizeof(ProxyInfo));
    pInfo->port = atoi(argv[1]);
    InitReceiverInfo(pInfo, argv[1], atoi(argv[2]));
    OpenSocket(pInfo); //Open socket with port in arg vector 1
    AcceptConnections(pInfo);

    return 0;
}

/*
 * Checks that a command line argument is a valid integer port number. If valid, it is printed and returned.
 * Otherwise, the program exits.
 *
 * numberOfArgs    - number of command line args
 * inputString     - hopefully, a valid integer port number
 */
void checkCommandLineArgs(int numberOfArgs)
{
    if (numberOfArgs != 6)
    {
        printf("Usage: ./proxy <rcvHostname> <rcvPort> <lostPercent> <delayedPercent> <errorPercent>\n");
        exit(1);
    }
}

int getReceivingPort(const char * portString)
{
    int port = atoi(portString);
    if( !isValidPort(port) ) exit(1);
    printf("Receiving Port = %d\n", port);
    return port;
}

int getPercent(const char * percentString)
{
    int percent = atoi(percentString);
    if( !isValidPercentage(percent) ) exit(1);
    printf("Percent = %d\n", percent);
    return percent;
}


////////////////////////////////

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   OpenSocket
    Opens the listening socket and resolves the host name.
    @param  port           -- The port number to bind the listen socket
    @return                -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void OpenSocket(ProxyInfo* pInfo)
{
    char hostname[HostNameMaxSize];

    if(gethostname(hostname, sizeof(hostname)) < 0)               //If getting hostname fails
        ExitOnError("Error acquiring hostname. Exiting");

    if( ( pInfo->proxyInfo = gethostbyname(hostname) ) ==  NULL)        //If gethostbyname fails print error message, exit
    {
        herror("GetHostByName failed. Error: ");
        exit(1);
    }
    InitAddressStruct(pInfo);
    BindSocket(pInfo);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   InitAddressStruct
    Initializes the ServerAddress structure with the IP, port, and protocol type.
    @param  port           --  Port number to open
    @return                --  void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void InitAddressStruct(ProxyInfo* pInfo)
{
    memset((void*)&pInfo->proxyAddress, '0', (size_t)sizeof(pInfo->proxyAddress));
    pInfo->proxyAddress.sin_family = AF_INET;
    memcpy( (void *)&pInfo->proxyAddress.sin_addr, (void *)pInfo->proxyInfo->h_addr, pInfo->proxyInfo->h_length);
    pInfo->proxyAddress.sin_port = htons(pInfo->port);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   DisplayInfo
    Displays the connection info of the server.
    @return           --    void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void DisplayInfo(ProxyInfo* pInfo)
{
    int i = 0;
    struct in_addr ipAddress;
    struct sockaddr_in sockIn;
    int sockLen = sizeof(sockLen);
    getsockname(pInfo->proxySocket, (struct sockaddr*)&sockIn, (socklen_t*)&sockLen );
    printf("Host Name: %s\n",pInfo->proxyInfo->h_name);
    printf("IP:        ");
    while(pInfo->proxyInfo->h_addr_list[i] != 0)
    {
        ipAddress.s_addr = *(u_long*)pInfo->proxyInfo->h_addr_list[i++];
        printf("%s\n", inet_ntoa(ipAddress));
    }
    printf("Port:      %d\n", htons(sockIn.sin_port));
    fflush(stdout);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   BindSocket
    Binds the server socket
    @return           --    void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void BindSocket(ProxyInfo* pInfo)
{
    if( ( bind( pInfo->proxySocket, (struct sockaddr *) &pInfo->proxyAddress, sizeof(pInfo->proxyAddress)) )  < 0)
        ExitOnError("Failed to bind socket"); //If binding of socket fails
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   AcceptConnections
    Accepts the client connection and creates a new detached thread for the client.
    @return           --    void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void AcceptConnections(ProxyInfo* pInfo)
{
    /*~~~~~~~~~~~~~~~~~~~~~Local vars~~~~~~~~~~~~~~~~~~~~~*/
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    DisplayInfo(pInfo);
    printf("Waiting for connection... ");
    fflush(stdout);
    for(;;)
    {
        HandleClientRequests(pInfo);
    }
}



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   HandleClientRequests
    Depending on the string received from the client, we either print that it failed or parse
    the message, then close the socket and free the thread.
    @param  ClientSocketPtr -- A pointer to the client socket typecasted to a void*
    @return           -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void HandleClientRequests(ProxyInfo* pInfo)
{
    fflush(stdout);
    /*~~~~~~~~~~~~~~~~~~~~~Local vars~~~~~~~~~~~~~~~~~~~~~*/
    char stringBuffer[BUFFERSIZE];
    bzero(stringBuffer, BUFFERSIZE);
    socklen_t clientAddressLength = sizeof(struct sockaddr_in);
    socklen_t bufSize = BUFFERSIZE;
    int length;
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    fflush(stdout);
    length = recvfrom(
            pInfo->proxySocket,                     //Server socket
            stringBuffer,                     //Buffer for message
            bufSize,             //Size of buffer
            0,                                //Flags
            (struct sockaddr*)&pInfo->senderAddress,  //Sender info
            &clientAddressLength              //Size of source address
    );
    ForwardToReceiver(pInfo, stringBuffer);
    ForwardToSender(pInfo);
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

void ForwardToReceiver(ProxyInfo* pInfo, char* message) {
    socklen_t destSize = sizeof(struct sockaddr_in);
    int size = sendto(pInfo->proxySocket,message, strlen(message), 0, (struct sockaddr *)&pInfo->receiverAddress, destSize);
    printf("Fowarding to receiver:");
}

void InitReceiverInfo(ProxyInfo *pInfo, const char *receiverName, int receiverPort){

    struct in_addr receiverIp;

    if( ( pInfo->proxySocket =  socket(AF_INET, SOCK_DGRAM, 0) ) <  0)  //If socket fails
        ExitOnError("Error creating socket");

    if( (pInfo->receiverInfo = gethostbyname(receiverName) ) == NULL)
    {
        perror("gethostbyname() failed to get receiver info, exit\n");
        exit(1);
    }

    int i=0;
    printf("\nIP: " );
    while(pInfo->receiverInfo->h_addr_list[i] != 0)
    {
        receiverIp.s_addr = *(u_long*)pInfo->receiverInfo->h_addr_list[i++];
        printf("%s\n", inet_ntoa(receiverIp));
    }

    memset((void*)&pInfo->receiverAddress, 0, sizeof(struct sockaddr_in));    /* zero the struct */
    pInfo->receiverAddress.sin_family = AF_INET;
    memcpy( (void *)&pInfo->receiverAddress.sin_addr, (void *)pInfo->receiverInfo->h_addr, pInfo->receiverInfo->h_length);
    pInfo->receiverAddress.sin_port = htons( (u_short)receiverPort );        /* set destination port number */
    printf("port: %d\n", htons(pInfo->receiverAddress.sin_port));

}

void ForwardToSender(ProxyInfo* pInfo){
    fflush(stdout);
    /*~~~~~~~~~~~~~~~~~~~~~Local vars~~~~~~~~~~~~~~~~~~~~~*/
    char stringBuffer[BUFFERSIZE];
    bzero(stringBuffer, BUFFERSIZE);
    socklen_t clientAddressLength = sizeof(struct sockaddr_in);
    socklen_t bufSize = BUFFERSIZE;
    int length;
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    fflush(stdout);
    length = recvfrom(
            pInfo->proxySocket,                     //Server socket
            stringBuffer,                     //Buffer for message
            bufSize,             //Size of buffer
            0,                                //Flags
            (struct sockaddr*)&pInfo->senderAddress,  //Sender info
            &clientAddressLength              //Size of source address
    );
}
