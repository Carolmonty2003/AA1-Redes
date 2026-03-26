#pragma once
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#define GRIDCOLUMN 6
#define GRIDROW 6


class GameManager
{

private:
    const short cellSize = 10;

public:

	std::vector<std::vector<short>> grid;
public:
	GameManager()
	{
		grid.assign(GRIDCOLUMN, std::vector<short>(GRIDROW, 0));
	}

    void AddPieceToGrid(const std::vector<std::vector<short>>& piece, int offsetX = 0, int offsetY = 0)
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

    void DrawGrid(sf::RenderWindow& window)
    {
        for (int x = 0; x < GRIDCOLUMN; x++)
        {
            for (int y = 0; y < GRIDROW; y++)
            {
                sf::RectangleShape cell(sf::Vector2f(cellSize - 2, cellSize - 2));
                cell.setPosition(sf::Vector2f( x * cellSize, y * cellSize));

                cell.setFillColor(grid[x][y] != 0 ? sf::Color::Cyan : sf::Color(50, 50, 50));
                window.draw(cell);
            }
        }
    }




};

