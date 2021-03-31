// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#include "pch.h"
#include "Foundation/InputManager.h"

InputManager& InputManager::getInstance()
{
	static InputManager m_instance;
	return m_instance;
}

bool InputManager::isDown(std::string action, int playerIdx)
{
	if(checkArguments(action, playerIdx) == 'k')
		return m_KeyboardBindings[playerIdx][action].isPressed;
	else 
		return m_MouseBindings[playerIdx][action].isPressed;

}

bool InputManager::isUp(std::string action, int playerIdx)
{
	return !isDown(action, playerIdx);
}

bool InputManager::isPressed(std::string action, int playerIdx)
{
	if(checkArguments(action, playerIdx) == 'k')
		return m_KeyboardBindings[playerIdx][action].isPressed && !m_KeyboardBindings[playerIdx][action].wasPressed;
	else
		return m_MouseBindings[playerIdx][action].isPressed && !m_MouseBindings[playerIdx][action].wasPressed;
}

bool InputManager::isReleased(std::string action, int playerIdx)
{
	if (checkArguments(action, playerIdx) == 'k')
		return !m_KeyboardBindings[playerIdx][action].isPressed && m_KeyboardBindings[playerIdx][action].wasPressed;
	else
		return !m_MouseBindings[playerIdx][action].isPressed && m_MouseBindings[playerIdx][action].wasPressed;

}
char InputManager::checkArguments(std::string& action, int& playerIdx) {
	if (m_KeyboardBindings.find(playerIdx) == m_KeyboardBindings.end() && m_MouseBindings.find(playerIdx) == m_MouseBindings.end())
		throw std::invalid_argument("Player not found");

	if (!isBound(action, playerIdx))
		throw std::invalid_argument("Key not bound to action");

	return (m_KeyboardBindings[playerIdx].find(action) == m_KeyboardBindings[playerIdx].end()) ? 'm' : 'k';
}

void InputManager::bind(std::string action, sf::Keyboard::Key key, int playerIdx)
{
	if (m_KeyboardBindings.find(playerIdx) == m_KeyboardBindings.end())
	{
		std::map<std::string, Keybind> map;
		// move to avoid copying, since map isnt used anyways
		m_KeyboardBindings.insert(std::make_pair(playerIdx, std::move(map)));
	}

	if (isBound(action, playerIdx))
	{
		m_KeyboardBindings[playerIdx][action].boundKey = key;
		return;
	}

	Keybind newBinding = { key, false, false };
	m_KeyboardBindings[playerIdx].insert(std::make_pair(action, std::move(newBinding)));
}


void InputManager::bind(std::string action, sf::Mouse::Button button, int playerIdx)
{
	if (m_MouseBindings.find(playerIdx) == m_MouseBindings.end())
	{
		std::map<std::string, Mousebind> map;
		// move to avoid copying, since map isnt used anyways
		m_MouseBindings.insert(std::make_pair(playerIdx, std::move(map)));
	}

	if (isBound(action, playerIdx))
	{
		m_MouseBindings[playerIdx][action].boundButton = button;
		return;
	}

	Mousebind newBinding = { button, false, false };
	m_MouseBindings[playerIdx].insert(std::make_pair(action, std::move(newBinding)));
}

void InputManager::unbind(std::string action, int playerIdx)
{
	if (checkArguments(action, playerIdx) == 'k')
		m_KeyboardBindings[playerIdx].erase(m_KeyboardBindings[playerIdx].find(action));
	else
		m_MouseBindings[playerIdx].erase(m_MouseBindings[playerIdx].find(action));
}

void InputManager::updateBindings()
{
	for (auto playerIterator = m_KeyboardBindings.begin(); playerIterator != m_KeyboardBindings.end(); ++playerIterator)
	{
		for (auto bindingIterator = playerIterator->second.begin(); bindingIterator != playerIterator->second.end(); ++bindingIterator)
		{
			bindingIterator->second.wasPressed = bindingIterator->second.isPressed;
		}
	}

 	for (auto playerIterator = m_MouseBindings.begin(); playerIterator != m_MouseBindings.end(); ++playerIterator)
	{
		for (auto bindingIterator = playerIterator->second.begin(); bindingIterator != playerIterator->second.end(); ++bindingIterator)
		{
			bindingIterator->second.wasPressed = bindingIterator->second.isPressed;
		}
	}
}

void InputManager::update(sf::Event event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		setKeyPressState(event.key.code, true);
	}

	if (event.type == sf::Event::KeyReleased)
	{
		setKeyPressState(event.key.code, false);
	}

	if (event.type == sf::Event::MouseButtonPressed)
	{
		setMouseButtonPressState(event.key.code, true);
	}

	if (event.type == sf::Event::MouseButtonReleased)
	{
		setMouseButtonPressState(event.key.code, false);
	}
}

void InputManager::setWindow(sf::RenderWindow* window)
{
	m_window = window;
}

sf::Vector2i InputManager::getMousePos()
{
	if (m_window == nullptr) throw "No window registered";

	return sf::Mouse::getPosition(*m_window);
}

void InputManager::setKeyPressState(int eventKey, bool isPressed)
{
	for (auto playerIterator = m_KeyboardBindings.begin(); playerIterator != m_KeyboardBindings.end(); ++playerIterator)
	{
		for (auto bindingIterator = playerIterator->second.begin(); bindingIterator != playerIterator->second.end(); ++bindingIterator)
		{
			if (bindingIterator->second.boundKey == eventKey)
			{
				bindingIterator->second.isPressed = isPressed;
			}
		}
	}
}

void InputManager::setMouseButtonPressState(int eventKey, bool isPressed)
{
	for (auto playerIterator = m_MouseBindings.begin(); playerIterator != m_MouseBindings.end(); ++playerIterator)
	{
		for (auto bindingIterator = playerIterator->second.begin(); bindingIterator != playerIterator->second.end(); ++bindingIterator)
		{
			if (bindingIterator->second.boundButton == eventKey)
			{
				bindingIterator->second.isPressed = isPressed;
			}
		}
	}
}

bool InputManager::isBound(std::string action, int playerIdx)
{
	if(m_KeyboardBindings.find(playerIdx) == m_KeyboardBindings.end() && m_MouseBindings.find(playerIdx) == m_MouseBindings.end())
		throw "Player not found";

	if(m_KeyboardBindings[playerIdx].find(action) != m_KeyboardBindings[playerIdx].end() || m_MouseBindings[playerIdx].find(action) != m_MouseBindings[playerIdx].end())
		return true;

	return false;
}
