#include "IPoll.hpp"
#include "EPollWaiter.hpp"

class Demultiplexer: public IPoll {
	std::unordered_map<int, std::shared_ptr<EventEntity>> fdToEntity;
	std::unordered_map<std::shared_ptr<IStateContext>, int> nodeToFd;

	std::unique_ptr<IWaiter> waiter;

	int epollFd = -1;
	int epollTimeout = -1;

	void removeFromFdToEntity(int fdToRemove);

public:
	Demultiplexer(int timeout = -1);
	Demultiplexer(std::unique_ptr<IWaiter> wait, int timeout = -1);

	std::vector<IPoll::ReadyEvent> wait() const override;

	void registerNode(std::shared_ptr<IStateContext> node, 
			std::shared_ptr<EventHandler> handler) override;

	void deregisterNode(std::shared_ptr<IStateContext> node) override;
	void deregisterHandler(std::shared_ptr<EventHandler> handler) override;
};
