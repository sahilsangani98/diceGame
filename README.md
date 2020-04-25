
A small multiple client server dice game to demonstrate workings of socket programming in C.

Steps for Running the Server
1. Compile the projectServer.c file using following command:
gcc projectServer.c

2. Run the projectServer using the following command signature:
./a.out <PORTNO>
EG: ./a.out 7000
Note:Command signature - "a.out portnumber"

3. Next step is to start the projectClient.c

4. Similary to server file compile the client file by the following command:
gcc projectClient.c

5. Next step is to run the client 1 with the following command signature:
./.a.out <IPADDRESS> <PORTNO>
EG: ./a.out 123.123.123.123 700

6. Next step is to run the client 2 with the following command signature:
./.a.out <IPADDRESS> <PORTNO>
EG: ./a.out 123.123.123.123 700

Note:
1.Keep the port number same for the client and the server.
2.Use the IPADDRESS of the server while running the client.
