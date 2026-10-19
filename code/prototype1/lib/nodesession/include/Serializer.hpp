#pragma once
#include "Session.hpp"
#include "Handshaker.hpp"
#include "InitialState.hpp"
#include "Packet.hpp"

namespace Serializer {

Packet serializeHandshake(uint8_t role, uint8_t qos, bool creat, bool newUsr,
			uint8_t rights, std::string topic, std::string login,
			std::string password, std::string secret);

Packet serializeInitial(bool isReconnect = false);
Packet serializeReconnect(uint16_t sessionId);
Packet serializeSession(uint16_t sessionId);

};
