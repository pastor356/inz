#include "tcp.hpp"
#include <signal.h>
#include <iostream>


int exitflag = 0;
void sigint(int signo);

int main() {
	TcpAcc serv("127.0.0.1", "4242");
	serv.listen(5);

	struct sigaction act, oldact;
	act.sa_handler = sigint;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	sigaction(SIGINT, &act, &oldact);

	std::string msg;
	while(exitflag != 1) {

		try {
			std::unique_ptr<TcpConn> remotePeer = serv.accept();
			(*remotePeer).recv(msg);
		} catch (std::exception &ex) {
			std::cout << ex.what() << std::endl;
			if(errno == EINTR) break;
		}
		
		std::cout << msg << std::endl;

	}

	return(0);
}

void sigint(int signo) {
	std::cout << "\ncaught SIGINT" << std::endl;
	(void)signo;
	exitflag = 1;
	
}
