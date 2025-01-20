/*
Exercise: Building a Simple Concurrent TCP Server

Objective: Create a simple concurrent TCP server using C/C++ that listens on a specified port and handles incoming client connections using worker threads.

Requirements:

1. Implement a TCP server that listens on a specific port (e.g., Port 8080).
2. The server should be able to handle multiple client connections concurrently using worker threads. Each client connection should be processed in its own thread.
3. When a client connects, the server should accept the connection, and the processing for that client should be done asynchronously in a separate worker thread.
4. In the worker thread, simulate some processing (e.g., calculating the factorial of a number, generating random data, or any other computationally intensive task).
5. The server should continue listening for new client connections while serving existing clients concurrently.
6. Implement a mechanism to gracefully shut down the server when required (e.g., by pressing a specific key combination or sending a termination signal).

*/



#include <iostream>  // For standard input and output operations (e.g., cout, cerr)
#include <thread>    // For threading support (e.g., std::thread)
#include <cstring>   // For C-style string manipulation functions (e.g., strlen)
#include <cstdlib>   // For standard library functions (e.g., rand, atoi)
#include <cstdio>    // For C-style input and output operations (e.g., printf)
#include <unistd.h>  // For POSIX operating system API (e.g., close)
#include <sys/socket.h>     // For socket-related functions and structures
#include <netinet/in.h>     // For Internet address family structures (e.g., struct sockaddr_in)
#include <csignal>   // For handling signals (e.g., SIGINT)
#include <atomic>    // For atomic variables used in multi-threading scenarios

#define PORT 8080
#define MAX_CLIENTS 5
std::atomic<bool> stopServer(false);

void handleClient(int clientSocket) {
      if (signal == SIGINT) {
          std::cout << "Received termination signal. Shutting down the server...\n";
          stopServer.store(true);
     }

     // Log the start of client processing
     std::cout << "Processing client in thread: " << std::this_thread::get_id() << std::endl; 

    // Simulate processing by sleeping for a random time between 10 to 14 seconds
    std::this_thread::sleep_for(std::chrono::seconds(rand() % 5 + 10));

    // Example processing: sending a message back to the client
    const char* message = "Hello from server!";
    send(clientSocket, message, strlen(message), 0);

    // Close the client socket
    close(clientSocket);

    // Log the end of client processing along with the thread ID
    std::cout << "Client processing completed in thread: " << std::this_thread::get_id() << std::endl;
}

int main() {
    // Set up signal handler
    signal(SIGINT, signalHandler);

    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket\n";
        return 1;
    }

    // Prepare the sockaddr_in structure
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Bind
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Bind failed\n";
        return 1;
    }

    // Listen
    if (listen(serverSocket, MAX_CLIENTS) < 0) {
        std::cerr << "Listen failed\n";
        return 1;
    }

    std::cout << "Server listening on port " << PORT << std::endl;
    
    while (!stopServer) {
        // Accept connection from an incoming client
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket < 0) {
            std::cerr << "Accept failed\n";
            return 1;
        }

        std::cout << "New client connected\n";

        // Create a new thread to handle the client
        std::thread clientThread(handleClient, clientSocket);
        clientThread.detach(); // Detach the thread to let it run independently
    }

    // Close the server socket
    close(serverSocket);

    return 0;
}
