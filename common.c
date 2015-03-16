/*
 * common.c
 * Systems and Networks II
 * Project 4
 * Christopher Schneider & Brett Rowberry
 * This file implementations of functions common to the rdtSender, rdtReceiver, and network.
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

/*
 * Checks if given port is valid percentage. Returns 1 if valid, 0 if invalid.
 *
 * percentage - input percentage
 *
 * return int
 */
int isValidPercentage(int percentage)
{
    if(percentage < 0 || percentage > 100)
    {
        printf("Invalid percentage.\n");
        return 0;
    }
    return 1;
}




