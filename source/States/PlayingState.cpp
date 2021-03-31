// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#include "pch.h"
#include "States/PlayingState.h"
#include "Foundation/InputManager.h"
#include "GameObjects/Components/RenderComponent.h"
#include "GameObjects/Components/CameraComponent.h"
#include "GameManager.h"
#include "Board/BoardComponent.h"
#include "States/MeeplePlaceState.h"
#include "States/MoveAttackState.h"
#include "Foundation/ResourceManager.h"
#include "Foundation/ConfigHandler.h"

PlayingState::PlayingState()
	: m_manager(nullptr)
	, m_subState(nullptr)
	, m_board(nullptr)
	, m_camera(nullptr)
	, m_rulesObj(nullptr)
	, m_signObj(nullptr)
{ }

void PlayingState::draw(sf::RenderWindow& window) {
	m_camera->onDraw(window);
	m_subState->draw(window);
}

void PlayingState::update() {
	m_subState->update();
}

void PlayingState::init(GameStateManager& manager) 
{
	m_manager = &manager;

	ConfigHandler::getInstance().setup();

	const float bgSizeFactor = 1.333f;
	const float boardSizeFactor = (7.f / 8.f);

	auto screenSize = GameManager::getInstance().getWindowSize();
	auto boardSize = screenSize.y * boardSizeFactor;
	int leftMargin = (screenSize.x - screenSize.y) / 2;
	int topMargin = (int)(screenSize.y - boardSize) / 2;
	sf::IntRect boardRect(leftMargin + topMargin, topMargin, (int)boardSize, (int)boardSize);

	m_board = std::make_shared<GameObject>("Board");
	m_boardComp = m_board->addComponent<BoardComponent>(9, boardRect);
	GameManager::getInstance().setBoard(m_boardComp);
	GameManager::getInstance().getHighlighter().setSize(sf::FloatRect((float)boardRect.left, (float)boardRect.top, (float)boardRect.width, (float)boardRect.height));

	m_camera = std::make_shared<GameObject>("Camera");
	auto camComp = m_camera->addComponent<CameraComponent>(sf::FloatRect(0.f, 0.f, (float)screenSize.x, (float)screenSize.y));

	m_backgroundObj = std::make_shared<GameObject>("menu background");
	auto bgSprite = ResourceManager::getInstance().getSprite("background");
	sf::Vector2f bgSpriteSize = sf::Vector2f(bgSprite->getLocalBounds().width, bgSprite->getLocalBounds().height);

	m_backgroundObj->addComponent<RenderComponent>(bgSprite, Layer::Background);
	m_backgroundObj->setOrigin(bgSpriteSize.x/2.f, bgSpriteSize.y/2.f);
	m_backgroundObj->scale(bgSizeFactor * screenSize.x / bgSpriteSize.x, bgSizeFactor * screenSize.y / bgSpriteSize.y);
	m_backgroundObj->setPosition(screenSize.x/2.f, screenSize.y/2.f);

	// Rules Explanation
	m_rulesObj = std::make_shared<GameObject>("rules");
	auto rulesSprite = ResourceManager::getInstance().getSprite("rules");
	
	m_rulesObj->addComponent<RenderComponent>(rulesSprite, Layer::UI);;
	sf::Vector2f rulesScaleFactor = sf::Vector2f(screenSize.x / 6.f / rulesSprite->getLocalBounds().width, screenSize.y / 6*4.5f / rulesSprite->getLocalBounds().height);
	m_rulesObj->setScale(rulesScaleFactor);
	m_rulesObj->setPosition((boardRect.left)/2.f - rulesSprite->getLocalBounds().width/2.f * rulesScaleFactor.x , (float)boardRect.top);

	// Sign for variable Info
	m_signObj = std::make_shared<GameObject>("sign");
	auto signSprite = ResourceManager::getInstance().getSprite("sign");
	m_signObj->addComponent<RenderComponent>(signSprite, Layer::BackgroundDetails);;
	sf::Vector2f signScaleFactor = sf::Vector2f(screenSize.x / 6 / signSprite->getLocalBounds().width, screenSize.y / 6 / signSprite->getLocalBounds().height);
	m_signObj->setScale(signScaleFactor.x, signScaleFactor.y);
	m_signObj->setPosition((boardRect.left + boardRect.width) + (screenSize.x - (boardRect.left + boardRect.width))/2.f  - signSprite->getLocalBounds().width / 2.f * signScaleFactor.x, (float)boardRect.top);


	m_subState = std::make_unique<MeeplePlaceState>();
	m_subState->init([pstate = this](IPlayingSubState* sub) { pstate->onSubStateFinished(sub); });
} 

void PlayingState::exit() 
{
	m_subState->abort();

	GameManager::getInstance().resetBoard();

	// since the playing state lives
	// forever inside the gamestatemanager
	// we have to release manually
	m_subState.reset();
	m_board.reset();
	m_boardComp.reset();
	m_camera.reset();
	m_rulesObj.reset();
	m_signObj.reset();
	m_backgroundObj.reset();
	m_effectivnessObj.reset();
}

void PlayingState::onSubStateFinished(IPlayingSubState* subState)
{
	if (typeid(*subState) == typeid(MeeplePlaceState)) {
		startState(std::make_unique<MoveAttackState>());
	}
	else if (typeid(*subState) == typeid(MoveAttackState)) {
		int p1Score, p2Score;
		m_boardComp->countMeeples(p1Score, p2Score);
		GameManager::getInstance().setPlayerScores(sf::Vector2i(p1Score, p2Score));
		m_manager->setState("score");
	}
}

void PlayingState::startState(std::unique_ptr<IPlayingSubState> state)
{
	m_subState = std::move(state);
	m_subState->init([pstate = this](IPlayingSubState* sub) { pstate->onSubStateFinished(sub); });
}
