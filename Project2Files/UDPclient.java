/**
 * Created by brett on 2/5/15.
 */
import java.io.*;
import java.net.*;
import java.net.DatagramSocket;

/*
 * UDPclient.java
 * Systems and Networks II
 * Project 2
 *
 * This file describes the functions to be implemented by the UDPclient class
 * You may also implement any helper functions you deem necessary to complete the project.
 */

public class UDPclient
{
    private DatagramSocket _socket; // the socket for communication with a server
    DatagramPacket packet;
    byte[] buf;

    /**
     * Constructs a UDPclient object.
     */
    public UDPclient()
    {
    }

    /**
     * Creates a datagram socket and binds it to a free port.
     *
     * @return - 0 or a negative number describing an error code if the connection could not be established
     */
    public int createSocket()
    {
        try {
            _socket = new DatagramSocket();
        }
        catch(Exception e)
        {
            System.out.println("New socket failed");
            return -1;
        }
        return 0;
    }

    /**
     * Sends a request for service to the server. Do not wait for a reply in this function. This will be
     * an asynchronous call to the server.
     *
     * @param request - the request to be sent
     * @param hostAddr - the ip or hostname of the server
     * @param port - the port number of the server
     *
     * @return - 0, if no error; otherwise, a negative number indicating the error
     */
    public int sendRequest(String request, String hostAddr, int port)
    {
        buf = new byte[256];

        for(int i = 0; i < request.length(); i++) {
            buf[i] = (byte)request.charAt(i);
        }

        try{
            request = request + '\0';
            InetAddress address = InetAddress.getByName(hostAddr);
            packet = new DatagramPacket(buf, buf.length, address, port);
            _socket.send(packet);
            System.out.println("Sent: " + request);
        }
        catch(Exception e)
        {
            System.out.println("sendRequest failed");
            return -1;
        }
        return 0;
    }

    /**
     * Receives the server's response following a previously sent request.
     *
     * @return - the server's response or NULL if an error occurred
     */
    public String receiveResponse()
    {
        packet = new DatagramPacket(buf, buf.length);
        String response;

        try{
            _socket.receive(packet);
            response = new String(packet.getData(), 0, packet.getLength());
            System.out.print("Received: ");
        }
        catch (Exception e)
        {
            System.out.println("receiveResponse() failed");
            return null;
        }
        return response;
    }

    /*
    * Prints the response to the screen in a formatted way.
    *
    * response - the server's response as an XML formatted string
    *
    */
    public static void printResponse(String response)
    {
        System.out.println("Received " + response);
    }


    /*
     * Closes an open socket.
     *
    * @return - 0, if no error; otherwise, a negative number indicating the error
     */
    public int closeSocket()
    {
        try{
            _socket.close();
        }
        catch(Exception e)
        {
            System.out.println("closeSocket() failed");
            return -1;
        }
        return 0;
    }
}