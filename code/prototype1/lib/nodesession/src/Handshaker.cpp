#include "Handshaker.hpp"

Handshaker::Handshaker(std::shared_ptr<IStateContext> ctx, 
		std::unique_ptr<IConnection> conn, 
		std::shared_ptr<IDataAccessor> dataAcc, 
		std::shared_ptr<NodeRegistry> nodeReg):
	context{ctx}, connection{std::move(conn)}, 
	dataAccessor{dataAcc}, nodeRegistry{nodeReg} {}


IConnection::Status Handshaker::recv(Packet& packet, int flag) {
	auto status = connection->recv(packet, flag);

	SessionConfig sc = parseMsg(packet);

	if(sc.newUsr) {
		auto authStatus = dataAccessor.createUser(sc.authData);

		if(authStatus != IDataAccessor::AccessorStatus::SUCCESS) {
			logError << "user creation failed";
		}

	} else {
		auto authStatus = dataAccessor.authorize(sc.authData);

		if(authStatus != IDataAccessor::AccessorStatus::SUCCESS) {
			logError << "user authorization failed";
		}
	}

	if(sc.creatFlag) 
		dataAccessor.addTopic(sc.topic);




}



