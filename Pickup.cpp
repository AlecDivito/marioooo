#include "Pickup.h"
#include "DataTables.h"
#include "Category.h"
#include "CommandQueue.h"
#include "Utility.h"
#include "ResourceHolder.h"
#include <iostream>
#include <SFML/Graphics/RenderTarget.hpp>


namespace
{
	const std::vector<PickupData> Table = initializePickupData();
}

Pickup::Pickup(Type type, const TextureHolder& textures)
	: Entity(1)
	, _type(type)
	, _sprite(textures.get(Table[type].texture), Table[type].textureRect)
{
	centerOrigin(_sprite);
}

unsigned int Pickup::getCategory() const
{
	return Category::Pickup;
}

sf::FloatRect Pickup::getBoundingRect() const
{
	return getWorldTransform().transformRect(_sprite.getGlobalBounds());
}

void Pickup::apply(Mario & player) const
{
	Table[_type].action(player);
}

void Pickup::switchDirection()
{
	setVelocity(-getVelocity().x, 0.0f);
}

void Pickup::ground(float yDiffrence)
{
	setPosition(getPosition().x, getPosition().y - yDiffrence);
	setVelocity(getVelocity().x, 0.0f);
}

void Pickup::updateCurrent(sf::Time dt, CommandQueue & commands)
{
	sf::Vector2f gravity(0.0f, 9.81f);
	setVelocity(gravity + getVelocity());

	Entity::updateCurrent(dt, commands);
}

void Pickup::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_sprite, states);
}

