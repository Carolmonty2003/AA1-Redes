#pragma once
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Player.h"
#include "PacketTypes.h"


#define GRIDCOLUMN 6
#define GRIDROW 6

class GameManager
{
private:
  
  
    const short CELL_SIZE = 90;

   
    std::vector<Player> players;
    int currentTurnIndex = 0;
    float turnTimer = 0.0f;
    const float MAX_TURN_TIME = 20.0f;

    std::vector<int> victoryOrder;
    bool isGameOver = false;
    sf::Font font;
    int localPlayerID = 1;

    /// gx Columna 
    /// gy Fila 
    void BroadcastMove(int gx, int gy, int playerID);

    
    void ReceiveNetworkMoves();

   
    void HandlePeerDisconnection(sf::TcpSocket* socket);

    int GetPlayerIndexBySocket(sf::TcpSocket* socket) const;

public:
    
    std::vector<std::vector<short>> grid;

    GameManager();

   
    void InitGame(const std::vector<Player>& connectedPlayers,
                  int localID);

    
    void Update(float dt);

   
    void TryPlacePieceScreen(float mouseX, float mouseY);

 
    bool TryPlacePieceGrid(int gx, int gy, int playerIndex);

  
    bool CheckWin(int gx, int gy, int playerID);

   
    void AdvanceTurn();

    void CheckGameOver();

   
    void DrawGrid(sf::RenderWindow& window);

   
    void DrawHUD(sf::RenderWindow& window);
};
