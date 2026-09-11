#include "TcpCallee.hpp"

TcpCallee::TcpCallee(int sockFd, const struct sockaddr_storage addr): TcpConnection(sockFd, addr) {}

