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

const char* ServerShutdownMessage = "Server shutting down...";

//Function prototypes
int validateAndPrintPort(int numberOfArgs, const char * nameOfProgram, const char * portString);
void ExitOnError(char * errorMessage);
void BindSocket();
void OpenSocket(int port);
void InitAddressStruct(int port);
void DisplayServerInfo();
void AcceptConnections();
void DisplayInfo();
void HandleClientRequests(struct sockaddr_in* clientAddress);
int XMLParser(  const char* beginXml,
        const char* endXml,
        char* clientMessage,
        char* token,
        int tokenSize);
void ParseClientMessage(char* clientMessage,  struct sockaddr_in* clientAddress, int clientSocket);


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
    char stringBuffer[SEGMENT_SIZE];
    bzero(stringBuffer, SEGMENT_SIZE);
    socklen_t clientAddressLength = sizeof(struct sockaddr_in);
    socklen_t bufSize = SEGMENT_SIZE;
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
    char string[SEGMENT_SIZE]; //String to send back to client
    char token[SEGMENT_SIZE];  //Token to use for echo reply
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
            char tempAvg[SEGMENT_SIZE];
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

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION: XMLParser
    Parses an XML value and returns a token.
    @param begin           --      The expected beginning of an XML expression
    @param end             --      The expected ending of an XML expression
    @param token           --      The token extracted from the expression
    @param clientMessage   --      Message to parse
    @param length          --      Size of token
    @return                --      1 on success, 0 on failure, -1 if token is too large to fit
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
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