add_library(openp2p-rootnetwork STATIC
	AuthenticatedSocket.cpp
	Endpoint.cpp
	EndpointMapSocket.cpp
	NetworkId.cpp
	NodeDatabase.cpp
	NodeDetectSocket.cpp
	NodeId.cpp
	NodeInfo.cpp
	Packet.cpp
	PacketSocket.cpp
	PrivateIdentity.cpp
	PublicIdentity.cpp
	RoutineIdGenerator.cpp
	TransportSocket.cpp
	
	Core/RPCClient.cpp
	Core/RPCMessage.cpp
	Core/RPCServer.cpp
	Core/Service.cpp
	
	DHT/RPCClient.cpp
	DHT/RPCMessage.cpp
	DHT/RPCServer.cpp
	DHT/Service.cpp
	)
