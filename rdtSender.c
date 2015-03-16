/*
 * rdtSender.c
 * Systems and Networks II
 * Project 4
 * Christopher Schneider & Brett Rowberry
 * This file implements the functions used by the RDT sender.
 */

#include <stdio.h>
#include <stdlib.h>
#include "rdtSender.h"

int main(int argc, const char* argv[])
{
    //when the sender is executed, the host name and port number of the proxy
    // should be passed in as parameters to main()

    //wait for :
    // data to send - keyboard input
    // timer to go off - handle timer event and arrival of ack with select() system call
    // acks from network
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
int getAndPrintPort(int numberOfArgs, const char * proxyHostnameString, const char *proxyPortString)
{
    if (numberOfArgs != 3)
    {
        printf("Usage: ./sender <proxyHostname> <proxyPort>\n");
        exit(1);
    }

    int proxyPort = atoi(proxyPortString);

    if(proxyPort < 0 || proxyPort > 65535)
    {
        printf("Invalid port number.\n");
        exit(1);
    }

    printf("Proxy Port = %d\n", proxyPort);
    return proxyPort;
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
    return 0;
}