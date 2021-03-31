// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"
#include <iostream>

class InputManager
{
public:
	static InputManager& getInstance();
	
	bool isDown(std::string action, int playerIdx);
	bool isUp(std::string action, int playerIdx);
	bool isPressed(std::string action, int playerIdx);
	bool isReleased(std::string action, int playerIdx);

	void bind(std::string action, sf::Keyboard::Key key, int playerIdx);
	void bind(std::string action, sf::Mouse::Button button, int playerIdx);
	void unbind(std::string action, int playerIdx);
	bool isBound(std::string action, int playerIdx);

	void updateBindings();
	void update(sf::Event event);

	void setWindow(sf::RenderWindow* window);
	sf::Vector2i getMousePos();
	
private:
	struct Keybind
	{
		sf::Keyboard::Key boundKey;
		bool isPressed;
		bool wasPressed;
	};

	struct Mousebind
	{
		sf::Mouse::Button boundButton;
		bool isPressed;
		bool wasPressed;
	};

	InputManager() = default;
	~InputManager() = default;
	InputManager(const InputManager& p) = delete;
	InputManager& operator=(InputManager const&) = delete;

	// return k for Key, m for Mouse
	char checkArguments(std::string& action, int& playerIdx);

	void setKeyPressState(int eventKey, bool isPressed);
	void setMouseButtonPressState(int eventKey, bool isPressed);

	std::map<int, std::map<std::string, Keybind>> m_KeyboardBindings;
	std::map<int, std::map<std::string, Mousebind>> m_MouseBindings;
	sf::RenderWindow* m_window;
};
