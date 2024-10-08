# TCP Client-Server Application in C++

This project implements a simple TCP client-server application in C++ using Linux sockets. The project demonstrates the usage of sockets for communication between multiple clients and a server in a multi-threaded environment. Each client periodically connects to the server, sends a timestamped message, and the server records these messages in a log file.

## Features

- **Multi-threaded server**: Each client connection is handled in a separate thread.
- **Simultaneous client connections**: Multiple clients can connect to the server concurrently.
- **Log file**: All client messages are logged to `log.txt` with timestamps.
- **Object-Oriented Design**: The code is structured following OOP principles, ensuring clarity and reusability.

## How It Works

### Client

The client takes the following parameters:
1. Client name (string)
2. Server port (integer)
3. Connection period (in seconds)

The client connects to the server at regular intervals, defined by the period parameter, and sends a message in the format:
[yyyy-mm-dd hh:mm.ms] "client_name"

### Server

The server takes one parameter: the port number on which it listens for incoming client connections. The server logs each message it receives from clients into a file called `log.txt`. Each client connection is handled in a separate thread to ensure concurrent access to the server.

## Getting Started

### Prerequisites

- Linux environment
- C++ compiler (g++)
- POSIX threads

### Compilation

To compile the server and client, run the following commands in the project directory:

```bash
mkdir build
cd build
cmake ..
make
```

## Usage
### Running the Server
To start the server on a specific port:

```
bash
./server <port_number>
```

### Running the Clients
To start a client with a specific name, connecting to the server at regular intervals:

```
bash
./client <client_name> <port_number> <period_in_seconds>
```
For Example: 

```
bash
./client Name1 3000 1
./client Name2 3000 2
./client Name3 3000 3

```
Example Output
When the server is running and multiple clients are connected, the log.txt file will look similar to:
```csharp
[2018-09-19 13:50:01.000] Name1
[2018-09-19 13:50:02.000] Name1
[2018-09-19 13:50:02.010] Name2
[2018-09-19 13:50:03.000] Name1
[2018-09-19 13:50:03.010] Name3
...
```

## Project Structure
- server: Contains the server implementation.

- client: Contains the client implementation.

- common: Contains addictive library for both implementations.
- log.txt: The file where the server logs client messages.
