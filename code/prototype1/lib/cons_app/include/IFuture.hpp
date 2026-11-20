#pragma once

class StatusFuture {
	IConnection::Status status;
public:
	StatusFuture(IConnection::Status st): status{st} {}
	StatusFuture() = default;

	void set(IConnection::Status st) { this->status = st; }
	IConnection::Status get() const { return(status); }
};
