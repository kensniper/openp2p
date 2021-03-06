#ifndef P2P_ROOTNETWORK_CORE_RPCSERVER_HPP
#define P2P_ROOTNETWORK_CORE_RPCSERVER_HPP

#include <stdint.h>

#include <vector>

#include <p2p/Transport/Socket.hpp>

#include <p2p/Root/Endpoint.hpp>
#include <p2p/Root/Message.hpp>
#include <p2p/Root/NetworkId.hpp>
#include <p2p/Root/NodePair.hpp>

namespace p2p {

	namespace Root {
	
		namespace Core {
		
			class RPCServer {
				public:
					RPCServer(Socket<NodePair, Message>& socket);
					~RPCServer();
					
					Event::Source eventSource() const;
					
					void addNetwork(const std::string& networkName);
					
					bool processRequest();
					
				private:
					// Non-copyable.
					RPCServer(const RPCServer&) = delete;
					RPCServer& operator=(RPCServer) = delete;
					
					Socket<NodePair, Message>& socket_;
					std::vector<NetworkId> networks_;
					
			};
			
		}
		
	}
	
}

#endif
