// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"
#include "States/GameStateManager.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/Components/RenderComponent.h"
#include <TGUI/TGUI.hpp>
#include "PatternRenderer.h"
#include "Foundation/ConfigHandler.h"

class MenuState : public GameState {
public:
	MenuState();

	void update() override;
	void draw(sf::RenderWindow& window) override;
	void init(GameStateManager& manager) override;
	void exit() override;

private:
	bool m_initiated;
	std::shared_ptr<GameObject> m_uiObj;
	std::shared_ptr<GameObject> m_backgroundObj;
	std::shared_ptr<GameObject> m_titleObj;
	std::shared_ptr<GameObject> m_logoObj;
	GameStateManager* m_manager;

	std::shared_ptr<tgui::Group> m_guiGroup;

	sf::FloatRect m_backgroundBounds;
};