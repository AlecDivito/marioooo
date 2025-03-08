#pragma once

#include "State.h"
#include "Container.h"
#include "World.h"
#include "Player.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class LevelState : public State
{
public:
	LevelState(StateStack & stack, Context context, Player::PlayableLevel level);

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event & event);

	bool canStart() const;

protected:
	World _world;
	Player & _player;

private:
	sf::Text _levelText;
	sf::Time _elapsedTime;
	sf::Time _waitTime;
	bool _canStart;
};

