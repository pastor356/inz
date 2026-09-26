#include "Session.hpp"

Session::Session(std::unique_ptr<IConnection> conn, std::shared_ptr<IStateContext> ctx,
			std::string const& passwd, std::string const& secrt):
				connection{std::move(conn)}, 
				context{ctx}, 
				password{passwd}, 
				secrt{secret} {}



IConnection::Status Session::send(std::string const& message, int flag) const {
	Packet packet;
	packet.payload = message;

	auto status = this->send(packet, flag);

	return(status);
}




IConnection::Status Session::recv(Packet& packet, int flag = 0) const {
	auto status = connection->recv(packet, flag);
	std::string methodName = "Session::recv(Packet&)";

	if(status != IConnection::Status::SUCCESS) {
		logError << methodName + ": failed";
		return(status);
	}

	SessionHeader sh = parseMsg(packet);
	// what's inside? sessionID? what for?

	return(status);
}




IConnection::Status Session::recv(std::string& message, int flag) const {
	Packet packet;

	auto status = this->recv(packet, flag);
	
	message = packet.payload;

	return(status);
}


IConnection::Status Session::send(Packet& packet, int flag) const {
	IConnection::Status status;

	auto time_stamp_ns = std::chrono::duration_cast<std::chrono::nanoseconds>
		(std::chrono::system_clock::now().time_since_epoch()).count();

	packet.header.push_back(thisSessionId & 0xff);
	packet.header.push_back((thisSessionId >> 0x8) & 0xff);

	for(uint8_t i = 0; i < 8; ++i) { // push back timestamp into header
		packet.header.push_back((time_stamp_ns >> (i*0x8)) & 0xff);
	}
	
	if((status = connection->send(packet, flag)) != IConnection::Status::SUCCESS)
		logError << "Session::send() failed";

	return(status);
}



void Session::setConnection(std::unique_ptr<IConnection> conn) {
	this->connection = std::move(conn);
}
