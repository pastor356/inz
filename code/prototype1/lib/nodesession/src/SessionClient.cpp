#include "SessionClient.hpp"

SessionClient::SessionClient(std::unique_ptr<IConnection> conn): connection(std::move(conn)) {
	Packet incoming;

	auto status = connection->recv(incoming);

	if(status != IConnection::Status::SUCCESS) {
		logError << std::string{"SessionClient(std::unique_ptr<IConnection>)"} + 
			" failed: conn->recv(incoming) failed";
	}

	std::memcpy(&sessionId, incoming.payload.data(), sizeof(sessionId));

	logInfo << std::string{"SessionClient(): got sessionId: "} + std::to_string(sessionId);
}


IConnection::Status SessionClient::recv(Packet& packet, int flag) {
	return(connection->recv(packet, flag));
}


IConnection::Status SessionClient::send(Packet& packet, int flag) {
	IConnection::Status status;

	auto time_stamp_ns = std::chrono::duration_cast<std::chrono::nanoseconds>
		(std::chrono::system_clock::now().time_since_epoch()).count();

	size_t time_stamp_size = sizeof(time_stamp_ns);

	packet.header.push_back(MSGTYPE_SESSIONMSG);
	packet.header.push_back(sessionId & 0xff);
	packet.header.push_back((sessionId >> 0x8) & 0xff);

	for(uint8_t i = 0; i < time_stamp_size ; ++i) { // push back timestamp into header
		packet.header.push_back((time_stamp_ns >> (i*0x8)) & 0xff);
	}

	
	if((status = connection->send(packet, flag)) != IConnection::Status::SUCCESS)
		logError << "Session::send() failed";


	return(status);
}


