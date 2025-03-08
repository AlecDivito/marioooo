#pragma once

#include "Entity.h"
#include "Command.h"
#include "ResourceIdentifiers.h"
#include "Animation.h"

#include <SFML\Graphics\Sprite.hpp>

class Enemy : public Entity
{
public:
	enum Type
	{
		Turtle,
		Goomba,
		Cactus,
		TypeCount
	};

public:
	Enemy(Type type, const TextureHolder & textures);

	virtual unsigned int getCategory() const;
	virtual sf::FloatRect getBoundingRect() const;

	void getDamage() const;
	void switchDirection();

	void ground(float yDiffrence);

protected:
	virtual void updateCurrent(sf::Time dt, CommandQueue & commands);
	virtual void drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const;

private:
	Type _type;
	sf::Sprite _sprite;
	Animation _animation;
};

