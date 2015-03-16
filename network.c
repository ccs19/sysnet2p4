/*
 * network.c
 * Systems and Networks II
 * Project 4
 * Christopher Schneider & Brett Rowberry
 * This file implements the proxy network.
 */

int main(int argc, const char* argv[])
{

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

