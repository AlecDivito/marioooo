#pragma once

#include "Entity.h"
#include "Animation.h"
#include "ResourceIdentifiers.h"

#include <SFML/Graphics/Sprite.hpp>

class Coin : public Entity
{
public:
	enum Type
	{
		YellowCoin,
		BlueCoin,
		TypeCount
	};

public:
	Coin(Type type, const TextureHolder & textures);

	unsigned int getCategory() const;
	sf::FloatRect getBoundingRect() const;

protected:
	virtual void updateCurrent(sf::Time dt, CommandQueue & commands);
	virtual void drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const;

private:
	Type _type;
	Animation _animation;
	float _lifeTime;
};

