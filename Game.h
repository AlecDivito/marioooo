#ifndef BOOK_Game_HPP
#define BOOK_Game_HPP

#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "Player.h"
#include "StateStack.h"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>


class Game
{
public:
	Game();
	void run();


private:
	void processInput();
	void update(sf::Time dt);
	void render();

	void updateStatistics(sf::Time dt);
	void registerStates();


private:
	static const sf::Time TimePerFrame;

	sf::RenderWindow _window;
	TextureHolder _textures;
	FontHolder _fonts;
	Player _player;

	StateStack _stateStack;

	sf::Text _statisticsText;
	sf::Time _statisticsUpdateTime;
	std::size_t _statisticsNumFrames;
};

#endif // BOOK_Game_HPP
