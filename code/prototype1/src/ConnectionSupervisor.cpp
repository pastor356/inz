#include "ConnectionSupervisor.hpp"

IConnection::Status ConnectionSupervisor::sendPacket(std::string const& packet) {
	if((*qosStrategy).send(packet) == IConnection::Status::SUCCESS) {


}


ConnectionSupervisor::recvPacket(std::string& packet) {



