#include "Block.h"
#include "DataTables.h"
#include "ResourceHolder.h"
#include "CommandQueue.h"
#include "Utility.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <iostream>

namespace
{
	const std::vector<BlockData> Table = initializeBlockData();
}

Block::Block(Type type, sf::IntRect size, const TextureHolder & textures, const FontHolder & fonts)
	: Entity(Table[type].hitpoints)
	, _type(type)
	, _sprites()
	, _size(size)
	, _coinBlockAction()
	, _mystryBlockAction()
	, _applyAction(false)
	, _applyDamage(false)
	, _canDestory(false)
{
	for (int i = 0; i < size.width ; i += Table[type].textureRect.width)
	{
		for (int j = 0; j < size.height; j += Table[type].textureRect.height)
		{
			auto sprite = std::make_unique<sf::Sprite>(textures.get(Table[type].texture), Table[type].textureRect);
			sprite->setPosition((float)size.left + i, (float)size.top + j);
			_sprites.push_back(std::move(sprite));
		}
	}

	_mystryBlockAction.category = Category::SceneLayer;
	_mystryBlockAction.action = [this, &textures](SceneNode& node, sf::Time)
	{
		int rnd = randomInt(100);
		if (rnd <= 70) 
			createPickup(node, Pickup::ShellRefill, 20.5f, 0.5f, textures);
		else if (rnd <= 80)
			createPickup(node, Pickup::Invcibility, 20.5f, 0.5f, textures);
		else if (rnd <= 90)
			createPickup(node, Pickup::OneUp, 20.5f, 0.5f, textures);
		else 
			createPickup(node, Pickup::HealthRefill, 20.5f, 0.5f, textures);
		_canDestory = true;
	};

	_coinBlockAction.category = Category::SceneLayer;
	_coinBlockAction.action = [this, &textures](SceneNode & node, sf::Time)
	{
		createCoin(node, Coin::YellowCoin, 5, 0.5f, textures);
		_canDestory = true;
	};

	if (_type != CoinBlock && _type != Mystery)
	{
		_canDestory = true;
	}
}

unsigned int Block::getCategory() const
{
	return Category::Block;
}

sf::FloatRect Block::getBoundingRect() const
{
	return getWorldTransform().transformRect((sf::FloatRect) _size);
}

bool Block::isMarkedForRemoval() const
{
	return isDestroyed() && _canDestory;
}

void Block::apply(Mario & mario)
{
	Table[_type].action(mario, *this);
}

void Block::setHeadBut()
{
	_applyAction = true;
}

void Block::applyDamage()
{
	_applyDamage = true;
}

Block::Type Block::getType() const
{
	return _type;
}

void Block::updateCurrent(sf::Time dt, CommandQueue & commands)
{
	if (_applyAction)
	{
		if (_type == Mystery)
		{
			commands.push(_mystryBlockAction);
		}
		else if (_type == CoinBlock)
		{
			commands.push(_coinBlockAction);
		}
		_applyAction = false;
	}
	if (_applyDamage)
	{
		damage(1);
		_applyDamage = false;
	}
	Entity::updateCurrent(dt, commands );
}

void Block::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto const & s : _sprites)
	{
		target.draw(*s, states);
	}
}

void Block::createPickup(SceneNode & node, Pickup::Type type, float xOffset, float yOffset, const TextureHolder & textures) const
{
	std::unique_ptr<Pickup> pickup(new Pickup(type, textures));

	sf::Vector2f offset(xOffset * _sprite.getGlobalBounds().width, yOffset * _sprite.getGlobalBounds().height);

	pickup->setPosition(_size.left + 17.5f, _size.top + offset.y);
	pickup->setVelocity(25.0f, -300.0f);
	node.attachChild(std::move(pickup));
}

void Block::createCoin(SceneNode & node, Coin::Type type, float xOffset, float yOffset, const TextureHolder & textures) const
{
	std::unique_ptr<Coin> coin(new Coin(type, textures));

	sf::Vector2f offset(xOffset * _sprite.getGlobalBounds().width, yOffset * _sprite.getGlobalBounds().height);

	coin->setPosition(_size.left + xOffset, _size.top + offset.y);
	coin->setScale(2, 2);
	node.attachChild(std::move(coin));
}