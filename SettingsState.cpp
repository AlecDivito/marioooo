#include <SettingsState.h>
#include <Utility.h>
#include <ResourceHolder.h>

#include <SFML/Graphics/RenderWindow.hpp>


SettingsState::SettingsState(StateStack& stack, Context context)
	: State(stack, context)
	, _GUIContainer()
{
	_backgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));

	// Build key binding buttons and labels
	addButtonLabel(Player::MoveLeft, context.window->getSize().x / 2.0f - 300, 400.0f, "Move Left", context);
	addButtonLabel(Player::MoveRight, context.window->getSize().x / 2.0f - 300, 450.0f, "Move Right", context);
	addButtonLabel(Player::MoveUp, context.window->getSize().x / 2.0f - 300, 500.0f, "Jump", context);
	addButtonLabel(Player::Duck, context.window->getSize().x / 2.0f + 50, 400.f, "Duck", context);
	addButtonLabel(Player::Fire, context.window->getSize().x / 2.0f + 50, 450.f, "Fire", context);

	updateLabels();

	auto backButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	backButton->setPosition(40.f, 650.f);
	backButton->setText("Back");
	backButton->setCallback(std::bind(&SettingsState::requestStackPop, this));

	_GUIContainer.pack(backButton);
}

void SettingsState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.draw(_backgroundSprite);
	window.draw(_GUIContainer);
}

bool SettingsState::update(sf::Time)
{
	return true;
}

bool SettingsState::handleEvent(const sf::Event& event)
{
	bool isKeyBinding = false;

	// Iterate through all key binding buttons to see if they are being pressed, waiting for the user to enter a key
	for (std::size_t action = 0; action < Player::ActionCount; ++action)
	{
		if (_bindingButtons[action]->isActive())
		{
			isKeyBinding = true;
			if (event.type == sf::Event::KeyReleased)
			{
				getContext().player->assignKey(static_cast<Player::Action>(action), event.key.code);
				_bindingButtons[action]->deactivate();
			}
			break;
		}
	}

	// If pressed button changed key bindings, update labels; otherwise consider other buttons in container
	if (isKeyBinding)
		updateLabels();
	else
		_GUIContainer.handleEvent(event);

	return false;
}

void SettingsState::updateLabels()
{
	Player& player = *getContext().player;

	for (std::size_t i = 0; i < Player::ActionCount; ++i)
	{
		sf::Keyboard::Key key = player.getAssignedKey(static_cast<Player::Action>(i));
		_bindingLabels[i]->setText(toString(key));
	}
}

void SettingsState::addButtonLabel(Player::Action action, float x, float y, const std::string& text, Context context)
{
	_bindingButtons[action] = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	_bindingButtons[action]->setPosition(x, y);
	_bindingButtons[action]->setText(text);
	_bindingButtons[action]->setToggle(true);

	_bindingLabels[action] = std::make_shared<GUI::Label>("", *context.fonts);
	_bindingLabels[action]->setPosition(x + 225.f, y + 15.f);

	_GUIContainer.pack(_bindingButtons[action]);
	_GUIContainer.pack(_bindingLabels[action]);
}
