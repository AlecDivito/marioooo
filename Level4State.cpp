#include "Level4State.h"
#include "Utility.h"
#include "Player.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


Level4State::Level4State(StateStack & stack, Context context)
	: LevelState(stack, context, Player::PlayableLevel::Level4)
{
	_player.setLevelStatus(Player::LevelRunning);
	_player.setLevel(Player::PlayableLevel::Level4);
}

void Level4State::draw()
{
	if (canStart())
	{
		_world.draw();
	}
	else
	{
		LevelState::draw();
	}
}

bool Level4State::update(sf::Time dt)
{
	// show state for 3 seconds, after start level
	if (canStart())
	{
		_world.update(dt);

		if (!_world.hasAlivePlayer())
		{
			_player.setLevelStatus(Player::LevelFailure);
			requestStackPush(States::GameOver);
		}
		else if (_world.hasPlayerReachedEnd())
		{
			_player.setLevelStatus(Player::LevelSuccess);
			requestStackPush(States::GameOver);
		}
	}
	return LevelState::update(dt);
}
