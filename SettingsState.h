#ifndef BOOK_SETTINGSSTATE_HPP
#define BOOK_SETTINGSSTATE_HPP

#include <State.h>
#include <Player.h>
#include <Container.h>
#include <Button.h>
#include <Label.h>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <array>


class SettingsState : public State
{
public:
	SettingsState(StateStack& stack, Context context);

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);


private:
	void updateLabels();
	void addButtonLabel(Player::Action action, float x, float y, const std::string& text, Context context);


private:
	sf::Sprite _backgroundSprite;
	GUI::Container _GUIContainer;
	std::array<GUI::Button::Ptr, Player::ActionCount> _bindingButtons;
	std::array<GUI::Label::Ptr, Player::ActionCount> _bindingLabels;
};

#endif // BOOK_SETTINGSSTATE_HPP
