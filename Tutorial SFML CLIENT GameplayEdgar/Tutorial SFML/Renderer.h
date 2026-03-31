#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include "Transform.h"

// Capa 5 - Componentes (Renderer Base)
class Renderer
{
protected:
	Transform* transform; // Puntero al Transform de la Entidad que lo posee

public:
	Renderer(Transform* t) : transform(t) {}
	virtual ~Renderer() = default;

	// Método puramente virtual: Obliga a las clases hijas a definir cómo dibujarse
	virtual void render(sf::RenderWindow& window) = 0;
	
	// Mantenemos Update por si tienes lógica de animación
	virtual void Update(float dt) = 0; 
	
	inline Transform* GetTransform() const { return transform; }
};
