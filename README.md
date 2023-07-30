# server-client-messaging-app
Server-Client Messaging using TCP/IP.

This project consists of a simple Server-Client messaging application developed using the C programming language. The application allows the transmission of text-based messages between a server and a client running on the same network.

## Features

->  Text-based messaging between server and client over TCP/IP sockets
->  End messaging sessions with the "quit" command

## How does it work?

->  The server listens for connection requests on the specified port and responds to incoming messages.

->  The client connects to the server over the specified IP and port and sends text-based messages to the server.

->  The server replies to the messages it receives and sends them back to the user.

->  The messaging session can be terminated by sending the "quit" command.

## Compile the server
->  gcc -o server server.c

## Compile the client
->  gcc -o client client.c

## Start the server
->  ./server

## Start the client
->  ./client
