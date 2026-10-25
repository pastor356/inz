#include "MessageRouter.hpp"

MessageRouter::MessageRouter(std::shared_ptr<Reactor> poll, int recvflag, int sendflag): 
	reactor{poll}, recvFlag{recvflag}, sendFlag{sendflag} {}


void MessageRouter::handleRead(std::shared_ptr<IStateContext> node) {
	const char* methodName = "MessageRouter::handleRead(std::shared_ptr<IStateContext>)";
	Packet packet;
	auto status = node->recv(packet, recvFlag);

	if(packet.header.size() == 0) {
		logError << std::string{methodName} + " failed: header size is 0";
		return;
	}

	if(packet.header[0] == MSGTYPE_TOPICDATA) {
		DataHeader dh = this->parseMsg(packet);

		packet.header.clear();
		auto nodesOnTopic = topicsToConnections.find(dh.topic);

		if(nodesOnTopic == topicsToConnections.end()) {
			NodeSet newSet;
			newSet.push_back(node);
			topicsToConnections.emplace(dh.topic, std::move(newSet));
		}

		NodeSet nodes = nodesOnTopic->second;

		for(size_t i = 0; i < nodes.size(); ++i) {
			if(nodes[i]->isConsumer()) 
				nodes[i]->send(packet, sendFlag);
		}
	}
}



MessageRouter::DataHeader MessageRouter::parseMsg(Packet const& packet) const {
	const char* methodName = "MessageRouter::parseMsg(Packet const&)";
	DataHeader res;
	std::string topic;

	if(packet.header.size() < TOPICDATA_HEADERLEN) {
		logError << std::string{methodName} + " failed: invalid header length";
		return(res);
	}

	uint8_t msgType = packet.header[0];
	
	if(msgType != MSGTYPE_TOPICDATA) {
		logError << std::string{methodName} + " failed: invalid message type";
		return(res);
	}

	uint8_t topicLen = packet.header[1];

	res.topic.assign(packet.header.begin() + 2, packet.header.begin() + 2 + topicLen); 

	return(res);
}


void MessageRouter::handleWrite(std::shared_ptr<IStateContext> node) {}

void MessageRouter::handleHup(std::shared_ptr<IStateContext> node) {}

void MessageRouter::handleError(std::shared_ptr<IStateContext> node) {}


void MessageRouter::registerNode(std::string const& topic, std::shared_ptr<IStateContext> node) {
	const char* methodName = 
		"MessageRouter::registerNode(std::string const&, std::shared_ptr<IStateContext>)";

	if(topicsToConnections.find(topic) != topicsToConnections.end()) {
		topicsToConnections[topic].push_back(node);
	} else {
		auto res = topicsToConnections.try_emplace(topic, std::vector{node});

		if(!res.second) {
			logError << std::string{methodName} + " failed to emplace new topic";
		}
	}
}




