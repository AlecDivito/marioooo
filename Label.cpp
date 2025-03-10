#include "Label.h"
#include "Utility.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


namespace GUI
{

	Label::Label(const std::string& text, const FontHolder& fonts)
		: _text(text, fonts.get(Fonts::Main), 16)
	{
	}

	bool Label::isSelectable() const
	{
		return false;
	}

	void Label::handleEvent(const sf::Event&)
	{
	}

	void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(_text, states);
	}

	void Label::setText(const std::string& text)
	{
		_text.setString(text);
	}

	sf::FloatRect Label::getGlobalBounds()
	{
		return _text.getGlobalBounds();
	}

	sf::FloatRect Label::getLocalBounds()
	{
		return _text.getLocalBounds();
	}

}