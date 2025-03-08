#ifndef BOOK_PICKUP_HPP
#define BOOK_PICKUP_HPP

#include "Entity.h"
#include "Command.h"
#include "ResourceIdentifiers.h"

#include <SFML/Graphics/Sprite.hpp>


class Mario;

class Pickup : public Entity
{
public:
	enum Type
	{
		OneUp,      
		HealthRefill,
		ShellRefill, 
		Invcibility, 
		TypeCount
	};


public:
	Pickup(Type type, const TextureHolder& textures);

	virtual unsigned int getCategory() const;
	virtual sf::FloatRect getBoundingRect() const;

	void apply(Mario & player) const;

	void switchDirection();

	void ground(float yDiffrence);


protected:
	virtual void updateCurrent(sf::Time dt, CommandQueue & commands);
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;


private:
	Type _type;
	sf::Sprite _sprite;
};

#endif // BOOK_PICKUP_HPP