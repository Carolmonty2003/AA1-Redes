#pragma once
#include <vector>
#include <algorithm>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Renderer.h"

class RenderManager
{
private:
	std::vector<Renderer*> renderers;

public:
	
	void AddRenderer(Renderer* renderer)
	{
		if (renderer) {
			renderers.push_back(renderer);
		}
	}

	
	void RemoveRenderer(Renderer* renderer)
	{
		auto it = std::find(renderers.begin(), renderers.end(), renderer);
		if (it != renderers.end()) {
			renderers.erase(it);
		}
	}

	
	void RenderAll(sf::RenderWindow& window)
	{
		for (Renderer* rnd : renderers) {
			if (rnd) {
				rnd->render(window);
			}
		}
	}
	
	void Clear()
	{
		renderers.clear();
	}
};
