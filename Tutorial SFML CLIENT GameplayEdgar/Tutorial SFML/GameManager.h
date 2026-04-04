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
    const short cellSize = 90; // Aumento de tamaño que queda pequeña

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
        //Centrar la cuadricula
        float offsetX = (800.f - (GRIDCOLUMN * cellSize)) / 2.f;
        float offsetY = (600.f - (GRIDROW * cellSize)) / 2.f;

        for (int x = 0; x < GRIDCOLUMN; x++)
        {
            for (int y = 0; y < GRIDROW; y++)
            {
                
                sf::RectangleShape cell({ (float)cellSize - 2.f, (float)cellSize - 2.f });
                cell.setPosition({ offsetX + (x * cellSize), offsetY + (y * cellSize) });

                cell.setFillColor(grid[x][y] != 0 ? sf::Color::Cyan : sf::Color(50, 50, 50));
                window.draw(cell);
            }
        }
    }




};

