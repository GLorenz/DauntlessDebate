// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#include "pch.h"
#include <iostream>
#include "States/TutorialState.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/Components/CameraComponent.h"
#include "Foundation/ResourceManager.h"
#include "Foundation/InputManager.h"
#include "GameObjects/GameObjectManager.h"
#include "GameManager.h"

TutorialState::TutorialState() :
	m_manager(nullptr),
	m_uiObjHowTo(nullptr),
	m_uiObjCharacters(nullptr),
	m_uiObjRanges(nullptr),
	m_initiated(false),
	m_currStage(TutorialStage::HowTo),
	m_backgroundBounds(sf::FloatRect(0.f, 0.f, (float)GameManager::getInstance().getWindowSize().x, (float)GameManager::getInstance().getWindowSize().y))
{ 
	m_guiGroup = tgui::Group::create();
	GameManager::getInstance().getGui()->add(m_guiGroup);
}

void TutorialState::draw(sf::RenderWindow& window) {
	window.setView(sf::View(m_backgroundBounds));
}

void TutorialState::init(GameStateManager& manager)
{
	InputManager::getInstance().bind("progress", sf::Keyboard::Space, 0);
	m_manager = &manager;

	m_uiObjHowTo = std::make_shared<GameObject>("howTo");
	m_uiObjCharacters = std::make_shared<GameObject>("characters");
	m_uiObjRanges = std::make_shared<GameObject>("ranges");

	auto continueButton = tgui::Button::create("Continue");
	continueButton->setSize({ "10%" , "5%" });
	continueButton->setPosition({ "&.width/2 - width/2", "&.height/6*5 - height/2" });
	continueButton->setTextSize(30);
	continueButton->setRenderer(ResourceManager::getInstance().getTheme()->getRenderer("Button"));
	continueButton->connect("clicked", [=]() {
		ResourceManager::getInstance().getSound("button")->play();
		switchState();
	});
	m_guiGroup->add(continueButton);

	m_uiObjHowTo->addComponent<RenderComponent>(ResourceManager::getInstance().getSprite("tutorial"), Layer::Background);
	m_uiObjCharacters->addComponent<RenderComponent>(ResourceManager::getInstance().getSprite("tutorialChar"), Layer::Background);
	m_uiObjRanges->addComponent<RenderComponent>(ResourceManager::getInstance().getSprite("tutorialDebateStyles"), Layer::Background);

	auto howToSprite = ResourceManager::getInstance().getSprite("tutorial");
	auto charSprite = ResourceManager::getInstance().getSprite("tutorialChar");
	auto rangesSprite = ResourceManager::getInstance().getSprite("tutorialDebateStyles");

	auto screenSize = GameManager::getInstance().getWindowSize();
	m_uiObjHowTo->setScale(screenSize.x / howToSprite->getLocalBounds().width, screenSize.y / howToSprite->getLocalBounds().height);
	m_uiObjCharacters->setScale(screenSize.x / charSprite->getLocalBounds().width, screenSize.y / charSprite->getLocalBounds().height);
	m_uiObjRanges->setScale(screenSize.x / rangesSprite->getLocalBounds().width, screenSize.y / rangesSprite->getLocalBounds().height);

	m_currStage = TutorialStage::HowTo;
	m_uiObjCharacters->setActive(false);
	m_uiObjRanges->setActive(false);
}

void TutorialState::exit()
{
	InputManager::getInstance().unbind("progress", 0);
	m_uiObjHowTo.reset();
	m_uiObjCharacters.reset();
	m_uiObjRanges.reset();

	m_guiGroup->removeAllWidgets();
}

void TutorialState::switchState()
{
	switch (m_currStage)
	{
	case TutorialStage::HowTo:
		m_currStage = TutorialStage::Characters;
		m_uiObjCharacters->setActive(true);
		m_uiObjHowTo->setActive(false);
		break;
	case TutorialStage::Characters:
		m_currStage = TutorialStage::Ranges;
		m_uiObjRanges->setActive(true);
		m_uiObjCharacters->setActive(false);
		break;
	case TutorialStage::Ranges:
		m_manager->setState("playing");
		break;
	}
}

void TutorialState::update() {
}