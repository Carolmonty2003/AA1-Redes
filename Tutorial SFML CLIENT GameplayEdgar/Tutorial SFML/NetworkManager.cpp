#include "NetworkManager.h"
#include "PacketTypes.h"
#include "SceneManager.h"
#include "GameScene.h"
#include "Player.h"

void NetworkManager::NetworkFetch()
{
    sf::Packet packet;
    if (serverSocket.receive(packet) == sf::Socket::Status::Done)
    {
        PacketTypes type;
        if (!(packet >> type)) return;

        switch (type)
        {
            case PacketTypes::STARTGAME:
            {
                int localID;
                int numPlayers;
                packet >> localID >> numPlayers;

                std::vector<Player> players;
                for (int i = 0; i < numPlayers; i++)
                {
                    int id;
                    std::string nick, ip;
                    unsigned short port;
                    short r, g, b;

                    packet >> id >> nick >> ip >> port >> r >> g >> b;

                    Player p(id, nick, 1000, sf::Color(r, g, b), (id == localID));
                    players.push_back(p);

                    if (id != localID)
                    {
                        AddConnection(ip, port);
                    }
                }

                if (auto scene = dynamic_cast<GameScene*>(SM.GetScene("GameScene"))) {
                    scene->SetupGame(players, localID);
                } else {
                    std::cerr << "Error: GameScene not found when starting game." << std::endl;
                }

                SM.SetNextScene("GameScene");
                std::cout << "Starting game with " << numPlayers << " players." << std::endl;

                break;
            }
            
            
            case PacketTypes::LOGIN:
            case PacketTypes::REGISTER:
            {
                bool success;
                std::string message;
                packet >> success >> message;
                std::cout << (success ? "Success: " : "Error: ") << message << std::endl;
                break;
            }

            case PacketTypes::NEXT_TURN:
            {
                int nextID;
                packet >> nextID;
                if (auto scene = dynamic_cast<GameScene*>(SM.GetScene("GameScene"))) {
                    scene->SyncNextTurn(nextID);
                }
                break;
            }

            default:
                break;
        }
    }
}
