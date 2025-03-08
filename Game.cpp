#include "Game.h"
#include "Utility.h"
#include "State.h"
#include "StateIdentifiers.h"
#include "DataTables.h"
#include "TitleState.h"
#include "MenuState.h"
#include "SettingsState.h"
#include "PauseState.h"
#include "GameOverState.h"
#include "Level1State.h"
#include "Level2State.h"
#include "Level3State.h"
#include "Level4State.h"


const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
	: _window(sf::VideoMode(DataTables::window.width, DataTables::window.height), "Graphics", sf::Style::Close)
	, _textures()
	, _fonts()
	, _player()
	, _stateStack(State::Context(_window, _textures, _fonts, _player))
	, _statisticsText()
	, _statisticsUpdateTime()
	, _statisticsNumFrames(0)
{
	_window.setKeyRepeatEnabled(false);

	_fonts.load(Fonts::Main, "Media/Super Plumber Brothers.ttf");
	_fonts.load(Fonts::Sansation, "Media/Sansation.ttf");

	_textures.load(Textures::TitleScreen, "Media/Textures/TitleScreen.png");
	_textures.load(Textures::Buttons, "Media/Textures/Buttons.png");

	_statisticsText.setFont(_fonts.get(Fonts::Main));
	_statisticsText.setPosition(5.f, 5.f);
	_statisticsText.setCharacterSize(10u);

	registerStates();
	_stateStack.pushState(States::Title);
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (_window.isOpen())
	{
		sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processInput();
			update(TimePerFrame);

			// Check inside this loop, because stack might be empty before update() call
			if (_stateStack.isEmpty())
				_window.close();
		}

		updateStatistics(dt);
		render();
	}
}

void Game::processInput()
{
	sf::Event event;
	while (_window.pollEvent(event))
	{
		_stateStack.handleEvent(event);

		if (event.type == sf::Event::Closed)
			_window.close();
	}
}

void Game::update(sf::Time dt)
{
	_stateStack.update(dt);
}

void Game::render()
{
	_window.clear();
	_stateStack.draw();
	_window.setView(_window.getDefaultView());
	_window.draw(_statisticsText);
	_window.display();
}

void Game::updateStatistics(sf::Time dt)
{
	_statisticsUpdateTime += dt;
	_statisticsNumFrames += 1;
	if (_statisticsUpdateTime >= sf::seconds(1.0f))
	{
		_statisticsText.setString("FPS: " + toString(_statisticsNumFrames));
		_statisticsUpdateTime -= sf::seconds(1.0f);
		_statisticsNumFrames = 0;
	}
}

void Game::registerStates()
{
	_stateStack.registerState<TitleState>(States::Title);
	_stateStack.registerState<MenuState>(States::Menu);
	_stateStack.registerState<SettingsState>(States::Settings);
	_stateStack.registerState<PauseState>(States::Pause);
	_stateStack.registerState<GameOverState>(States::GameOver);
	_stateStack.registerState<Level1State>(States::Level1);
	_stateStack.registerState<Level2State>(States::Level2);
	_stateStack.registerState<Level3State>(States::Level3);
	_stateStack.registerState<Level4State>(States::Level4);
}
