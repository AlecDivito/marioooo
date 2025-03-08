#include "Component.h"

namespace GUI
{

	Component::Component()
		: _isSelected(false)
		, _isActive(false)
	{
	}

	Component::~Component()
	{
	}

	bool Component::isSelected() const
	{
		return _isSelected;
	}

	void Component::select()
	{
		_isSelected = true;
	}

	void Component::deselect()
	{
		_isSelected = false;
	}

	bool Component::isActive() const
	{
		return _isActive;
	}

	void Component::activate()
	{
		_isActive = true;
	}

	void Component::deactivate()
	{
		_isActive = false;
	}

}