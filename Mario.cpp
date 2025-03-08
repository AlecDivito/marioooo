#include "Mario.h"
#include "DataTables.h"
#include "Utility.h"
#include "Pickup.h"
#include "CommandQueue.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <cmath>

#include <iostream>

namespace
{
	const std::vector<MarioData> Table = initializeMarioData();
}

Mario::Mario(Type type, const TextureHolder& textures, const FontHolder& fonts)
	: Entity(Table[type].hitpoints)
	, _type(type)
	, _standing(textures.get(Table[type].texture),  Table[type].standTextRect)
	, _jumping(textures.get(Table[type].texture),   Table[type].jumpTextRect)
	, _falling(textures.get(Table[type].texture),   Table[type].fallTextRect)
	, _crouching(textures.get(Table[type].texture), Table[type].crouchTextRect)
	, _kill(textures.get(Table[type].texture),      Table[type].deadTextRect)
	, _running(textures.get(Table[type].texture))
	, _lifeDisplay(nullptr)
	, _missileDisplay(nullptr)
	, _scoreDisplay(nullptr)
	, _invincibilityDisplay(nullptr)
	, _explosion(textures.get(Textures::Explosion))
	, _fireCommand()
	, _fireCountdown(sf::Time::Zero)
	, _shouldReset(false)
	, _isFiring(false)
	, _fireRateLevel(1)
	, _lives(0)
	, _invincible(0)
	, _missileAmmo(0)
	, _score(0)
	, _action(Falling)
	, _drawAction(Falling)
	, _pushing(None)
{
	_running.setFrameSize(sf::Vector2i(Table[type].runningTextRect.width, Table[type].runningTextRect.height));
	_running.setStartFrame(sf::Vector2i(Table[type].runningTextRect.left, Table[type].runningTextRect.top));
	_running.setRepeating(true);
	_running.setNumFrames(Table[type].frameNum);
	_running.setDuration(Table[type].runningDuration);

	_explosion.setFrameSize(sf::Vector2i(256, 256));
	_explosion.setNumFrames(16);
	_explosion.setDuration(sf::seconds(1));

	centerOrigin(_standing);
	centerOrigin(_running);
	centerOrigin(_jumping);
	centerOrigin(_falling);
	centerOrigin(_crouching);
	centerOrigin(_kill);
	centerOrigin(_explosion);

	_fireCommand.category = Category::SceneLayer;
	_fireCommand.action = [this, &textures](SceneNode& node, sf::Time)
	{
		createProjectile(node, Projectile::Missile, 0.f, 0.0f, textures);
	};

	std::unique_ptr<TextNode> lifeDisplay(new TextNode(fonts, ""));
	_lifeDisplay = lifeDisplay.get();
	attachChild(std::move(lifeDisplay));

	std::unique_ptr<TextNode> missileDisplay(new TextNode(fonts, ""));
	missileDisplay->setPosition(0, 70);
	_missileDisplay = missileDisplay.get();
	attachChild(std::move(missileDisplay));

	std::unique_ptr<TextNode> scoreDisplay(new TextNode(fonts, ""));
	scoreDisplay->setPosition(0, 90);
	_scoreDisplay = scoreDisplay.get();
	attachChild(std::move(scoreDisplay));

	std::unique_ptr<TextNode> invincibilityDisplay(new TextNode(fonts, ""));
	invincibilityDisplay->setPosition(0, 110);
	_invincibilityDisplay = invincibilityDisplay.get();
	attachChild(std::move(invincibilityDisplay));
}

void Mario::updateTexts()
{
	// Display life points
	if (isDestroyed())
	{
		_lifeDisplay->setString("");
	}
	else
	{
		_lifeDisplay->setString(toString(getLives()) + " Lives");
		_scoreDisplay->setString(toString(_score) + " SCORE");
	}
	_lifeDisplay->setPosition(0.0f, 50.0f);
	_lifeDisplay->setRotation(-getRotation());

	// display missiles if available
	if (_missileAmmo == 0 || isDestroyed())
	{
		_missileDisplay->setString("M: " + toString(0));
	}
	else
	{
		_missileDisplay->setString("M: " + toString(_missileAmmo));
	}
	
	if (isInvincible())
	{
		int secondsLeft = ((int)_invincible / 1000) + 1;
		_invincibilityDisplay->setString(toString(secondsLeft));
	}
	else
	{
		_invincibilityDisplay->setString("");
	}
}

void Mario::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (isDestroyed())
		return;
	else if (isRunning())
		target.draw(_running, states);
	else if (_drawAction == Standing)
		target.draw(_standing, states);
	else if (getVelocity().y > 0)
		target.draw(_falling, states);
	else if (_drawAction == Jumping)
		target.draw(_jumping, states);
	else if (_drawAction == Crouching)
		target.draw(_crouching, states);
	else if (_drawAction == Dieing)
		target.draw(_kill, states);
	else
		target.draw(_standing, states);
}

void Mario::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	updateTexts();
	// Entity has been destroyed: Possibly drop pickup, mark for removal
	if (isDestroyed())
	{
		return;
	}

	if (isRunning())
	{
		_running.update(dt);
		std::cout << getPosition().x << ", " << getPosition().y << std::endl;

	}

	if (_invincible > 0)
	{
		_invincible -= dt.asMilliseconds();
	}

	// Check if bullets or missiles are fired
	checkProjectileLaunch(dt, commands);

	// apply gravity
	const sf::Vector2f gravity(0.0f, 9.81f);
	setVelocity(gravity + getVelocity());
	if (_action == Standing)
	{
		setVelocity(getVelocity().x, 0.0f);
		_action = Falling;
		_drawAction = Standing;
	}
	else if (_action == Crouching)
	{
		setVelocity(getVelocity().x, 0.0f);
		_action = Falling;
		_drawAction = Crouching;
	}
	else if (getVelocity().y > 0)
	{
		_action = Falling;
	}
	else
	{
		if (_drawAction == Crouching)
		{
			_drawAction = _action;
		}
	}

	Entity::updateCurrent(dt, commands);
}

unsigned int Mario::getCategory() const
{
	return Category::Mario;
}

sf::FloatRect Mario::getBoundingRect() const
{
		if (isRunning())
			return getWorldTransform().transformRect(_running.getGlobalBounds());
		else if (_drawAction == Standing)
			return getWorldTransform().transformRect(_standing.getGlobalBounds());
		else if (_drawAction == Jumping)
			return getWorldTransform().transformRect(_jumping.getGlobalBounds());
		else if (_drawAction == Falling)
			return getWorldTransform().transformRect(_falling.getGlobalBounds());
		else if (_drawAction == Crouching)
			return getWorldTransform().transformRect(_crouching.getGlobalBounds());
		else if (_drawAction == Dieing)
			return getWorldTransform().transformRect(_kill.getGlobalBounds());
		else
			return getWorldTransform().transformRect(_standing.getGlobalBounds());
}

bool Mario::isMarkedForRemoval() const
{
	return isDestroyed() && getLives() == 0;
}

void Mario::remove()
{
	Entity::remove();
	_action = Dieing;
	_drawAction = Dieing;
}

float Mario::getMaxSpeed() const
{
	return Table[_type].speed;
}

void Mario::addMissile()
{
	_missileAmmo++;
}

void Mario::addHealth()
{
	if (getHitpoints() < 2)
	{
		// set mario to big mario
		_type = Mario::MarioPlayerBig;
		repair(1);
		setSprites();
	}
}

void Mario::addScore()
{
	_score++;
}

int Mario::getScore() const
{
	return _score;
}

void Mario::addLife()
{
	_lives += 1;
}

void Mario::removeLife()
{
	_lives -= 1;
}

int Mario::getLives() const
{
	return _lives;
}

void Mario::hit()
{
	if (!isDestroyed())
	{
		// set mario to small
		_type = Mario::MarioPlayerSmall;
		setSprites();
	}
	if (!isInvincible())
	{
		damage(1);
	}
}

bool Mario::shouldResetMario() const
{
	return _shouldReset;
}

void Mario::fire()
{
	// Only ships with fire interval != 0 are able to fire
	if (Table[_type].fireInterval != sf::Time::Zero)
		_isFiring = true;
}

void Mario::jump()
{
	if (_action == Standing)
	{
		_action = Jumping;
		_drawAction = Jumping;
		setVelocity(getVelocity().x, -500.0f);
	}
}

void Mario::crouch()
{
	if (_action == Standing)
	{
		auto standing = getBoundingRect();
		auto crouch = Table[_type].crouchTextRect;
		auto yDiffrence = (standing.height - crouch.height) / 2;
		setPosition(getPosition().x, getPosition().y + yDiffrence);
		_action = Crouching;
		_drawAction = Crouching;
	}
	else if (_drawAction == Crouching)
	{
		_action = Crouching;
		_drawAction = Crouching;
	}
}

void Mario::setFalling(float yDiffrence)
{
	if (_action != Falling)
	{
		_action = Falling;
		setPosition(getPosition().x, getPosition().y + yDiffrence);
		setVelocity(getVelocity().x, 0.0f);
	}
}

void Mario::setPushing(float xDiffrence)
{
	setPosition(getPosition().x - xDiffrence, getPosition().y);
}

void Mario::setGrounded(float groundYPos)
{
	_action = Standing;
	setPosition(getPosition().x, getPosition().y - groundYPos);
}

void Mario::setInvincibility(float time)
{
	_invincible = time;
}

bool Mario::isGrounded() const
{
	return _action == Standing;
}

bool Mario::isCrouched() const
{
	return _drawAction == Crouching;
}

bool Mario::isRunning() const
{
	return getVelocity().x != 0 && _drawAction == Standing;
}

bool Mario::isJumping() const
{
	return _action == Jumping;
}

bool Mario::isFalling() const
{
	return _action == Falling && getVelocity().y > 0;
}

bool Mario::isInvincible() const
{
	return _invincible > 0;
}

bool Mario::isDead() const
{
	return getHitpoints() == 0;
}

void Mario::setSprites()
{
	_standing.setTextureRect(sf::IntRect(Table[_type].standTextRect));
	_jumping.setTextureRect(sf::IntRect(Table[_type].jumpTextRect));
	_falling.setTextureRect(sf::IntRect(Table[_type].fallTextRect));
	_crouching.setTextureRect(sf::IntRect(Table[_type].crouchTextRect));
	_kill.setTextureRect(sf::IntRect(Table[_type].deadTextRect));

	_running.setFrameSize(sf::Vector2i(Table[_type].runningTextRect.width, Table[_type].runningTextRect.height));
	_running.setStartFrame(sf::Vector2i(Table[_type].runningTextRect.left, Table[_type].runningTextRect.top));
	_running.setNumFrames(Table[_type].frameNum);
	_running.setDuration(Table[_type].runningDuration);
	_running.restart();

	centerOrigin(_standing);
	centerOrigin(_jumping);
	centerOrigin(_falling);
	centerOrigin(_crouching);
	centerOrigin(_kill);
	centerOrigin(_explosion);
}

void Mario::checkProjectileLaunch(sf::Time dt, CommandQueue & commands)
{
	// Check for automatic gunfire, allow only in intervals
	if (_isFiring && _fireCountdown <= sf::Time::Zero && _missileAmmo > 0)
	{
		// Interval expired: We can fire a new bullet
		commands.push(_fireCommand);
		_missileAmmo--;
		_fireCountdown += Table[_type].fireInterval / (_fireRateLevel + 1.f);
		_isFiring = false;
	}
	else if (_fireCountdown > sf::Time::Zero)
	{
		// Interval not expired: Decrease it further
		_fireCountdown -= dt;
		_isFiring = false;
	}
}

void Mario::createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const
{
	std::unique_ptr<Projectile> projectile(new Projectile(type, textures));

	sf::Vector2f offset(xOffset * _standing.getGlobalBounds().width, yOffset * _standing.getGlobalBounds().height);
	sf::Vector2f velocity(projectile->getMaxSpeed(), 0);

	float sign = +1.f;
	projectile->setPosition(getWorldPosition() + offset * sign);
	projectile->setVelocity(velocity * sign);
	projectile->setScale(2, 2);
	node.attachChild(std::move(projectile));
}
