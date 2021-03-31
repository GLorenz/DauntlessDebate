// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"
#include "States/GameStateManager.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/Components/RenderComponent.h"
#include <TGUI/TGUI.hpp>

class TutorialState : public GameState {
public:
	TutorialState();

	void update() override;
	void draw(sf::RenderWindow& window) override;
	void init(GameStateManager& manager) override;
	void exit() override;

private:

	void switchState();
	enum class TutorialStage{HowTo, Ranges, Characters};

	bool m_initiated;
	std::shared_ptr<GameObject> m_uiObjHowTo;
	std::shared_ptr<GameObject> m_uiObjCharacters;
	std::shared_ptr<GameObject> m_uiObjRanges;
	
	std::shared_ptr<tgui::Group> m_guiGroup;

	GameStateManager* m_manager;
	sf::FloatRect m_backgroundBounds;
	TutorialStage m_currStage;
};