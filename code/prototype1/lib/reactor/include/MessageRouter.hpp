#pragma once
#include "EventHandler.hpp"
#include "IPoll.hpp"

struct DataHeader {
	std::string topic;
	uint16_t dataLen;
};

#define MSGTYPE_TOPICDATA		(uint8_t)(0xa << MsgType_Pos)
#define TOPICDATA_HEADERLEN		0x4

class MessageRouter: public EventHandler {
	using NodeSet = std::vector<std::shared_ptr<IStateContext>>;

	std::weak_ptr<IPoll> reactor;
	std::unordered_map<std::string, NodeSet> topicsToConnections;

	DataHeader parseMsg(Packet const& packet) const;
	int recvFlag = 0;
	int sendFlag = 0;

public:
	void handleRead(std::shared_ptr<IStateContext> node) override;
	void handleWrite(std::shared_ptr<IStateContext> node) override;
	void handleHup(std::shared_ptr<IStateContext> node) override;
	void handleError(std::shared_ptr<IStateContext> node) override;
};
