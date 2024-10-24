#include <iostream>

#include <client.hpp>

int main() {
    try {
        boost::asio::io_context io_context;
        TcpClient client(io_context, "localhost", "3333");
        client.connect();
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}