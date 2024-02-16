// #include <iostream>
// #include <string>
// #include <map>
// #include <sstream>
// #include <boost/asio.hpp>

// using namespace std;
// namespace asio = boost::asio;
// using tcp = asio::ip::tcp;

// class SimpleRESTServer {
// public:
//     SimpleRESTServer(asio::io_context& io_context, short port)
//         : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
//         do_accept();
//     }

// private:
//     void do_accept() {
//         acceptor_.async_accept([this](std::error_code ec, tcp::socket socket) {
//             if (!ec) {
//                 std::make_shared<Session>(std::move(socket))->start();
//             }
//             do_accept();
//         });
//     }

//     class Session : public std::enable_shared_from_this<Session> {
//     public:
//         Session(tcp::socket socket) : socket_(std::move(socket)) {}

//         void start() {
//             do_read();
//         }

//     private:
//         void do_read() {
//             auto self(shared_from_this());
//             socket_.async_read_some(asio::buffer(data_, max_length),
//                                     [this, self](std::error_code ec, std::size_t length) {
//                 if (!ec) {
//                     std::string response = process_request(data_, length);
//                     do_write(response);
//                 }
//             });
//         }

//         void do_write(const std::string& response) {
//             auto self(shared_from_this());
//             asio::async_write(socket_, asio::buffer(response.c_str(), response.length()),
//                                [this, self](std::error_code ec, std::size_t /*length*/) {
//                 if (!ec) {
//                     // No need to shutdown or close the socket as we are handling one request and response per connection.
//                 }
//             });
//         }

//         std::string process_request(const char* data, std::size_t length) {
//             std::string request(data, length);
//             // Your logic to process the request and generate the response goes here.
//             return "HTTP/1.1 200 OK\r\nContent-Length: 5\r\nContent-Type: text/plain\r\n\r\nHello to me";
//         }

//         tcp::socket socket_;
//         enum { max_length = 1024 };
//         char data_[max_length];
//     };

//     tcp::acceptor acceptor_;
// };

// int main(int argc, char* argv[]) {
//     try {
//         if (argc != 2) {
//             std::cerr << "Usage: server <port>\n";
//             return 1;
//         }
//         asio::io_context io_context;
//         short port = std::atoi(argv[1]);
//         std::cout << __LINE__ << std::endl;
//         SimpleRESTServer server(io_context, port);
//         std::cout << __LINE__ << std::endl;
//         io_context.run();
//         std::cout << __LINE__ << std::endl;
//         // Display message indicating server is starting
//         std::cout << __LINE__ << std::endl;
//         std::cout << "Server is starting on port " << port << "..." << std::endl;

//         io_context.run();

//         // This line will be executed after the server stops running
//         std::cout << "Server has stopped." << std::endl;
//     } catch (std::exception& e) {
//         std::cerr << "Exception: " << e.what() << "\n";
//     }
//     return 0;
// }


#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;

using tcp = asio::ip::tcp;

class SimpleRESTServer {
public:
    SimpleRESTServer(asio::io_context& io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
        do_accept();
    }

private:
    void do_accept() {
        acceptor_.async_accept([this](std::error_code ec, tcp::socket socket) {
            if (!ec) {
                std::make_shared<Session>(std::move(socket))->start();
            }
            do_accept();
        });
    }

    class Session : public std::enable_shared_from_this<Session> {
    public:
        Session(tcp::socket socket) : stream_(std::move(socket)) {}

        void start() {
            do_read();
        }

    private:
        void do_read() {
            req_ = {};
            http::async_read(stream_, buffer_, req_,
                [self = shared_from_this()](std::error_code ec, std::size_t /*bytes_transferred*/) {
                    if (!ec) {
                        self->handle_request();
                    }
                });

        }

        void handle_request() {
            // Print a message indicating that a request has been received
            std::cout << "Received a request from: " << stream_.remote_endpoint().address().to_string() << std::endl;

            res_.version(req_.version());
            res_.result(http::status::ok);
            res_.set(http::field::server, "Simple REST Server");
            res_.set(http::field::content_type, "text/plain");
            res_.body() = "Hello from Boost.Beast REST Server!";
            res_.prepare_payload();

            http::async_write(stream_, res_,
                [self = shared_from_this()](const boost::system::error_code& ec, std::size_t /*bytes_transferred*/) {
                    if (!ec) {
                        self->stream_.shutdown(tcp::socket::shutdown_send);
                    }
                });


        }

        tcp::socket stream_;
        beast::flat_buffer buffer_;
        http::request<http::string_body> req_;
        http::response<http::string_body> res_;
    };

    tcp::acceptor acceptor_;
};

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: server <port>\n";
            return 1;
        }

        asio::io_context io_context;
        short port = std::atoi(argv[1]);
        SimpleRESTServer server(io_context, port);

        std::cout << "Server is starting on port " << port << "..." << std::endl;

        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}
