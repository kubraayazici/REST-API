# Multi-threaded HTTP Servers

This repository contains implementations of multi-threaded HTTP servers in C++ using different libraries.

## Boost Beast HTTP Server

The `multi-thread-beast.cpp` file provides a straightforward implementation of a multi-threaded HTTP server using the Boost.Beast library in C++. The server is designed to handle incoming HTTP requests concurrently by utilizing multiple threads, allowing for efficient processing of client requests.

## Poco HTTP Server

The 'multi-thread-poco.cpp' file do the same thing with `multi-thread-beast.cpp` file. It is using the Poco library in C++. 

### Prerequisites

Before running the Boost Beast server, make sure you have Boost installed on your system. You can install it using your package manager or download it from the [official Boost website](https://www.boost.org/users/download/).


NOTE: Before running the Poco server, make sure you have the Poco C++ Libraries installed on your system. You can download and install Poco from the [official Poco website](https://pocoproject.org/download/index.html).

### Compilation

Use the following command to compile the Boost Beast server:

```bash
g++ -std=c++11 -o output_beast multi-thread-beast.cpp -I/path/to/boost/include -L/path/to/boost/lib -lboost_system -lpthread
```

### Run 

```bash
./output_beast
```

### Compilation

Use the following command to compile the Poco server:

```bash
g++ -std=c++11 -o output_poco multi-thread-poco.cpp -I/path/to/poco/include -L/path/to/poco/lib -lPocoNet -lPocoUtil -lPocoFoundation -lpthread
```

### Run 

```bash
./output_poco
```


