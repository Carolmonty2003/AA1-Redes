#include "GameManager.h"
#include <string>

GameManager::GameManager()
{
    grid.assign(GRIDCOLUMN, std::vector<short>(GRIDROW, 0));
    
    // Load standard Windows font
    if (!font.openFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
        std::cerr << "Warning: Failed to open arial.ttf font." << std::endl;
    }
}

void GameManager::InitGame()
{
    grid.assign(GRIDCOLUMN, std::vector<short>(GRIDROW, 0));
    
    // Create 4 players
    players.clear();
    players.push_back(Player(1, "Player 1 (You)", 1250, sf::Color::Cyan, true));
    players.push_back(Player(2, "Player 2 (Bot)", 1100, sf::Color::Red, false));
    players.push_back(Player(3, "Player 3 (Bot)", 1350, sf::Color::Green, false));
    players.push_back(Player(4, "Player 4 (Bot)",  950, sf::Color::Yellow, false));
    
    currentTurnIndex = 0;
    turnTimer = MAX_TURN_TIME;
    victoryOrder.clear();
    isGameOver = false;
    
    std::cout << "--- Partida Iniciada ---" << std::endl;
}

void GameManager::Update(float dt)
{
    if (isGameOver) return;

    turnTimer -= dt;
    if (turnTimer <= 0.0f)
    {
        std::cout << "Tiempo agotado para " << players[currentTurnIndex].nickName << ". Se salta el turno." << std::endl;
        AdvanceTurn();
        return;
    }

    // Bot logic
    if (!players[currentTurnIndex].isLocal)
    {
        // Wait ~2 seconds then place a piece somewhere random
        if (MAX_TURN_TIME - turnTimer > 2.0f)
        {
            std::vector<std::pair<int, int>> emptySpaces;
            for (int x = 0; x < GRIDCOLUMN; x++) {
                for (int y = 0; y < GRIDROW; y++) {
                    if (grid[x][y] == 0) {
                        emptySpaces.push_back({x, y});
                    }
                }
            }

            if (!emptySpaces.empty()) {
                int r = rand() % emptySpaces.size();
                TryPlacePieceGrid(emptySpaces[r].first, emptySpaces[r].second, currentTurnIndex);
            }
        }
    }
}

void GameManager::TryPlacePieceScreen(float mouseX, float mouseY)
{
    if (isGameOver) return;
    if (!players[currentTurnIndex].isLocal) return; // Not local player's turn

    float offsetX = (800.f - (GRIDCOLUMN * cellSize)) / 2.f;
    float offsetY = (600.f - (GRIDROW * cellSize)) / 2.f;

    if (mouseX < offsetX || mouseX > offsetX + (GRIDCOLUMN * cellSize) ||
        mouseY < offsetY || mouseY > offsetY + (GRIDROW * cellSize)) {
        return; // Click outside grid
    }

    int gx = (int)((mouseX - offsetX) / cellSize);
    int gy = (int)((mouseY - offsetY) / cellSize);

    TryPlacePieceGrid(gx, gy, currentTurnIndex);
}

bool GameManager::TryPlacePieceGrid(int gx, int gy, int playerIndex)
{
    if (gx < 0 || gx >= GRIDCOLUMN || gy < 0 || gy >= GRIDROW) return false;

    if (grid[gx][gy] != 0) {
        if (players[playerIndex].isLocal) {
            std::cout << "Turno invalido: Posicion acupada." << std::endl;
        }
        return false; // Cell occupied
    }

    grid[gx][gy] = players[playerIndex].id;
    std::cout << ">>> " << players[playerIndex].nickName << " coloca ficha en la posicion (" << gx << ", " << gy << ")" << std::endl;

    if (CheckWin(gx, gy, players[playerIndex].id))
    {
        std::cout << ">>> OMG! " << players[playerIndex].nickName << " HA GANADO su linea! Pasa a modo espectador." << std::endl;
        players[playerIndex].isSpectator = true;
        victoryOrder.push_back(players[playerIndex].id);
        CheckGameOver();
    }
    
    if (!isGameOver) {
        AdvanceTurn();
    }

    return true;
}

bool GameManager::CheckWin(int gx, int gy, int playerID)
{
    int directions[4][2][2] = {
        {{-1, 0}, {1, 0}},  // Horizontal
        {{0, -1}, {0, 1}},  // Vertical
        {{-1, -1}, {1, 1}}, // Diagonal \ 
        {{-1, 1}, {1, -1}}  // Diagonal /
    };

    for (int d = 0; d < 4; ++d) {
        int count = 1; 
        for (int i = 0; i < 2; ++i) { 
            int k = 1;
            while(true) {
                int nx = gx + directions[d][i][0] * k;
                int ny = gy + directions[d][i][1] * k;
                if (nx < 0 || nx >= GRIDCOLUMN || ny < 0 || ny >= GRIDROW) break;
                if (grid[nx][ny] == playerID) {
                    count++;
                    k++;
                } else {
                    break;
                }
            }
        }
        if (count >= 3) {
            return true;
        }
    }
    return false;
}

void GameManager::AdvanceTurn()
{
    // Check for draw (full board)
    bool boardFull = true;
    for (int x = 0; x < GRIDCOLUMN; x++) {
        for (int y = 0; y < GRIDROW; y++) {
            if (grid[x][y] == 0) boardFull = false;
        }
    }

    if (boardFull) {
        std::cout << "Tablero lleno. NINGUN ESPACIO." << std::endl;
        CheckGameOver();
        return;
    }

    int nextIndex = currentTurnIndex;
    int safeguard = 0;
    while(true) {
        nextIndex = (nextIndex + 1) % players.size();
        safeguard++;
        if (!players[nextIndex].isSpectator) {
            currentTurnIndex = nextIndex;
            std::cout << "--- Turno de " << players[currentTurnIndex].nickName << " ---" << std::endl;
            break;
        }
        if (safeguard > 10) {
            CheckGameOver(); // All are spectators or error
            break;
        }
    }
    turnTimer = MAX_TURN_TIME; // Reset timer
}

void GameManager::CheckGameOver()
{
    // Check if 3 players won, or all are spectators, or board is full.
    bool allSpectatorsOrFull = true;
    int spectatorCount = 0;
    for(size_t i = 0; i < players.size(); i++) {
        if (players[i].isSpectator) spectatorCount++;
    }
    
    // boardFull check again
    bool boardFull = true;
    for (int x = 0; x < GRIDCOLUMN; x++) {
        for (int y = 0; y < GRIDROW; y++) {
            if (grid[x][y] == 0) boardFull = false;
        }
    }

    if (spectatorCount >= 3 || boardFull) {
        std::cout << "FIN DE LA PARTIDA" << std::endl;
        if (victoryOrder.empty()) {
            std::cout << "Empate, nadie consiguio 3 en raya!" << std::endl;
        } else {
            std::cout << "Orden de victoria:" << std::endl;
            for (size_t i = 0; i < victoryOrder.size(); i++) {
                for (auto& p : players) {
                    if (p.id == victoryOrder[i]) {
                        std::cout << i + 1 << " LUGAR: " << p.nickName << " - Puntos: " << p.scoreRanking << std::endl;
                        break;
                    }
                }
            }
        }
        std::cout << "Lobby" << std::endl;
        std::cout << "Enviando conexion a Bootstrap Server" << std::endl;
        isGameOver = true;
        SM.SetNextScene("LobbyScene");
    }
}

void GameManager::AddPieceToGrid(const std::vector<std::vector<short>>& piece, int offsetX, int offsetY)
{
    for (int x = 0; x < piece.size(); x++)
    {
        for (int y = 0; y < piece[x].size(); y++)
        {
            if (piece[x][y] != 0)
            {
                int gx = x + offsetX;
                int gy = y + offsetY;

                if (gx >= 0 && gx < GRIDCOLUMN && gy >= 0 && gy < GRIDROW)
                    grid[gx][gy] = piece[x][y];
            }
        }
    }
}

void GameManager::DrawGrid(sf::RenderWindow& window)
{
    float offsetX = (800.f - (GRIDCOLUMN * cellSize)) / 2.f;
    float offsetY = (600.f - (GRIDROW * cellSize)) / 2.f;

    for (int x = 0; x < GRIDCOLUMN; x++)
    {
        for (int y = 0; y < GRIDROW; y++)
        {
            sf::RectangleShape cell({ (float)cellSize - 2.f, (float)cellSize - 2.f });
            cell.setPosition({ offsetX + (x * cellSize), offsetY + (y * cellSize) });

            // Assign color based on the player ID in grid[x][y]
            sf::Color cellColor = sf::Color(50, 50, 50); // empty block default
            if (grid[x][y] != 0) {
                // Find player matching ID
                for (auto& p : players) {
                    if (p.id == grid[x][y]) {
                        cellColor = p.color;
                        break;
                    }
                }
            }
            
            cell.setFillColor(cellColor);
            window.draw(cell);
        }
    }
}

void GameManager::DrawHUD(sf::RenderWindow& window)
{
    if (players.empty() || players.size() <= currentTurnIndex) return;

    // Turn info
    sf::Text turnText(font);
    turnText.setCharacterSize(20);
    turnText.setPosition({ 20.f, 20.f });
    
    std::string textStr = "Turno de: " + players[currentTurnIndex].nickName + "\n";
    textStr += "Tiempo Restante: " + std::to_string((int)turnTimer) + "s";
    
    turnText.setString(textStr);
    turnText.setFillColor(players[currentTurnIndex].color);
    window.draw(turnText);

    // Scoreboard
    sf::Text scoreText(font);
    scoreText.setCharacterSize(16);
    scoreText.setPosition({ 600.f, 20.f });

    std::string scoreStr = "Ranking:\n";
    for(auto& p : players) {
        scoreStr += p.nickName + ": " + std::to_string(p.scoreRanking);
        if (p.isSpectator) scoreStr += " (ESP)";
        scoreStr += "\n";
    }
    
    scoreText.setString(scoreStr);
    scoreText.setFillColor(sf::Color::White);
    window.draw(scoreText);
}
