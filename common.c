/*
 * common.c
 * Systems and Networks II
 * Project 4
 * Christopher Schneider & Brett Rowberry
 * This file contains definitions common the the rdtSender, rdtReceiver, and network.
 */

#include <stdio.h>

/*
 * Checks if given port is a legal port number. Returns 1 if valid, 0 if invalid.
 *
 * port - input port
 *
 * return int
 */
int isValidPort(int port)
{
    if(port < 0 || port > 65535)
    {
        printf("Invalid port number.\n");
        return 0;
    }

    return 1;
}




