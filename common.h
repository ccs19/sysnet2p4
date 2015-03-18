/*
 * common.h
 * Systems and Networks II
 * Project 4
 * Christopher Schneider & Brett Rowberry
 * This file contains definitions common to the rdtSender, rdtReceiver, and network.
 */

#ifndef _SYSNET2P4_COMMONDEFINITIONS_H_
#define _SYSNET2P4_COMMONDEFINITIONS_H_

typedef unsigned char byte;

/*
 * The size of a segment in bytes.
 */
int SEGMENT_SIZE = 10;

/*
 * Maximum length of a hostname
 */
int MAX_HOSTNAME_LENGTH = 256;

/*
 * The size of a segment in bytes.
 */
int MAX_MESSAGE_SIZE = 256;

/*
 * Checks if given port is a legal port number. Returns 1 if valid, 0 if invalid.
 *
 * port - input port
 *
 * return int
 */
int isValidPort(int port);

/*
 * Checks if given port is valid percentage. Returns 1 if valid, 0 if invalid.
 *
 * percentage - input percentage
 *
 * return int
 */
int isValidPercentage(int percentage);

#endif //_SYSNET2P4_COMMONDEFINITIONS_H_
