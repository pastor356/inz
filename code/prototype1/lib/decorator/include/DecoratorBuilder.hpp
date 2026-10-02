#pragma once
#include "ConnectionDecorator.hpp"
#include "LiveMonitor.hpp"
#include "Qos1.hpp"

class DecoratorBuilder {
	std::unique_ptr<IConnection> connection;
public:
	DecoratorBuilder() = default;

	std::unique_ptr<IConnection> getDecorator();
	void setConnection(std::unique_ptr<IConnection> conn); 
	void setQos1();
	//void setQos2() = default;
	void setLiveMon(); 
};


