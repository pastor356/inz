#include "tcp.hpp"

int main() {
	TcpClient cli{"127.0.0.1", "4242"};

	std::unique_ptr<TcpConn> conn = cli.connect();

	(*conn).send(std::string{"blalba!"});

	return(0);
}


