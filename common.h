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


#define SEQUENCE_BIT 0
#define CORRUPT_BIT 1
#define ACK_BIT 2
//Returns sequence bit value
#define getSequenceBit(packet) ((packet >> SEQUENCE_BIT) & 1) ? 1 : 0

//Set sequence bit
//params: Byte, val
#define setSequenceBit(packet, val) packet |= val << SEQUENCE_BIT

//Set corrupt bit
//params: Byte to set, value
#define setCorruptBit(packet, val) packet |= val << CORRUPT_BIT

//Returns corrupt bit value
#define getCorruptBit(packet) ((packet >> CORRUPT_BIT) & 1) ? 1 : 0

//Set ACK bit
#define setAckBit(packet, val) packet |= val << ACK_BIT

//Get ACK Bit
#define getAckBit(packet) ((packet >> ACK_BIT) & 1) ? 1 : 0




typedef enum{
    DATA,
    ACK
}Segment_type;

typedef enum{
    ZERO,
    ONE
}Sequence;

typedef enum{
    NO,
    YES
}Corrupt;

typedef struct {
    byte HEADER;
    byte body[9];
}Segment;

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
