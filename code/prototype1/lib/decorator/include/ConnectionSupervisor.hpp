#include "TcpConnection.hpp"
#include "TcpSocket.hpp"


class ConnectionSupervisor {
	std::unique_ptr<IConnection> connection;
	std::shared_ptr<IQualityStrategy> qosStrategy;
	std::shared_ptr<ILiveMonitor> liveMonitor;

public:

	IConnection::Status sendPacket(std::string const& packet);
	IConnection::Status recvPacket(std::string& packet);

	void setQualityStrategy(std::shared_ptr<IQualityStrategy> qStrategy);

	ConnectionSupervisor(std::unique_ptr<IConnection> conn, 
			std::shared_ptr<IQualityStrategy> qStrategy, 
			std::shared_ptr<ILiveMonitor> liveMon);

};
	




