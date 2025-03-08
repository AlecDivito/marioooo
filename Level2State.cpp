#include "Level2State.h"
#include "Utility.h"
#include "Player.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


Level2State::Level2State(StateStack & stack, Context context)
	: LevelState(stack, context, Player::PlayableLevel::Level2)
{
	_player.setLevelStatus(Player::LevelRunning);
	_player.setLevel(Player::PlayableLevel::Level2);
}

void Level2State::draw()
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

bool Level2State::update(sf::Time dt)
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
			requestStackPush(States::Level3);
		}
	}
	return LevelState::update(dt);
}
