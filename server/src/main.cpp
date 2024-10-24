#include "TCPServer.hpp"
#include <csignal>
#include <atomic>
#include <thread>

std::atomic<bool> running{true};
boost::asio::io_context* io_context_ptr = nullptr;

void signal_handler(int signum) {
    std::cout << "\nReceived signal " << signum << std::endl;
    running = false;
    if (io_context_ptr) {
        std::cout << "Stopping io_context..." << std::endl;
        io_context_ptr->stop();
    }
}

void setup_signal_handlers() {
    std::signal(SIGINT, signal_handler);  // Handle Ctrl+C
    std::signal(SIGTERM, signal_handler); // Handle termination request
}

int main() {
    try {
        setup_signal_handlers();
        
        // Create io_context and store its pointer for signal handler
        boost::asio::io_context io_context;
        io_context_ptr = &io_context;
        
        // Create the server
        const uint16_t port{3333};
        auto server_ptr = std::make_shared<TcpServer>(io_context, port);
        
        std::cout << "TCP Server started on port 12345" << std::endl;
        std::cout << "Press Ctrl+C to stop the server" << std::endl;
        
        // Run the io_context in a separate thread
        std::thread io_thread([&io_context]() {
            while (running) {
                try {
                    io_context.run();
                    if (!running) break;
                    io_context.restart();
                } catch (const std::exception& e) {
                    std::cerr << "Error in io_context thread: " << e.what() << std::endl;
                    break;
                }
            }
        });
        
        // Main thread waits here until signal is received
        while (running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        // Cleanup
        std::cout << "Shutting down server..." << std::endl;
        io_context.stop();
        if (io_thread.joinable()) {
            io_thread.join();
        }
        
        // Reset pointers
        server_ptr.reset();
        io_context_ptr = nullptr;
        
        std::cout << "Server shut down successfully" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }
}