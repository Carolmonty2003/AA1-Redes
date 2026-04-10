#pragma once
#include <SFML/Network.hpp>
enum PacketTypes
{
	HANDSHAKE,      
	LOGIN,          
	REGISTER,       
	LOBBYCREATION,  
	STARTGAME,      
	PIECEADDED,     
	NEXT_TURN,      
	ENDGAME,        
	RANKINGUPDATE,  
	DISCONNECT,     
	PLAYER_DISCONNECTED 
};



inline sf::Packet& operator <<(sf::Packet& packet, PacketTypes& type)
{
	return packet << static_cast<short>(type);
}


inline sf::Packet& operator >>(sf::Packet& packet, PacketTypes& type)
{
	short packetType;
	packet >> packetType;
	type = static_cast<PacketTypes>(packetType);
	return packet;
}