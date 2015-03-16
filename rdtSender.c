/*
 * rdtSender.c
 * Systems and Networks II
 * Project 4
 * Christopher Schneider & Brett Rowberry
 * This file implements the functions used by the RDT sender.
 */

#include "rdtSender.h"

int main()
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