#include <boost/asio.hpp>

#include <iostream>
#include <string>

using boost::asio::ip::tcp;

// TcpClient class that manages the connection to the server.
class TcpClient
{

public:
    // The constructor resolves the server address and port.
    TcpClient(boost::asio::io_context &io_context, const std::string &host, const std::string &port)
        : socket_(io_context)
    {
        tcp::resolver resolver(io_context);
        endpoints_ = resolver.resolve(host, port);
    }

    // connect() method establishes an asynchronous connection to the server
    void connect()
    {
        boost::asio::async_connect(socket_, endpoints_,
                                   [this](boost::system::error_code ec, tcp::endpoint)
                                   {
                                       if (!ec)
                                       {
                                           std::cout << "Connected to server." << std::endl;
                                           send_message("cat");
                                       }
                                       else
                                       {
                                           std::cerr << "Connection error: " << ec.message() << std::endl;
                                       }
                                   });
    }

private:
    // After sending, it waits to receive a response from the server.
    void send_message(const std::string &message)
    {
        boost::asio::async_write(socket_, boost::asio::buffer(message),
                                 [this](boost::system::error_code ec, std::size_t /*length*/)
                                 {
                                     if (!ec)
                                     {
                                         std::cout << "Message sent. Waiting for response..." << std::endl;
                                         start_receive();
                                     }
                                     else
                                     {
                                         std::cerr << "Send error: " << ec.message() << std::endl;
                                     }
                                 });
    }

    void start_receive()
    {
        socket_.async_read_some(boost::asio::buffer(data_, max_length),
                                [this](boost::system::error_code ec, std::size_t length)
                                {
                                    if (!ec)
                                    {
                                        std::cout << "Received: " << std::string(data_, length) << std::endl;
                                    }
                                    else
                                    {
                                        std::cerr << "Receive error: " << ec.message() << std::endl;
                                    }
                                });
    }

    tcp::socket socket_;
    tcp::resolver::results_type endpoints_;
    enum
    {
        max_length = 1024
    };
    char data_[max_length];
};