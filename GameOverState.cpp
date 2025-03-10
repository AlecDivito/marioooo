#include "GameOverState.h"
#include "Utility.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


GameOverState::GameOverState(StateStack& stack, Context context)
	: State(stack, context)
	, mPlayer(*context.player)
	, mGameOverText()
	, mTotalTimeText()
	, mTotalScoreText()
	, mElapsedTime(sf::Time::Zero)
{
	sf::Font& font = context.fonts->get(Fonts::Main);
	sf::Vector2f windowSize(context.window->getSize());

	mGameOverText.setFont(font);
	if (context.player->getLevelStatus() == Player::LevelFailure)
	{
		mGameOverText.setString("YOU LOST CAUSE YOUR BAD!");
	}
	else
	{
		mGameOverText.setString("GAME COMPLETED!");
	}

	mGameOverText.setCharacterSize(70);
	centerOrigin(mGameOverText);
	mGameOverText.setPosition(0.5f * windowSize.x, 0.2f * windowSize.y);

	mTotalScoreText.setFont(font);
	mTotalScoreText.setString("Final Score: " + toString(mPlayer.getScore()));
	mTotalScoreText.setCharacterSize(70);
	centerOrigin(mTotalScoreText);
	mTotalScoreText.setPosition(0.5f * windowSize.x, 0.3f * windowSize.y);


	mTotalTimeText.setFont(font);
	mTotalTimeText.setString("Total Time: " + toString(mPlayer.getTime().asSeconds()));
	mTotalTimeText.setCharacterSize(70);
	centerOrigin(mTotalTimeText);
	mTotalTimeText.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);

}

void GameOverState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	// Create dark, semitransparent background
	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);
	window.draw(mGameOverText);
	window.draw(mTotalTimeText);
	window.draw(mTotalScoreText);
}

bool GameOverState::update(sf::Time dt)
{
	// Show state for 3 seconds, after return to menu
	mElapsedTime += dt;
	if (mElapsedTime > sf::seconds(3))
	{
		requestStateClear();
		requestStackPush(States::Menu);
	}
	return false;
}

bool GameOverState::handleEvent(const sf::Event&)
{
	return false;
}