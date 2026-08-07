#include "IConnection.hpp"

class TcpCallee: public TcpConnection {
public:
	TcpCallee(int sockFd, std::string const& addr, int port);
};
