// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#include "pch.h"
#include <iostream>
#include "States/MenuState.h"
#include "GameObjects/GameObject.h"
#include "GameObjects/Components/CameraComponent.h"
#include "Foundation/ResourceManager.h"
#include "Foundation/InputManager.h"
#include "GameObjects/GameObjectManager.h"
#include "GameManager.h"

MenuState::MenuState() :
	m_manager(nullptr),
	m_uiObj(nullptr),
	m_backgroundObj(nullptr),
	m_backgroundBounds(sf::FloatRect(0.f, 0.f, (float)GameManager::getInstance().getWindowSize().x, (float)GameManager::getInstance().getWindowSize().y)),
	m_initiated(false)
{ 
	m_guiGroup = tgui::Group::create();
	GameManager::getInstance().getGui()->add(m_guiGroup);
}

void MenuState::draw(sf::RenderWindow& window) {
	window.setView(sf::View(m_backgroundBounds));
}

void MenuState::update() {
}

void MenuState::init(GameStateManager& manager) {

	m_manager = &manager;

	auto startButton = tgui::Button::create("Start Game");
	startButton->setSize({ "10%" , "5%" });
	startButton->setPosition({ "&.width/5*2 - width/2", "&.height/3*1.85 - height/2" });
	startButton->setRenderer(ResourceManager::getInstance().getTheme()->getRenderer("Button"));
	startButton->setTextSize(30);
	startButton->connect("clicked", [=]() { 
		ResourceManager::getInstance().getSound("button")->play();
		m_manager->setState("tutorial"); 
	});
	m_guiGroup->add(startButton);

	auto quitButton = tgui::Button::create("Quit Game");
	quitButton->setSize({ "10%" , "5%" });
	quitButton->setPosition({ "&.width/5*3 - width/2", "&.height/3*1.85 - height/2" });
	quitButton->setRenderer(ResourceManager::getInstance().getTheme()->getRenderer("Button"));
	quitButton->setTextSize(30);
	quitButton->connect("clicked", [=]() { 
		ResourceManager::getInstance().getSound("button")->play();
		GameManager::getInstance().getWindow()->close(); 
	});
	m_guiGroup->add(quitButton);

	m_uiObj = std::make_shared<GameObject>("menu items");
	m_backgroundObj = std::make_shared<GameObject>("menu background");
	m_titleObj = std::make_shared<GameObject>("title");
	m_logoObj = std::make_shared<GameObject>("mmt logo");

	m_backgroundObj->addComponent<RenderComponent>(ResourceManager::getInstance().getSprite("backgroundTitle"), Layer::Background);
	m_titleObj->addComponent<RenderComponent>(ResourceManager::getInstance().getSprite("title"), Layer::BackgroundDetails);
	m_logoObj->addComponent<RenderComponent>(ResourceManager::getInstance().getSprite("logo"), Layer::BackgroundDetails);

	sf::FloatRect bgSpriteRect = sf::FloatRect(ResourceManager::getInstance().getSprite("backgroundTitle")->getLocalBounds());
	m_backgroundObj->setScale(m_backgroundBounds.width / bgSpriteRect.width, m_backgroundBounds.height / bgSpriteRect.height);

	sf::FloatRect titleSpriteRect = sf::FloatRect(ResourceManager::getInstance().getSprite("title")->getLocalBounds());
	sf::Vector2f titleScaleFactor = sf::Vector2f(m_backgroundBounds.width / titleSpriteRect.width * 0.45f, m_backgroundBounds.height / titleSpriteRect.height * 0.45f);
	m_titleObj->setScale(titleScaleFactor.x, titleScaleFactor.y);
	m_titleObj->setPosition(m_backgroundBounds.width / 2 - titleSpriteRect.width * titleScaleFactor.x / 2, m_backgroundBounds.height / 3 -  titleSpriteRect.height * titleScaleFactor.y /2);

	auto credits = std::make_shared<sf::Text>("FH Salzburg - MultiMediaTechnology\na game by Lorenz Gonsa & Sabrina Loder", *ResourceManager::getInstance().getFont("ptsans_regular"), 20u);
	credits->setOutlineColor(sf::Color::Black);
	credits->setOutlineThickness(5.0);
	credits->setFillColor(sf::Color::White);
	credits->setPosition(sf::Vector2f(10, m_backgroundBounds.height - credits->getGlobalBounds().height - 20));

	sf::FloatRect logoSpriteRect = sf::FloatRect(ResourceManager::getInstance().getSprite("logo")->getLocalBounds());
	sf::Vector2f logoScaleFactor = sf::Vector2f(m_backgroundBounds.width / logoSpriteRect.width * 0.1f, m_backgroundBounds.height / logoSpriteRect.height * 0.1f);
	m_logoObj->setScale(logoScaleFactor.x, logoScaleFactor.y);
	m_logoObj->setPosition(10, m_backgroundBounds.height - logoSpriteRect.height * logoScaleFactor.y - credits->getLocalBounds().height - 40);

	m_uiObj->addComponent<RenderComponent>(credits, Layer::UI);

	m_initiated = true;
}

void MenuState::exit() {
	m_uiObj.reset();
	m_backgroundObj.reset();
	m_titleObj.reset();
	m_logoObj.reset();

	m_guiGroup->removeAllWidgets();
}