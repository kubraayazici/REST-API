#include <iostream>
#include <chrono>
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Poco/Util/ServerApplication.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace Poco::Util;

// Define a request handler for the Poco HTTP server
class PocoRequestHandler : public HTTPRequestHandler {
public:
    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) override {
         // Log the request details to the terminal
        cout << "Received Request:" << endl;
        cout << "Method: " << request.getMethod() << endl;
        cout << "URI: " << request.getURI() << endl;
        cout << "HTTP Version: " << request.getVersion() << endl;
        cout << "Client Address: " << request.clientAddress().toString() << endl;
        
        // Respond to the client
        response.setStatus(HTTPResponse::HTTP_OK);
        response.setContentType("text/html");
        ostream& ostr = response.send();
        ostr << "Hello, this is Poco REST server!";
    }
};

// Define a factory for creating PocoRequestHandler instances
class PocoRequestHandlerFactory : public HTTPRequestHandlerFactory {
public:
    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request) override {
        return new PocoRequestHandler;
    }
};

// Define a class that encapsulates the server behavior
class PocoServerThread : public Runnable {
private:
    bool shouldStop;

public:
    PocoServerThread() : shouldStop(false) {}

    void run() override {
        try {
            HTTPServerParams* pParams = new HTTPServerParams;
            HTTPServer server(new PocoRequestHandlerFactory, ServerSocket(8000), pParams);
            cout << "Before starting Poco HTTP server..." << endl;
            server.start();
            cout << "Starting Poco HTTP server on port 8000..." << endl;
            while (!shouldStop) {
                Thread::sleep(1000);
            }
            server.stop();
        } catch (Exception& e) {
            cerr << "Error: " << e.displayText() << endl;
        }
    }

    void stop() {
        shouldStop = true;
    }
};

int main(int argc, char** argv) {
    // Create and start the Poco server in a separate thread
    PocoServerThread serverThread;
    Thread thread;
    thread.start(serverThread);

    // Count in the main thread
    for (int i = 1; i <= 100; ++i) {
        cout << "Count: " << i << endl;
        Thread::sleep(1000); // Sleep for 1 second
    }

    // Stop the server thread
    serverThread.stop();
    thread.join();

    return 0;
}
