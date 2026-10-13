#include "Serializer.hpp"

namespace Serializer {

Packet serializeHandshake(bool role, uint8_t qos, bool creat, bool newUsr,
				uint8_t rights, std::string topic, std::string login,
				std::string password, std::string secret) {
    Packet packet;

    uint8_t firstByte = MSGTYPE_HANDSHAKE;

    if(role) {
        firstByte |= (0b11 << RoleType_Pos);
    }

    firstByte |= ((qos & 0b11) << QosType_Pos);

    uint8_t secondByte = 0;

    if(creat) {
        secondByte |= (0b1 << CREATFLAG_Pos);
    }

    if(newUsr) {
        secondByte |= (0b1 << NEWUSRFLAG_Pos);
    }

    secondByte |= ((rights & 0b11) << RIGHTS_Pos);

    uint8_t loginLen  = static_cast<uint8_t>(login.size());
    uint8_t passwdLen = static_cast<uint8_t>(password.size());
    uint8_t secretLen = static_cast<uint8_t>(secret.size());
    uint8_t topicLen  = static_cast<uint8_t>(topic.size());

    packet.header.resize(HANDSHAKER_HEADERLEN);
    packet.header[0] = firstByte;   
    packet.header[1] = secondByte;  
    packet.header[2] = loginLen;    
    packet.header[3] = passwdLen;   
    packet.header[4] = secretLen;   
    packet.header[5] = topicLen;    

    packet.payload.reserve(topicLen + loginLen + passwdLen + secretLen);
    packet.payload.insert(packet.payload.end(), topic.begin(), topic.end());
    packet.payload.insert(packet.payload.end(), login.begin(), login.end());
    packet.payload.insert(packet.payload.end(), password.begin(), password.end());
    packet.payload.insert(packet.payload.end(), secret.begin(), secret.end());

    return(packet);
}


Packet serializeInitial(bool isReconnect) {
	Packet packet;
	if(isReconnect)
		packet.header.push_back(MSGTYPE_RECONNECT);
	else
		packet.header.push_back(MSGTYPE_HANDSHAKE);
	
	return(packet);
}


Packet serializeReconnect(uint16_t sessionId) {
	Packet packet;
	packet.header.push_back(MSGTYPE_RECONNECT);

	packet.header.push_back(sessionId & 0xff);
	packet.header.push_back((sessionId >> 0x8) & 0xff);

	return(packet);
}

};




