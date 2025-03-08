#include "Coin.h"
#include "DataTables.h"
#include "Category.h"
#include "CommandQueue.h"
#include "Utility.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <iostream>


namespace
{
	const std::vector<CoinData> Table = initializeCoinData();
}

Coin::Coin(Type type, const TextureHolder& textures)
	: Entity(1)
	, _type(type)
	, _animation(textures.get(Table[type].texture))
	, _lifeTime(Table[type].lifeTime)
{
	_animation.setFrameSize(sf::Vector2i(Table[type].textureRect.width, Table[type].textureRect.height));
	_animation.setStartFrame(sf::Vector2i(Table[type].textureRect.left, Table[type].textureRect.top));
	_animation.setRepeating(true);
	_animation.setNumFrames(Table[type].frames);
	_animation.setDuration(Table[type].frameSpeed);
}

unsigned int Coin::getCategory() const
{
	return Category::Coin;
}

sf::FloatRect Coin::getBoundingRect() const
{
	return getWorldTransform().transformRect(_animation.getGlobalBounds());
}

void Coin::updateCurrent(sf::Time dt, CommandQueue & commands)
{
	_animation.update(dt);
	if (_lifeTime > 0)
	{
		if (_lifeTime > (Table[_type].lifeTime - 500))
		{
			setVelocity(0.0f, -100.0f);
		}
		else
		{
			setVelocity(0, 0);
		}
		_lifeTime -= dt.asMilliseconds();
	}
	else
	{
		destroy();
	}
	Entity::updateCurrent(dt, commands);
}

void Coin::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_animation, states);
}
