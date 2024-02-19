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

// Define a class for counting
class CountRunnable : public Runnable {
public:
    void run() override {
        for (int i = 1; i <= 35; ++i) {
            std::cout << "Count: " << i << endl;
            Thread::sleep(1000); // Sleep for 1 second
        }
    }
};

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
        ostr << "Hello, this is Poco REST server!!!";
    }
};

// Define a factory for creating PocoRequestHandler instances
class PocoRequestHandlerFactory : public HTTPRequestHandlerFactory {
public:
    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request) override {
        return new PocoRequestHandler;
    }
};

class MyApp : public ServerApplication {
protected:
    int main(const vector<string>& args) override {
        // Create and start the counting thread
        CountRunnable countRunnable;
        Thread countThread;
        countThread.start(countRunnable);

        // Create and start the Poco HTTP server
        try {
            HTTPServerParams* pParams = new HTTPServerParams;
            HTTPServer server(new PocoRequestHandlerFactory, ServerSocket(8001), pParams);
            server.start();
            std::cout << "Starting Poco HTTP server on port 8001..." << endl;

            // Wait for CTRL-C or SIGINT
            waitForTerminationRequest();

            // Stop the server
            server.stop();

            // Wait for the counting thread to finish
            countThread.join();

            return Application::EXIT_OK;
        } catch (Exception& e) {
            cerr << "Error: " << e.displayText() << endl;
            return Application::EXIT_SOFTWARE;
        }
    }
};

int main(int argc, char** argv) {
    MyApp app;
    std::cout<<"Starting App"<<std::endl;
    return app.run(argc, argv);
}
