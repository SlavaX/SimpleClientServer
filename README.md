# SimpleClientServer
This simple application created as a code example.
The most typical task before I get invitation for job interview is a client server application.
So here it is. There are three parts of it: sender, server and receiver. Server is a multithread application, which allow works with several clients.

How to run it
- run the server;
- run several of senders, they will send messages to the server;
- run several of receivers, which will take the messages from the server.

How it works
Server takes messages from the receivers, which have string type, and push them into the queue.
Receivers take the messages from the queue, one by one and print them out to console.
