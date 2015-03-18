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
const int MAXLISTENERS = 5;
const int BUFFERSIZE = 256;
const int PORT_UNDEFINED = 99999;
const int MAX_PORT = 65535;
const char* ServerShutdownMessage = "Server shutting down...";

//Private function prototypes
void checkCommandLineArgs(int numberOfArgs);
int getReceivingPort(const char * portString);
int getPercent(const char * percentString);

int XMLParser(  const char* beginXml,
        const char* endXml,
        char* clientMessage,
        char* token,
        int tokenSize);

int main(int argc, const char* argv[])
{
//    checkCommandLineArgs(argc);
//
//    char receivingHostname[MAX_HOSTNAME_LENGTH];
//    int receivingPort = getReceivingPort(argv[2]);
//    int lostPercent = getPercent(argv[3]);
//    int delayedPercent = getPercent(argv[4]);
//    int errorPercent = getPercent(argv[5]);

    unsigned int port = PORT_UNDEFINED;

    if(argc > 2)
        printf("Usage:\n%s <optional:port to open>\n",argv[0]);
    else if(argc == 1) //If no port specified
        port = 0;
    else if(atoi(argv[1]) <= 0 || atoi(argv[1]) > MAX_PORT) //Checking valid port bounds
        printf("Invalid port number\n");
    else
    {
        if (port == PORT_UNDEFINED) port = atoi(argv[1]); //If port specified at command line
    }

    if(port != PORT_UNDEFINED)
    {
        OpenSocket(port); //Open socket with port in arg vector 1
        AcceptConnections();
    }


    //The Proxy also simulates slow packet transport within the network by delaying
    // transport of a packet to its destination using a fixed time delay. This time
    // delay should be between 1.5 and 2 times the waiting time for the sender to resend
    // packets. To accurately simulate slow data transport, the process delaying the transport
    // should be able to continue execution. This means that simply calling sleep() to delay
    // sending a packet will block the process preventing it to accept any new packets or sending
    // new packets. Instead, you need to create a new thread within the Proxy that will be tasked
    // with sending the packet with some delay using sleep(). You must simulate the probability
    // of a lost or delayed packet as some percentage such as 60%. The two separate parameters
    // for specifying the delayed and lost packet probabilities must be accepted as an input
    // parameter in main() to the Proxy.

    //You will need to simulate damaged data using a tag in the packet to indicate that your packets
    // are either free of error or corrupt – no checksum computation is needed. Randomly select the
    // tag to be either corrupt or not corrupt in each packet including acknowledgments. Corrupt
    // packets must also occur at a set percentage rate as above. Similarly, the percentage of
    // corrupted packets must be accepted as an input parameter in main() to the Proxy, which
    // handles the simulation of damaged data.


    //simulate lost packets using the Proxy that delivers data to the Sender and Receiver.
    // Random drop of packets is easily accomplished by using random numbers to decide when
    // to drop a packet. The Proxy’s send function that is sending a packet to the receiver must
    // use a random number to determine whether to call sendto() or not, to simulate a lost packet
    // on the network.

    //host name and port number of the receiver should be passed in as parameters to main()
    //bind socket to a user-specified port and print the port number on the screen

    //must use sleep() to test whether timer sender is working
    //read select() man page and on eLearning

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
void OpenSocket(int port)
{
    char hostname[HostNameMaxSize];

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
/*  FUNCTION:   DisplayInfo
    Displays the connection info of the server.
    @return           --    void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void DisplayInfo()
{
    int i = 0;
    struct in_addr ipAddress;
    struct sockaddr_in sockIn;
    int sockLen = sizeof(sockLen);
    getsockname(ServerSocket, (struct sockaddr*)&sockIn, (socklen_t*)&sockLen );
    printf("Host Name: %s\n",HostByName->h_name);
    printf("IP:        ");
    while(HostByName->h_addr_list[i] != 0)
    {
        ipAddress.s_addr = *(u_long*)HostByName->h_addr_list[i++];
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
void BindSocket()
{
    if( ( bind( ServerSocket, (struct sockaddr *) &ServerAddress, sizeof(ServerAddress)) )  < 0)
        ExitOnError("Failed to bind socket"); //If binding of socket fails
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   AcceptConnections
    Accepts the client connection and creates a new detached thread for the client.
    @return           --    void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void AcceptConnections()
{
    /*~~~~~~~~~~~~~~~~~~~~~Local vars~~~~~~~~~~~~~~~~~~~~~*/
    struct sockaddr_in clientAddress;
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    DisplayInfo();
    printf("Waiting for connection... ");
    fflush(stdout);
    for(;;)
    {
        HandleClientRequests(&clientAddress);
    }
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

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   HandleClientRequests
    Depending on the string received from the client, we either print that it failed or parse
    the message, then close the socket and free the thread.
    @param  ClientSocketPtr -- A pointer to the client socket typecasted to a void*
    @return           -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void HandleClientRequests(struct sockaddr_in* clientAddress)
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
            ServerSocket,                     //Server socket
            stringBuffer,                     //Buffer for message
            bufSize,             //Size of buffer
            0,                                //Flags
            (struct sockaddr*)clientAddress,  //Source address
            &clientAddressLength              //Size of source address
    );
    stringBuffer[length] = '\0';
    puts("");
    printf("Client IP: %s\n", inet_ntoa(clientAddress->sin_addr));
//    printf("Client port: %hu\n", ntohs(clientAddress->sin_port));
    printf("Received message: %s\n", stringBuffer);
    ParseClientMessage(stringBuffer, clientAddress, length);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   ParseClientMessage
    Handles the message for the client and sends a message back to the client
    @param  clientMessage        -- Pointer to message received by client
    @param  ClientSocket         -- The socket to the client
    @return                      -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ParseClientMessage(char* clientMessage,  struct sockaddr_in* clientAddress, int clientSocket)
{
    /*~~~~~~~~~~~~~~~~~~~~~Local vars~~~~~~~~~~~~~~~~~~~~~*/
    int i = 0;
    char string[BUFFERSIZE]; //String to send back to client
    char token[BUFFERSIZE];  //Token to use for echo reply
    string[0] = '\0';
    const int NUMLOADAVG = 3; //Number of load averages queries
    socklen_t clientAddressLength = sizeof(struct sockaddr_in);
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    /*~~~~~~~~~~~~~~~~~~~~~Load avg response~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    if(strcmp(clientMessage, "<loadavg/>") == 0)
    {
        double loadavg[NUMLOADAVG];
        getloadavg(loadavg, NUMLOADAVG); //Get load average and put in double array

        //Begin string format:
        strcat(string, "<replyLoadAvg>");
        for(i = 0; i < NUMLOADAVG; i++)
        {
            char tempAvg[BUFFERSIZE];
            sprintf(tempAvg, "%lf:", loadavg[i]);
            strcat(string, tempAvg);              //Append string to string we return to client
        }
        strcat(string, "</replyLoadAvg>\0"); //End of string
    }
        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

        /*~~~~~~~~~~~~~~~~~~~~~Echo response~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    else if ((XMLParser("<echo>", "</echo>", clientMessage, token, sizeof(token))) == 1)
    {
        //Set return echo string
        strcat(string, "<reply>");
        strcat(string, token);
        strcat(string, "</reply>\0");
    }
        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

        /*~~~~~~~~~~~~~~~~~~~~~Shut down response~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    else if(strcmp(clientMessage, "<shutdown/>") == 0)
    {
        strcat(string, ServerShutdownMessage);
    }
        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    else //Else we have an invalid format
        strcat(string, "<error>unknown format</error>\0");
    printf("Sending back %s\n\n", string);
    if(     (sendto(
            ServerSocket,                       //Client socket
            string,                             //String buffer to send to client
            strlen(string),                     //Length of buffer
            0,                                  //flags
            (struct sockaddr*)clientAddress,    //Destination
            clientAddressLength                 //Length of clientAddress

    )) < 0) //If sendto fails
        printf("Failed to send\n");
    if(strcmp(string, ServerShutdownMessage) == 0) //If server shutdown message received
    {
        close(ServerSocket);
        exit(1);
    }
}

int XMLParser(  const char* beginXml,
        const char* endXml,
        char* clientMessage,
        char* token,
        int tokenSize)
{
    //~~~~~~~~~~~~~Local vars ~~~~~~~~~~~~~~~~~~~~~~~//
    char tempString[strlen(clientMessage)];
    char *delimiter = NULL;
    int returnVal = 0;
    int i = 0;
    int foundDelimiter = 0;
    int beginXmlLength = strlen(beginXml);
    int endXmlLength = strlen(endXml);
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

    token[0] = '\0';
    memcpy(tempString, clientMessage, beginXmlLength); //Copy first part of clientMessage into temp for comparison.
    tempString[beginXmlLength] = '\0';
    if(strcmp(tempString, beginXml) == 0 ) //If beginXml is found
    {
        memcpy(tempString, clientMessage, strlen(clientMessage)); //Copy entire clientMessage
        for(i = 1; i < strlen(clientMessage); i++) //Check for valid delimiter here
        {
            if(tempString[i] == '<')
            {
                delimiter = tempString+i;//Potential valid delimiter found. Point delimiter ptr to location.
                foundDelimiter = 1;
                break;
            }
        }
        if(foundDelimiter)
        {
            delimiter[endXmlLength] = '\0';//Set end of delimiter to null
            if (strcmp(delimiter, endXml) != 0) //If invalid delimiter
                returnVal = 0;
            else {
                returnVal = 1;//Set valid return
                char *tempToken = clientMessage + (strlen(beginXml)); //Set temporary token to end of starting delimiter
                strtok(tempToken, "<");
                if (strlen(tempToken) > tokenSize) returnVal = -1;              //If token is too large, return -1
                else if (strcmp(tempToken, endXml) == 0) token[0] = '\0';       //Else if empty token found
                else strcat(token, tempToken);                                  //Else put extracted token in variable
            }
        }
    }
    return returnVal;
}