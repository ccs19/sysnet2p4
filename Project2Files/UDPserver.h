

#ifndef UDPSERVER_H
#define UDPSERVER_H

//#include <bits/pthreadtypes.h>
#include <sys/types.h>
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   OpenSocket
    Opens the listening socket and resolves the host name.
    @param  port           -- The port number to bind the listen socket
    @return                -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void OpenSocket(int port);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   InitAddressStruct
    Initializes the ServerAddress structure with the IP, port, and protocol type.
    @param  port           --  Port number to open
    @return                --  void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void InitAddressStruct(int port);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   DisplayInfo
    Displays the connection info of the server.
    @return           --    void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void DisplayInfo();

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   BindSocket
    Binds the server socket and listens on that socket
    @return           --    void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void BindSocket();

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   AcceptConnections
    Accepts the client connection and creates a new detached thread for the client.
    @return           --    void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void AcceptConnections();

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   ExitOnError
    Prints a message to stdout and exits
    @param  errorMessage           -- Error message to be printed
    @return                        -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ExitOnError(char*);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   HandleClientRequests
    Depending on the string received from the client, we either print that it failed or parse
    the message, then close the socket and free the thread.
    @param  ClientSocketPtr -- A pointer to the client socket typecasted to a void*
    @return           -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void HandleClientRequests(struct sockaddr_in*);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  FUNCTION:   ParseClientMessage
    Handles the message for the client and sends a message back to the client
    @param  clientMessage        -- Pointer to message received by client
    @param  ClientSocket         -- The socket to the client
    @return                      -- void
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void ParseClientMessage(char*, struct sockaddr_in*, int);

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
int XMLParser(const char*, const char*, char*, char*, int);

#endif