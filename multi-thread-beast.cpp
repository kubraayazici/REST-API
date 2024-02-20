#include <iostream>
#include <string>
#include <thread>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/beast.hpp>

using namespace std;
using namespace boost::asio;
namespace beast = boost::beast;
using tcp = boost::asio::ip::tcp;

class BoostBeastRequestHandler {
public:
    void operator()(tcp::socket socket) const {
        try {
            beast::flat_buffer buffer;
            beast::http::request<beast::http::string_body> req;
            beast::http::read(socket, buffer, req);
            
            // Log the request details to the terminal
            cout << "Received Request:" << endl;
            cout << "Method: " << req.method() << endl;
            cout << "URI: " << req.target() << endl;
            cout << "HTTP Version: " << req.version() << endl;
            cout << "Client Address: " << socket.remote_endpoint().address().to_string() << endl;
            
            // Respond to the client
            beast::http::response<beast::http::string_body> res{beast::http::status::ok, req.version()};
            res.set(beast::http::field::server, "Boost Beast Server");
            res.set(beast::http::field::content_type, "text/html");
            res.body() = "Hello, this is Boost Beast REST server!";
            res.prepare_payload();
            beast::http::write(socket, res);
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }
};

// Function to run the server
void runServer() {
    try {
        io_context io;
        tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 8888));
        
        // Print server start message
        cout << "Starting Boost Beast REST server on port 8888..." << endl;

        while (true) {
            tcp::socket socket(io);
            acceptor.accept(socket);
            BoostBeastRequestHandler handler;
            thread{std::move(handler), std::move(socket)}.detach();
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

int main() {
    // Launch the server function in a separate thread
    thread serverThread(runServer);
    // Count in the main thread
    for (int i = 1; i <= 100; ++i) {
        cout << "Count: " << i << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    // Wait for the server thread to finish
    serverThread.join();

    return 0;
}
