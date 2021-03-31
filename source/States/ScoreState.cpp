// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#include "pch.h"
#include "States/ScoreState.h"
#include "GameManager.h"
#include "Foundation/ResourceManager.h"
#include "GameObjects/Components/RenderComponent.h"
#include "Foundation/InputManager.h"
#include "Board/Meeple.h"
#include "Foundation/ConfigHandler.h"

ScoreState::ScoreState()
	: m_p1Score(0)
	, m_p2Score(0)
	, m_manager(nullptr)
	, m_buttonName("back2Menu")
{
	
}

void ScoreState::init(GameStateManager& manager)
{
	m_manager = &manager;
	auto scores = GameManager::getInstance().getPlayerScores();
	m_p1Score = scores.x;
	m_p2Score = scores.y;

	m_p1TextObj = createScoreText(true);
	m_p2TextObj = createScoreText(false);

	auto windowSize = GameManager::getInstance().getWindowSize();
	sf::Vector2f screenSize((float)windowSize.x, (float)windowSize.y);
	
	m_p1TextObj->setPosition(screenSize.x * 1 / 4, screenSize.y * 2 / 4);
	m_p2TextObj->setPosition(screenSize.x * 3 / 4, screenSize.y * 2 / 4);

	m_winnerTextObj = createWinnerText();
	m_winnerTextObj->setPosition(screenSize.x * 1 / 2, screenSize.y * 1 / 2);

	m_background = createBackground();

	auto toMenuBtn = tgui::Button::create("Return to menu");
	toMenuBtn->setRenderer(ResourceManager::getInstance().getTheme()->getRenderer("Button"));
	toMenuBtn->setPosition("&.width/2 - width/2", "70%");
	toMenuBtn->connect("clicked", [&]() {
		ResourceManager::getInstance().getSound("button")->play();
		m_manager->setState("menu");
		});
	toMenuBtn->setTextSize(30);
	GameManager::getInstance().getGui()->add(toMenuBtn, m_buttonName);
}

void ScoreState::update()
{
}

void ScoreState::draw(sf::RenderWindow& window)
{
}

void ScoreState::exit()
{
	GameManager::getInstance().getGui()->remove(GameManager::getInstance().getGui()->get(m_buttonName));

	m_p1TextObj.reset();
	m_p2TextObj.reset();
	m_winnerTextObj.reset();
	m_background.reset();
}

std::unique_ptr<GameObject> ScoreState::createScoreText(bool forPlayerOne)
{
	std::string pStr = forPlayerOne ? "1" : "2";
	std::string scStr = std::to_string(forPlayerOne ? m_p1Score : m_p2Score);
	std::string textStr = "Player" + pStr + "\n" + scStr + " debaters";
	
	auto textObj = std::make_unique<GameObject>("P"+pStr+"Score");
	auto textComp = std::make_shared<sf::Text>(textStr, *ResourceManager::getInstance().getFont("ptsans_regular"), 40u);

	textComp->setFillColor(sf::Color::White);
	textComp->setOutlineColor(sf::Color::Black);
	textComp->setOutlineThickness(5.0);

	textObj->setOrigin(textComp->getLocalBounds().width / 2, textComp->getLocalBounds().height / 2);
	textObj->addComponent<RenderComponent>(textComp, Layer::UI);

	return textObj;
}

std::unique_ptr<GameObject> ScoreState::createWinnerText()
{
	auto winnerTextObj = std::make_unique<GameObject>("WinnerText");
	std::string winnerTextStr;
	sf::Color textColor;

	if (m_p1Score > m_p2Score) {
		winnerTextStr = "Player 1\nwins!";
		textColor = ConfigHandler::getInstance().getP1Color();
	}
	else if (m_p2Score > m_p1Score) {
		winnerTextStr = "Player 2\nwins!";
		textColor = ConfigHandler::getInstance().getP2Color();
	}
	else {
		winnerTextStr = "It's a tie!";
		textColor = sf::Color::White;
	}	
	
	auto textComp = std::make_shared<sf::Text>(winnerTextStr, *ResourceManager::getInstance().getFont("ptsans_regular"), 90u);

	textComp->setFillColor(textColor);
	textComp->setOutlineColor(sf::Color::Black);
	textComp->setOutlineThickness(5.0);

	winnerTextObj->setOrigin(textComp->getLocalBounds().width / 2, textComp->getLocalBounds().height / 2);
	winnerTextObj->addComponent<RenderComponent>(textComp, Layer::UI);

	return winnerTextObj;
}

std::unique_ptr<GameObject> ScoreState::createBackground()
{
	auto bg = std::make_unique<GameObject>("menu background");
	auto bgSprite = ResourceManager::getInstance().getSprite("background");
	bg->addComponent<RenderComponent>(bgSprite);
	auto screenSize = GameManager::getInstance().getWindowSize();
	bg->setScale(screenSize.x / bgSprite->getLocalBounds().width, screenSize.y / bgSprite->getLocalBounds().height);
	return bg;
}
