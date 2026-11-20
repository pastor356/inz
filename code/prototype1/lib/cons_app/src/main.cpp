#include "AppendRequest.hpp"
#include "DatabaseAccessor.hpp"
#include "Scheduler.hpp"
#include "Qos1.hpp"
#include "IConnection.hpp"
#include <signal.h>

volatile sig_atomic_t quitflag = 0;
void sigint(int signo);

char appname[64];

int main(int argc, char* argv[]) {
	(void) argc;
	memcpy(appname, argv[0], strlen(argv[0]));

	sigset_t mask;
	sigfillset(&mask);
	sigdelset(&mask, SIGINT);
	sigprocmask(SIG_SETMASK, &mask, NULL);

	struct sigaction act;
	act.sa_handler = sigint;
	act.sa_flags = 0; 
	sigemptyset(&act.sa_mask);
	sigaction(SIGINT, &act, NULL);


	enableLogger();
	
	auto db = std::make_shared<DatabaseAccessor>(
				 std::string{"http://localhost:8086?db=consAppDB"});


	if(db.get() == nullptr) {
		 logError << std::string{"Database access failed"};
	} else {
		logInfo << std::string{"Database connection established"};
	}


	while(quitflag != 1)  {


	}


	return(0);
}

void sigint(int signo) {
	if(signo == SIGINT) {
		logInfo << std::string{appname} + " quits";
		quitflag = 1;
	}
}

