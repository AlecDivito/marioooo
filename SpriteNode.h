#ifndef BOOK_SPRITENODE_HPP
#define BOOK_SPRITENODE_HPP

#include <SceneNode.h>

#include <SFML/Graphics/Sprite.hpp>


class SpriteNode : public SceneNode
{
public:
	explicit SpriteNode(const sf::Texture& texture);
	SpriteNode(const sf::Texture& texture, const sf::IntRect& textureRect);


private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;


private:
	sf::Sprite _sprite;
};

#endif // BOOK_SPRITENODE_HPP
