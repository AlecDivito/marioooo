#include "Player.h"
#include "CommandQueue.h"
#include "Mario.h"
#include "Foreach.h"

#include <map>
#include <string>
#include <algorithm>

using namespace std::placeholders;


struct MarioMover
{
	MarioMover(float vx, float vy)
		: velocity(vx, vy)
	{
	}

	void operator() (Mario & mario, sf::Time) const
	{
		if (mario.isCrouched())
		{
			return;
		}
		mario.accelerate(velocity * mario.getMaxSpeed());
		if (mario.getVelocity().x > 0)
		{
			mario.setScale(1.0f, 1.0f);
		}
		else if (mario.getVelocity().x < 0)
		{
			mario.setScale(-1.0f, 1.0f);
		}
	}

	sf::Vector2f velocity;
};

Player::Player()
	: _currentLevelStatus(LevelRunning)
	, _level(NoLevel)
	, _score(0)
	, _totalTime(sf::Time::Zero)
{
	// Set initial key bindings
	_keyBinding[sf::Keyboard::Left] = MoveLeft;
	_keyBinding[sf::Keyboard::Right] = MoveRight;
	_keyBinding[sf::Keyboard::Up] = MoveUp;
	_keyBinding[sf::Keyboard::Down] = Duck;
	_keyBinding[sf::Keyboard::Space] = Fire;

	// Set initial action bindings
	initializeActions();

	// Assign all categories to player's Mario
	FOREACH(auto& pair, _actionBinding)
		pair.second.category = Category::Mario;
}

void Player::handleEvent(const sf::Event& event, CommandQueue & commands)
{
	if (event.type == sf::Event::KeyPressed)
	{
		// Check if pressed key appears in key binding, trigger command if so
		auto found = _keyBinding.find(event.key.code);
		if (found != _keyBinding.end() && !isRealtimeAction(found->second))
			commands.push(_actionBinding[found->second]);
	}
}

void Player::handleRealtimeInput(CommandQueue & commands)
{
	// Traverse all assigned keys and check if they are pressed
	FOREACH(auto pair, _keyBinding)
	{
		// If key is pressed, lookup action and trigger corresponding command
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
			commands.push(_actionBinding[pair.second]);
	}
}

void Player::assignKey(Action action, sf::Keyboard::Key key)
{
	// Remove all keys that already map to action
	for (auto itr = _keyBinding.begin(); itr != _keyBinding.end(); )
	{
		if (itr->second == action)
			_keyBinding.erase(itr++);
		else
			++itr;
	}

	// Insert new binding
	_keyBinding[key] = action;
}

sf::Keyboard::Key Player::getAssignedKey(Action action) const
{
	FOREACH(auto pair, _keyBinding)
	{
		if (pair.second == action)
			return pair.first;
	}

	return sf::Keyboard::Unknown;
}

void Player::setLevelStatus(LevelStatus status)
{
	_currentLevelStatus = status;
}

Player::LevelStatus Player::getLevelStatus() const
{
	return _currentLevelStatus;
}

void Player::setLevel(PlayableLevel level)
{
	_level = level;
}

Player::PlayableLevel Player::getLevel() const
{
	return _level;
}

void Player::setScore(int score)
{
	_score = score;
}

int Player::getScore() const
{
	return _score;
}

void Player::setTempScore(int score)
{
	_tempScore = _score + score;
}

int Player::getTempScore()
{
	return _tempScore;
}

void Player::startTime()
{
	_totalTime = sf::Time::Zero;
}

void Player::addTime(sf::Time time)
{
	_totalTime += time;
}

sf::Time Player::getTime() const
{
	return _totalTime;
}

void Player::initializeActions()
{
	_actionBinding[MoveLeft].action = derivedAction<Mario>(MarioMover(-1, 0));
	_actionBinding[MoveRight].action = derivedAction<Mario>(MarioMover(+1, 0));
	_actionBinding[MoveUp].action = derivedAction<Mario>([](Mario& a, sf::Time) { a.jump(); });
	_actionBinding[Duck].action = derivedAction<Mario>([](Mario& a, sf::Time) { a.crouch(); });
	_actionBinding[Fire].action = derivedAction<Mario>([](Mario& a, sf::Time) { a.fire(); });
}

bool Player::isRealtimeAction(Action action)
{
	switch (action)
	{
	case MoveLeft:
	case MoveRight:
	case Duck:
	case MoveUp:
	case Fire:
		return true;

	default:
		return false;
	}
}
