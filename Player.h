#ifndef BOOK_PLAYER_HPP
#define BOOK_PLAYER_HPP

#include "Command.h"

#include <SFML/Window/Event.hpp>

#include <map>
#include <string>


class CommandQueue;

class Player
{
public:
	enum Action
	{
		MoveLeft,
		MoveRight,
		MoveUp,
		Duck,
		Fire,
		ActionCount
	};

	enum LevelStatus
	{
		LevelRunning,
		LevelSuccess,
		LevelFailure
	};

	enum PlayableLevel
	{
		NoLevel,
		Level1,
		Level2,
		Level3,
		Level4,
	};


public:
	Player();

	void handleEvent(const sf::Event& event, CommandQueue& commands);
	void handleRealtimeInput(CommandQueue & commands);

	void assignKey(Action action, sf::Keyboard::Key key);
	sf::Keyboard::Key getAssignedKey(Action action) const;

	void setLevelStatus(LevelStatus status);
	LevelStatus getLevelStatus() const;

	void setLevel(PlayableLevel level);
	PlayableLevel getLevel() const;

	void setScore(int score);
	int getScore() const;

	void setTempScore(int score);
	int getTempScore();

	void startTime();
	void addTime(sf::Time time);
	sf::Time getTime() const;

private:
	void initializeActions();
	static bool isRealtimeAction(Action action);


private:
	std::map<sf::Keyboard::Key, Action> _keyBinding;
	std::map<Action, Command> _actionBinding;
	LevelStatus _currentLevelStatus;
	PlayableLevel _level;
	sf::Time _totalTime;
	int _score;
	int _tempScore;
};

#endif // BOOK_PLAYER_HPP
