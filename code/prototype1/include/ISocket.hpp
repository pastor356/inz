#pragma once
#include "IConnection.hpp"
#include <memory>
#include <cstring>
#include <memory>
#include <arpa/inet.h>
#include <string>
#include <memory>

class ISocket {
public:
	virtual std::unique_ptr<IConnection> accept() const = 0;
	virtual std::unique_ptr<IConnection> connect(struct sockaddr_storage addr) const = 0;

	virtual void bind() const = 0;
	virtual void listen(int backlog) const = 0;

	virtual int getDescriptor() const = 0; 
};




