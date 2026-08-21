#pragma once
#include "IQualityStrategy.hpp"


class Qos1: public IQualityStrategy {
	IConnection::Status send(std::string const& msg, int flag = 0) const override;
	IConnection::Status recv(std::string& msg, int flag = 0) const override;
};
