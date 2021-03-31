// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#include "pch.h"
#include "States/Rounds/MoveRoundState.h"
#include "GameManager.h"
#include "Board/BoardComponent.h"
#include "Foundation/ResourceManager.h"
#include "GameObjects/Components/RenderComponent.h"
#include "Foundation/ConfigHandler.h"

void MoveRoundState::start(bool playerOneActive, finishCallback callBack)
{
	m_finishCallback = callBack;
	m_status = Status::Waiting;
	m_isPlayerOneActive = playerOneActive;

	if (m_uiObj == nullptr)
	{
		m_uiObj = std::make_shared<GameObject>("ui items");
		m_playerUI = std::make_shared<sf::Text>("", *ResourceManager::getInstance().getFont("ptsans_regular"), 30u);
		m_playerUI->setOutlineColor(sf::Color::Black);
		m_playerUI->setOutlineThickness(4.0);

		m_movesUI = std::make_shared<sf::Text>("", *ResourceManager::getInstance().getFont("ptsans_regular"), 30u);
		m_movesUI->setOutlineColor(sf::Color::Black);
		m_movesUI->setOutlineThickness(4.0);
		m_uiObj->addComponent<RenderComponent>(m_playerUI, Layer::UI);
		m_uiObj->addComponent<RenderComponent>(m_movesUI, Layer::UI);
	}

	updateUI();
	m_uiObj->setActive(true);

	resetMoveCount();
	startObserveBoardClicks();
}

void MoveRoundState::moveSelectedMeeple(std::shared_ptr<SquareComponent> square)
{
	m_selectedMeeple->move(square);
	--m_curMoveCount;

	if (m_curMoveCount <= 0) {
		finishState();
	}
}

// this is the main method for this state: it listens to user clicks
void MoveRoundState::observerUpdate(Event message)
{
	/*
	* . check state
	* a. if in waiting state, do not care about square clicks
	* b. if in waiting state, register meeple clicks, fill selected meeple and change to picked up state
	* c. if in picked up state, listen for square clicks and move selected meeple to there
	* d .if in picked up state, listen for meeple clicks and update selected meeple or if its the same meeple, go back to waiting state
	*/
	if (m_status == Status::Waiting) {
		if (message.level == Event::MEEPLE_LEVEL) {
			auto meepClicked = std::static_pointer_cast<Meeple>(message.object);

			// ignore this event if clicked meeple is not in my team
			if (meepClicked->isPlayerOne() != m_isPlayerOneActive)
				return;

			ResourceManager::getInstance().getSound("button")->play();
			setSelectedMeeple(meepClicked);
		}
	}
	else if (m_status == Status::MeeplePickedUp) {
		if (message.level == Event::SQUARE_LEVEL) {
			auto squareClicked = std::static_pointer_cast<SquareComponent>(message.object);

			// ignore this event if meeple can't move to clicked square
			auto foundIt = std::find(m_curMoveOptions.begin(), m_curMoveOptions.end(), squareClicked);
			if (foundIt == m_curMoveOptions.end())
				return;

			// if correct square clicked, move and de-select
			ResourceManager::getInstance().getSound("move")->play();
			resetSelectedMeeple();
			moveSelectedMeeple(squareClicked);
		}
		else if (message.level == Event::MEEPLE_LEVEL) {
			auto meepClicked = std::static_pointer_cast<Meeple>(message.object);

			// ignore this event if clicked meeple is not in my team
			if (meepClicked->isPlayerOne() != m_isPlayerOneActive)
				return;

			// if the same meeple is clicked twice, de-select it
			if (meepClicked == m_selectedMeeple) {
				resetSelectedMeeple();
			}
			else {
				setSelectedMeeple(meepClicked);
			}
		}
	}
}

void MoveRoundState::updateUI()
{
	sf::IntRect boardRect = GameManager::getInstance().getBoard()->getBoardRect();
	sf::Vector2u screenSize = GameManager::getInstance().getWindowSize();

	m_playerUI->setString(m_isPlayerOneActive ? "Player One" : "Player Two");
	m_playerUI->setFillColor(m_isPlayerOneActive ? ConfigHandler::getInstance().getP1Color() : ConfigHandler::getInstance().getP2Color());
	m_playerUI->setPosition(sf::Vector2f((boardRect.left + boardRect.width) + 
		(screenSize.x - (boardRect.left + boardRect.width)) / 2.f - m_playerUI->getLocalBounds().width / 2.f,
		boardRect.top + screenSize.y / 14.f));

	m_movesUI->setString("Moves left: " + std::to_string(m_curMoveCount));
	m_movesUI->setPosition(sf::Vector2f((boardRect.left + boardRect.width) + 
		(screenSize.x - (boardRect.left + boardRect.width)) / 2.f - m_movesUI->getLocalBounds().width / 2.f,
		boardRect.top + +screenSize.y / 9.f));
	m_movesUI->setFillColor(m_isPlayerOneActive ? ConfigHandler::getInstance().getP1Color() : ConfigHandler::getInstance().getP2Color());
}

void MoveRoundState::resetSelectedMeeple()
{
	// TODO: would like to set selected to nullptr but c++ doesnt want me to :(
	m_selectedMeeple->onDeselect();
	m_status = Status::Waiting;
	GameManager::getInstance().getSquareHighlighter().toDefault();
}

void MoveRoundState::setSelectedMeeple(std::shared_ptr<Meeple> meeple)
{
	if (m_selectedMeeple != nullptr)
		m_selectedMeeple->onDeselect();

	m_selectedMeeple = meeple;
	m_status = Status::MeeplePickedUp;
	m_selectedMeeple->onSelect(true);

	updateCurMoveOptions();
	highlightCurMoveOptions();
}

void MoveRoundState::abort()
{
	stopObserveBoardClicks();
	GameManager::getInstance().getSquareHighlighter().toDefault(); // unhighlight all
}

void MoveRoundState::finishState()
{
	// release everything, then call regular ending of state
	abort();
	m_finishCallback(this);
}


void MoveRoundState::updateCurMoveOptions()
{
	m_curMoveOptions = GameManager::getInstance().getBoard()->getFreeNeighbourSquares(m_selectedMeeple->getCurrentSquare());
}

void MoveRoundState::highlightCurMoveOptions()
{
	GameManager::getInstance().getSquareHighlighter().highlight(m_curMoveOptions);
}

void MoveRoundState::startObserveBoardClicks()
{
	GameManager::getInstance().getBoard()->getClickManager().addObserver(*this);
}

void MoveRoundState::stopObserveBoardClicks()
{
	GameManager::getInstance().getBoard()->getClickManager().removeObserver(*this);
}


void MoveRoundState::resetMoveCount()
{
	m_curMoveCount = ConfigHandler::getInstance().getMaxMoves();
}

void MoveRoundState::update()
{
	updateUI();
}