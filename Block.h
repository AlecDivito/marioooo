#pragma once

#include "Entity.h"
#include "Command.h"
#include "ResourceIdentifiers.h"
#include "Pickup.h"
#include "Coin.h"

#include <SFML\Graphics\Sprite.hpp>

#include <vector>
#include <memory>

class Mario;

class Block : public Entity
{
public:
	enum Type
	{
		Indestructible,
		Ground,
		Wall,
		Breakable,
		CoinBlock,
		Mystery,
		SteelBrick,
		TypeCount
	};

public:
	Block(Type type, sf::IntRect size, const TextureHolder & textures, const FontHolder & fonts);

	virtual unsigned int getCategory() const;
	virtual sf::FloatRect getBoundingRect() const;
	virtual bool isMarkedForRemoval() const;

	void apply(Mario & mario);
	void setHeadBut();
	void applyDamage();
	Block::Type getType() const;

protected:
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
	virtual void drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const;
	void createPickup(SceneNode& node, Pickup::Type type, float xOffset, float yOffset, const TextureHolder& textures) const;
	void createCoin(SceneNode& node, Coin::Type type, float xOffset, float yOffset, const TextureHolder& textures) const;

private:
	Type _type;
	sf::Sprite _sprite;
	sf::IntRect _size;
	std::vector<std::unique_ptr<sf::Sprite>> _sprites;

	Command _coinBlockAction;
	Command _mystryBlockAction;
	bool _applyAction;
	bool _applyDamage;
	bool _canDestory;
};

