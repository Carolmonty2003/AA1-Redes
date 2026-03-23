#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#define GRUDCOLUMNROW 6;


class GameManager
{
public:
	static short constexpr  gridRowColumn = 6;
	short fillGrid[gridRowColumn][gridRowColumn];
};

