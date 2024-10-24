#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <string>

#include "CmdProcessor.hpp"

using boost::asio::ip::tcp;

class TcpServer
{
private:
  tcp::acceptor acceptor_;
  std::shared_ptr<ProcessorFactory> factory_;

public:
  TcpServer(boost::asio::io_context &io_context, uint16_t port);

private:
  void initialize_processors();

  void start_accept();
  void on_accept(std::shared_ptr<tcp::socket> socket);

  void send_message(std::shared_ptr<tcp::socket> socket, const std::string &message);
};