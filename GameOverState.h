#ifndef BOOK_GAMEOVERSTATE_HPP
#define BOOK_GAMEOVERSTATE_HPP

#include "State.h"
#include "Container.h"
#include "Player.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class GameOverState : public State
{
public:
	GameOverState(StateStack& stack, Context context);

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);


private:
	Player & mPlayer;
	sf::Text mGameOverText;
	sf::Text mTotalTimeText;
	sf::Text mTotalScoreText;
	sf::Time mElapsedTime;
};

#endif // BOOK_GAMEOVERSTATE_HPP