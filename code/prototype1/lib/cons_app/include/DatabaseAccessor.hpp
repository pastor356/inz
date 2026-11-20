#pragma once
#include <InfluxDB/InfluxDBFactory.h>
#include <memory>
#include <cstdint>
#include <string_view>

enum class DBStatus: uint8_t {
	SUCCESS = 0,
	ERROR = 1
};

struct Sample {
	std::string_view topic;
	uint64_t value;
	std::chrono::system_clock::time_point timePoint;
};

class DatabaseAccessor {
	std::unique_ptr<influxdb::InfluxDB> connection;
public:
	DatabaseAccessor(std::unique_ptr<influxdb::InfluxDB> conn);
	DatabaseAccessor(std::string const& url);
	DBStatus append(Sample const& sample);
};
