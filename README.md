# Concurrent_TCP_Server
A simple concurrent TCP server using C/C++ that listens on a specified port and handles incoming client connections using worker threads.

### Fulfilling following requirements:
1. A TCP server that listens on a specific port (e.g., Port 8080).
2. The server is able to handle multiple client connections concurrently using worker threads. Each client connection should be processed in its own thread.
3. When a client connects, the server accepts the connection, and the processing for that client done asynchronously in a separate worker thread.
4. In the worker thread, simulated some processing (e.g. generating random data).
5. The server continue listening for new client connections while serving existing clients concurrently.
6. It have a mechanism to gracefully shut down the server when required.
