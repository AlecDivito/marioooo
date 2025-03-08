#include "Level1State.h"
#include "Utility.h"
#include "Player.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


Level1State::Level1State(StateStack & stack, Context context)
	: LevelState(stack, context, Player::PlayableLevel::Level1)
{
	_player.startTime();
	_player.setLevelStatus(Player::LevelRunning);
	_player.setLevel(Player::PlayableLevel::Level1);

}

void Level1State::draw()
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

bool Level1State::update(sf::Time dt)
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
			_player.setScore(_player.getScore() + _world.getPlayerScore());
			requestStackPop();
			requestStackPush(States::Level2);
		}
	}
	return LevelState::update(dt);
}
