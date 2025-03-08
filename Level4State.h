#pragma once

#include "LevelState.h"
#include "World.h"
#include "Player.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class Level4State : public LevelState
{
public:
	Level4State(StateStack & stack, Context context);

	virtual void draw();
	virtual bool update(sf::Time dt);
};

