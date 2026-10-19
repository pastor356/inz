#include "MessageRouter.hpp"

void MessageRouter::handleRead(std::shared_ptr<IStateContext> node) {
	Packet packet;
	auto status = node->recv(packet, recvFlag);

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
