#include "Entity.h"

#include <cassert>
#include <iostream>


Entity::Entity(int hitpoints)
	: _velocity()
	, _hitpoints(hitpoints)
{
}

void Entity::setVelocity(sf::Vector2f velocity)
{
	_velocity = velocity;
}

void Entity::setVelocity(float vx, float vy)
{
	_velocity.x = vx;
	_velocity.y = vy;
}

sf::Vector2f Entity::getVelocity() const
{
	return _velocity;
}

void Entity::accelerate(sf::Vector2f velocity)
{
	_velocity += velocity;
}

void Entity::accelerate(float vx, float vy)
{
	_velocity.x += vx;
	_velocity.y += vy;
}

int Entity::getHitpoints() const
{
	return _hitpoints;
}

void Entity::repair(int points)
{
	assert(points > 0);

	_hitpoints += points;
}

void Entity::damage(int points)
{
	assert(points > 0);

	_hitpoints -= points;
}

void Entity::destroy()
{
	_hitpoints = 0;
}

void Entity::remove()
{
	destroy();
}

bool Entity::isDestroyed() const
{
	return _hitpoints <= 0;
}

void Entity::updateCurrent(sf::Time dt, CommandQueue&)
{
	move(_velocity * dt.asSeconds());
}
