#ifndef OPENP2P_SOCKET_HPP
#define OPENP2P_SOCKET_HPP

#include <OpenP2P/Buffer.hpp>

namespace OpenP2P {

	template <typename EndpointType>
	class Socket {
		public:
			virtual size_t send(const EndpointType& endpoint, const uint8_t* data, size_t size, Timeout timeout) = 0;
			
			virtual size_t receive(EndpointType* endpoint, uint8_t* data, size_t size, Timeout timeout) = 0;
			
			virtual void close() = 0;
			
	};
	
}

#endif
