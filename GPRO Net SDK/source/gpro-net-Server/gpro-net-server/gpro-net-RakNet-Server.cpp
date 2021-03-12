/*
   Copyright 2021 Daniel S. Buckstein

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

	   http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

/*
	GPRO Net SDK: Networking framework.
	By Daniel S. Buckstein

	gpro-net-RakNet-Server.cpp
	Source for RakNet server management.
*/

#include "gpro-net/gpro-net-server/gpro-net-RakNet-Server.hpp"
#include <string>

const int MAX_TIMEOUT = 50000; //50 seconds?

struct User
{
	std::string username;
	std::string ip;
	bool connected; //for keeping track of status in case of a rapid disconnect and reconnect, so data isnt lost
};

struct Lobby
{
	User user1;
	User user2;
	//gametype gameType;
	std::string id; //lobby id
};

//unordered map of users to keep track of them
// unorderedmap userlist;

//unordered map of lobbies with their ID to keep track of them
//i forget how to declare a map
// unorderedmap lobbylist;

void lobbymenu()
{
	//let user pick from the following options:
	//make a lobby
	//join a lobby
	//spectate a lobby
	//display a list of lobbies
	//disconnect

	//complete their option via switch statement
}

void gameover()
{
	//keep track of when game ends, so that the lobby in question can close
	// remove said lobby from lobbylist to make room for new ones
}

namespace gproNet
{
	cRakNetServer::cRakNetServer()
	{
		RakNet::SocketDescriptor sd(SET_GPRO_SERVER_PORT, 0);
		unsigned short MAX_CLIENTS = 10;

		peer->Startup(MAX_CLIENTS, &sd, 1);
		peer->SetMaximumIncomingConnections(MAX_CLIENTS);
	}

	cRakNetServer::~cRakNetServer()
	{
		peer->Shutdown(0);
	}

	bool cRakNetServer::ProcessMessage(RakNet::BitStream& bitstream, RakNet::SystemAddress const sender, RakNet::Time const dtSendToReceive, RakNet::MessageID const msgID)
	{
		if (cRakNetManager::ProcessMessage(bitstream, sender, dtSendToReceive, msgID))
			return true;

		// server-specific messages
		switch (msgID)
		{
		case ID_NEW_INCOMING_CONNECTION:
			printf("A connection is incoming.\n");

			/*
				welcome user to the server
				save user's data such as username and IP in a user struct object then put that in user list
				run lobby menu
				lobbymenu();
			*/
			return true;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			printf("The server is full.\n");
			//reject connections if full
			return true;
		case ID_DISCONNECTION_NOTIFICATION:
			printf("A client has disconnected.\n");
			//say goodbye to user and remove them from the userlist
			return true;
		case ID_CONNECTION_LOST:
			printf("A client lost the connection.\n");
			return true;

			/*
				for(int i = 0; i < userlist.size(); i++)
				{
					if user has disconnected, set their bool to false but dont remove them until they time out
					i.e. dont reconnect within a set amount of time
				}

				for(int i = 0; i < lobbylist.size(); i++)
				{
					notify other player in lobby of their disconnection
					by searching for missing player in each lobby until they match
					tell user they are waiting for reconnection but give option for said player to leave
				}
			*/

			// test message
		case ID_GPRO_MESSAGE_COMMON_BEGIN:
		{
			// server receives greeting, print it and send one back
			RakNet::BitStream bitstream_w;
			ReadTest(bitstream);
			WriteTest(bitstream_w, "Hello client from server");
			peer->Send(&bitstream_w, MEDIUM_PRIORITY, UNRELIABLE_SEQUENCED, 0, sender, false);
		}	return true;

		}
		return false;
	}
}