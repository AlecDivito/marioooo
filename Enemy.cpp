#include "Enemy.h"
#include "DataTables.h"
#include "Category.h"
#include "CommandQueue.h"
#include "ResourceHolder.h"
#include "Utility.h"

#include <SFML\Graphics\RenderTarget.hpp>

namespace
{
	const std::vector<EnemyData> Table = initializeEnemyData();
}

Enemy::Enemy(Type type, const TextureHolder& textures)
	: Entity(Table[type].hitpoints)
	, _type(type)
	, _sprite(textures.get(Table[type].texture), Table[type].textureRect)
	, _animation(textures.get(Table[type].texture))
{
	_animation.setFrameSize(sf::Vector2i(Table[type].startingFrame.width, Table[type].startingFrame.height));
	_animation.setStartFrame(sf::Vector2i(Table[type].startingFrame.left, Table[type].startingFrame.top));
	_animation.setRepeating(true);
	_animation.setNumFrames(Table[type].frames);
	_animation.setDuration(Table[type].frameSpeed);
	centerOrigin(_sprite);
	centerOrigin(_animation);
	setVelocity(Table[type].speed, 0.0f);
}

unsigned int Enemy::getCategory() const
{
	return Category::Enemy;
}

sf::FloatRect Enemy::getBoundingRect() const
{
	return getWorldTransform().transformRect(_sprite.getGlobalBounds());
}

void Enemy::getDamage() const
{
	Table[_type].damage;
}

void Enemy::switchDirection()
{
	if (getScale().x > 0)
	{
		setScale(getScale().x, getScale().y);
	}
	else
	{
		setScale(-1.0f * getScale().x, getScale().y);
	}
	setVelocity(-getVelocity().x, 0.0f);
}

void Enemy::ground(float yDiffrence)
{
	setPosition(getPosition().x, getPosition().y - yDiffrence);
	setVelocity(getVelocity().x, 0.0f);
}

void Enemy::updateCurrent(sf::Time dt, CommandQueue & commands)
{
	_animation.update(dt);
	const sf::Vector2f gravity(0.0f, 9.81f);
	setVelocity(gravity + getVelocity());
	Entity::updateCurrent(dt, commands);
}

void Enemy::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_animation, states);
}
