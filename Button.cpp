#include "Button.h"
#include "Utility.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace GUI
{

	Button::Button(const FontHolder& fonts, const TextureHolder& textures)
		: _callback()
		, _sprite(textures.get(Textures::Buttons))
		, _text("", fonts.get(Fonts::Main), 16)
		, _isToggle(false)
	{
		changeTexture(Normal);

		sf::FloatRect bounds = _sprite.getLocalBounds();
		_text.setPosition(bounds.width / 2.f, bounds.height / 2.f);
	}

	void Button::setCallback(Callback callback)
	{
		_callback = std::move(callback);
	}

	void Button::setText(const std::string& text)
	{
		_text.setString(text);
		centerOrigin(_text);
	}

	void Button::setToggle(bool flag)
	{
		_isToggle = flag;
	}

	bool Button::isSelectable() const
	{
		return true;
	}

	void Button::select()
	{
		Component::select();

		changeTexture(Selected);
	}

	void Button::deselect()
	{
		Component::deselect();

		changeTexture(Normal);
	}

	void Button::activate()
	{
		Component::activate();

		// If we are toggle then we should show that the button is pressed and thus "toggled".
		if (_isToggle)
			changeTexture(Pressed);

		if (_callback)
			_callback();

		// If we are not a toggle then deactivate the button since we are just momentarily activated.
		if (!_isToggle)
			deactivate();
	}

	void Button::deactivate()
	{
		Component::deactivate();

		if (_isToggle)
		{
			// Reset texture to right one depending on if we are selected or not.
			if (isSelected())
				changeTexture(Selected);
			else
				changeTexture(Normal);
		}
	}

	void Button::handleEvent(const sf::Event & event)
	{
	}

	sf::FloatRect Button::getGlobalBounds()
	{
		auto rect = _sprite.getGlobalBounds();
		rect.left = getPosition().x;
		rect.top = getPosition().y;
		return rect;
	}

	sf::FloatRect Button::getLocalBounds()
	{
		auto rect = _sprite.getGlobalBounds();
		rect.left = getPosition().x;
		rect.top = getPosition().y;
		return rect;
	}

	void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(_sprite, states);
		target.draw(_text, states);
	}

	void Button::changeTexture(Type buttonType)
	{
		sf::IntRect textureRect(0, 50 * buttonType, 200, 50);
		_sprite.setTextureRect(textureRect);
	}

}
