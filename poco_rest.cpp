#include <iostream>
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"

using namespace Poco::Net;
using namespace Poco::Util;

// Define HelloRequestHandler
class HelloRequestHandler : public HTTPRequestHandler {
public:
    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) override {
        response.setChunkedTransferEncoding(true);
        response.setContentType("text/plain");
        std::ostream& ostr = response.send();
        ostr << "Hello from POCO REST Server!" << std::endl;
    }
};

// Define HelloRequestHandlerFactory
class HelloRequestHandlerFactory : public HTTPRequestHandlerFactory {
public:
    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest&) override {
        return new HelloRequestHandler;
    }
};

class RESTServerApp : public ServerApplication {
protected:
    int main(const std::vector<std::string>&) override {
        // Instantiate the server with the custom request handler factory
        HTTPServer server(new HelloRequestHandlerFactory, ServerSocket(8080), new HTTPServerParams);
        server.start();
        waitForTerminationRequest();
        server.stop();
        return Application::EXIT_OK;
    }
};

int main(int argc, char** argv) {
    RESTServerApp app;
    return app.run(argc, argv);
}
