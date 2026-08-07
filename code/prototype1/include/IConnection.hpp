#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <netdb.h>


class IConnection {
public:
	enum class Status: ssize_t {
		SUCCESS = 0,
		HUP, 
		ERROR
	};

	virtual void send(std::string const& msg, int flag = 0) const = 0;
	virtual Status recv(std::string& msg, int flag = 0) const = 0;
	virtual const std::string& getAddr() const = 0;
};


