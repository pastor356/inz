#include "DatabaseAccessor.hpp"
#include "ConsoleLogger.hpp"

DatabaseAccessor::DatabaseAccessor(std::unique_ptr<influxdb::InfluxDB> conn):
	connection{std::move(conn)} {}

DatabaseAccessor::DatabaseAccessor(std::string const& url) {
	using namespace influxdb;
	connection = InfluxDBFactory::Get(url);
	connection->createDatabaseIfNotExists();
	for (auto i: connection->query("select * from temperature")) {
		logInfo <<i.getName()<<":";
		logInfo <<i.getTags()<<":";
		logInfo <<i.getFields()<<":";
		logInfo <<std::to_string(
				std::chrono::duration_cast<std::chrono::nanoseconds>(
						  i.getTimestamp().time_since_epoch()).count());
	}
}


DBStatus DatabaseAccessor::append(Sample const& sample) {
    if (!connection) {
        return DBStatus::ERROR;
    }

    try {
        auto point = influxdb::Point{std::string{sample.topic}}
            .addField("value", static_cast<long long>(sample.value))
            .setTimestamp(sample.timePoint);

        connection->write(std::move(point));
        return DBStatus::SUCCESS;
    } catch (const std::exception&) {
        return DBStatus::ERROR;
    }
}
