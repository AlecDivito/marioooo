#include "Level3State.h"
#include "Utility.h"
#include "Player.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


Level3State::Level3State(StateStack & stack, Context context)
	: LevelState(stack, context, Player::PlayableLevel::Level3)
{
	_player.setLevelStatus(Player::LevelRunning);
	_player.setLevel(Player::PlayableLevel::Level3);
}

void Level3State::draw()
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

bool Level3State::update(sf::Time dt)
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
			requestStackPop();
			requestStackPush(States::Level4);
		}
	}
	return LevelState::update(dt);
}
