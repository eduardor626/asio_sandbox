#include "TCPServer.hpp"
#include "Processors.hpp"

TcpServer::TcpServer(boost::asio::io_context &io_context, uint16_t port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)), factory_(std::make_shared<ProcessorFactory>())
{
  initialize_processors();
  start_accept();
}

void TcpServer::initialize_processors()
{
  factory_->registerProcessor("dog", std::make_shared<DogProcessor>());
  factory_->registerProcessor("cat", std::make_shared<CatProcessor>());
}

void TcpServer::start_accept()
{
  auto new_connection = std::make_shared<tcp::socket>(acceptor_.get_executor());
  acceptor_.async_accept(*new_connection, [this, new_connection](boost::system::error_code ec) {
    if (!ec) {
      on_accept(new_connection);
    }
    start_accept();
  });
}

// is called when a new connection is accepted. It reads data asynchronously from the client.
void TcpServer::on_accept(std::shared_ptr<tcp::socket> socket)
{
  auto buffer = std::make_shared<std::array<char, 1024>>();
  socket->async_read_some(boost::asio::buffer(*buffer),
                          [this, socket, buffer](boost::system::error_code ec, std::size_t length) {
                            if (!ec) {
                              std::string received_data(buffer->data(), length);
                              std::cout << "Received: " << received_data << std::endl;

                              std::istringstream iss(received_data);
                              std::string word;
                              std::vector<std::string> found_words;

                              while (iss >> word) {
                                if (word == "dog" || word == "cat") {
                                  found_words.push_back(word);
                                }
                              }

                              // Print found words
                              for (const auto &found_word : found_words) {
                                std::cout << "Found word: " << found_word << std::endl;
                                auto processor = factory_->getProcessor(found_word);
                                if (processor) {
                                  std::string reply = processor->process(found_word);
                                  send_message(socket, reply + "\n");
                                } else {
                                  send_message(socket, "error!\n");
                                }
                              }

                            } else {
                              std::cerr << "Error: " << ec.message() << std::endl;
                            }
                          });
}

// After sending, it waits to receive a response from the server.
void TcpServer::send_message(std::shared_ptr<tcp::socket> socket, const std::string &message)
{
  boost::asio::async_write(*socket, boost::asio::buffer(message),
                           [this](boost::system::error_code ec, std::size_t /*length*/) {
                             if (!ec) {
                               std::cout << "Message sent. Waiting for response..." << std::endl;
                               start_accept();
                             } else {
                               std::cerr << "Send error: " << ec.message() << std::endl;
                             }
                           });
}