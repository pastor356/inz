#pragma once
#include "EventHandler.hpp"
#include "IPoll.hpp"
#include "Reactor.hpp"


#define MSGTYPE_TOPICDATA		(uint8_t)(0xa << MsgType_Pos)
#define TOPICDATA_HEADERLEN		0x4

class MessageRouter: public EventHandler {
public:

	struct DataHeader {
		std::string topic;
		uint16_t dataLen;
	};

private:
	using NodeSet = std::vector<std::shared_ptr<IStateContext>>;
	std::shared_ptr<Reactor> reactor;

	std::unordered_map<std::string, NodeSet> topicsToConnections;

	DataHeader parseMsg(Packet const& packet) const;
	int recvFlag = 0;
	int sendFlag = 0;

public:
	MessageRouter(std::shared_ptr<Reactor> poll, int recvflag = 0, int sendflag = 0);

	void registerNode(std::string const& topic, std::shared_ptr<IStateContext> node);

	void handleRead(std::shared_ptr<IStateContext> node) override;
	void handleWrite(std::shared_ptr<IStateContext> node) override;
	void handleHup(std::shared_ptr<IStateContext> node) override;
	void handleError(std::shared_ptr<IStateContext> node) override;
};
