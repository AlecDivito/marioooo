#ifndef BOOK_LABEL_HPP
#define BOOK_LABEL_HPP

#include "Component.h"
#include "ResourceIdentifiers.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/Text.hpp>


namespace GUI
{

	class Label : public Component
	{
	public:
		typedef std::shared_ptr<Label> Ptr;


	public:
		Label(const std::string& text, const FontHolder& fonts);

		virtual bool isSelectable() const;
		void setText(const std::string& text);

		virtual void handleEvent(const sf::Event& event);
		virtual sf::FloatRect getGlobalBounds();
		virtual sf::FloatRect getLocalBounds();

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;


	private:
		sf::Text _text;
	};

}

#endif // BOOK_LABEL_HPP
