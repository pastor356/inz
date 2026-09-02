#pragma once
#include <vector>
#include <string>
#include <deque>


struct Packet {
	std::vector<uint8_t> header; // or deque for push_front
	std::string payload;
};
