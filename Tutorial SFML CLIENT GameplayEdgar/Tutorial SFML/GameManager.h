#pragma once
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Player.h"

#define GRIDCOLUMN 6
#define GRIDROW 6

class GameManager
{
private:
    const short cellSize = 90;

    std::vector<Player> players;
    int currentTurnIndex = 0;
    float turnTimer = 0.0f;
    const float MAX_TURN_TIME = 20.0f;

    std::vector<int> victoryOrder;
    bool isGameOver = false;

    sf::Font font;

public:
	std::vector<std::vector<short>> grid;

	GameManager();

    void InitGame();
    void Update(float dt);
    
    void TryPlacePieceScreen(float mouseX, float mouseY);
    
   
    bool TryPlacePieceGrid(int gx, int gy, int playerIndex);

    bool CheckWin(int gx, int gy, int playerID);
    void AdvanceTurn();
    void CheckGameOver();

    void AddPieceToGrid(const std::vector<std::vector<short>>& piece, int offsetX = 0, int offsetY = 0);

    void DrawGrid(sf::RenderWindow& window);
    void DrawHUD(sf::RenderWindow& window);
};
