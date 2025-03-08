#ifndef BOOK_PAUSESTATE_HPP
#define BOOK_PAUSESTATE_HPP

#include "State.h"
#include "Container.h"
#include "Player.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class PauseState : public State
{
public:
	PauseState(StateStack& stack, Context context);

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);


private:
	Player & mPlayer;
	sf::Sprite mBackgroundSprite;
	sf::Text mPausedText;
	sf::Text mScoreText;
	sf::Text mTimeText;
	GUI::Container mGUIContainer;
};

#endif // BOOK_PAUSESTATE_HPP