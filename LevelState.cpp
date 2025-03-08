#include "LevelState.h"
#include "Utility.h"
#include "Player.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

LevelState::LevelState(StateStack & stack, Context context, Player::PlayableLevel level)
	: State(stack, context)
	, _world(*context.window, *context.fonts, level)
	, _player(*context.player)
	, _canStart(false)
	, _waitTime(sf::seconds(3))
{
	sf::Font & font = context.fonts->get(Fonts::Main);
	sf::Vector2f windowSize(context.window->getSize());

	_levelText.setFont(font);
	_levelText.setString("Level 1-" + toString(level));
	_levelText.setCharacterSize(70);
	centerOrigin(_levelText);
	_levelText.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);
}

void LevelState::draw()
{
	if (_elapsedTime < _waitTime)
	{
		sf::RenderWindow& window = *getContext().window;
		window.setView(window.getDefaultView());

		// Create dark, semitransparent background
		sf::RectangleShape backgroundShape;
		backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
		backgroundShape.setSize(window.getView().getSize());

		window.draw(backgroundShape);
		window.draw(_levelText);
	}
}

bool LevelState::update(sf::Time dt)
{
	if (_elapsedTime < _waitTime)
	{
		_elapsedTime += dt;
	}
	else
	{
		_canStart = true;
		_player.addTime(dt);
		CommandQueue & commands = _world.getCommandQueue();
		_player.handleRealtimeInput(commands);
	}
	return true;
}

bool LevelState::handleEvent(const sf::Event & event)
{
	if (_elapsedTime > _waitTime)
	{
		// Game input handling
		CommandQueue & commands = _world.getCommandQueue();
		_player.handleEvent(event, commands);

		// Escape pressed, trigger the pause screen
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		{
			requestStackPush(States::Pause);
			_player.setTempScore(_player.getScore() + _world.getPlayerScore());
		}
	}

	return true;
}

bool LevelState::canStart() const
{
	return _canStart;
}
