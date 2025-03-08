#include "TextNode.h"
#include "Utility.h"

#include <SFML/Graphics/RenderTarget.hpp>


TextNode::TextNode(const FontHolder& fonts, const std::string& text)
{
	_text.setFont(fonts.get(Fonts::Main));
	_text.setCharacterSize(20);
	setString(text);
}

void TextNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_text, states);
}

void TextNode::setString(const std::string& text)
{
	_text.setString(text);
	centerOrigin(_text);
}