#pragma once

#include <iostream>
#include <boost/asio.hpp>

std::string read(boost::asio::ip::tcp::socket& socket);

void send(boost::asio::ip::tcp::socket& socket, const std::string& message);

void server_main();
