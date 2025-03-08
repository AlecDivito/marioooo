#ifndef BOOK_Mario_HPP
#define BOOK_Mario_HPP

#include "Entity.h"
#include "Command.h"
#include "ResourceIdentifiers.h"
#include "Projectile.h"
#include "Animation.h"
#include "TextNode.h"

#include <SFML/Graphics/Sprite.hpp>

class CommandQueue;

class Mario : public Entity
{
public:
	enum Type
	{
		MarioPlayerBig,
		MarioPlayerSmall,
		TypeCount
	};

	enum Action
	{
		Jumping,
		Falling, 
		Standing,
		Crouching,
		Dieing,
	};

	enum Pushing
	{
		None,
		Left,
		Right
	};

public:
	Mario(Type type, const TextureHolder& textures, const FontHolder& fonts);

	virtual unsigned int getCategory() const;
	virtual sf::FloatRect getBoundingRect() const;
	virtual void remove();
	virtual bool isMarkedForRemoval() const;
	float getMaxSpeed() const;
	void addMissile();
	void addHealth();
	void addScore();
	int getScore() const;
	void addLife();
	void removeLife();
	int getLives() const;
	void hit();
	bool shouldResetMario() const;

	void fire();
	void jump();
	void crouch();
	void setGrounded(float groundYPos);
	void setFalling(float yDiffrence = 0);
	void setPushing(float xDiffrence);
	void setInvincibility(float time);

	bool isGrounded() const;
	bool isCrouched() const;
	bool isRunning() const;
	bool isJumping() const;
	bool isFalling() const;
	bool isInvincible() const;
	bool isDead() const;

private:
	void updateTexts();
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
	void setSprites();
	void checkProjectileLaunch(sf::Time dt, CommandQueue& commands);
	void createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const;

private:
	Type _type;
	sf::Sprite _standing;
	sf::Sprite _jumping;
	sf::Sprite _falling;
	sf::Sprite _crouching;
	sf::Sprite _kill;
	Animation _running;
	TextNode * _lifeDisplay;
	TextNode * _missileDisplay;
	TextNode * _scoreDisplay;
	TextNode * _invincibilityDisplay;

	Animation _explosion;
	Command	_fireCommand;
	sf::Time _fireCountdown;

	bool _shouldReset;
	bool _isFiring;
	int _fireRateLevel;
	int _lives;
	int _missileAmmo;
	int _score;

	float _invincible;

	Action _action;
	Action _drawAction;
	Pushing _pushing;
};

#endif // BOOK_Mario_HPP
