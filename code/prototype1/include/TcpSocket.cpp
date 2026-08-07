#include "ISocket.hpp"

TcpSocket::TcpSocket(std::string const& ipAddr, int port): 
	sockFd{-1}, addr{ipAddr}, port{port} {}

TcpSocket::TcpSocket(int sockfd, std::string const& addr, int port): 
	sockFd{sockfd}, addr{addr}, port{port} {}




