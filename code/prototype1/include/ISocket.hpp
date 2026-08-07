#pragma once
#include "IConnection.hpp"
#include <memory>

class ISocket {
public:
	virtual std::unique_ptr<IConnection> accept() const = 0;
	virtual std::unique_ptr<IConnection> connect() const = 0;

	virtual void bind() const = 0;
	virtual void listen(int backlog) const = 0;

	virtual int getDescriptor() const = 0; 
};




