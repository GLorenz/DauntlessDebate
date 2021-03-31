// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#include "pch.h"
#include "States/MoveAttackState.h"
#include "GameManager.h"
#include "Foundation/ResourceManager.h"
#include "GameObjects/Components/RenderComponent.h"
#include "Foundation/ConfigHandler.h"

void MoveAttackState::init(std::function<void(IPlayingSubState*)> onSubStateFinished)
{
	m_finishedCallback = onSubStateFinished;
	m_maxRounds = ConfigHandler::getInstance().getMaxRounds();

	m_roundObj = std::make_shared<GameObject>("ui items");
	m_roundText = std::make_shared<sf::Text>("", *ResourceManager::getInstance().getFont("ptsans_regular"), 30u);
	m_roundText->setFillColor(sf::Color::White);
	m_roundText->setOutlineColor(sf::Color::Black);
	m_roundText->setOutlineThickness(4.0);
	m_roundObj->addComponent<RenderComponent>(m_roundText, Layer::UI);

	updateUI();

	startNextRound();
}

void MoveAttackState::startNextRound()
{
	m_roundManager.startNewRound([pstate = this]() { pstate->onRoundFinished(); });
}

void MoveAttackState::onRoundFinished()
{
	++m_roundCounter;

	if (GameManager::getInstance().getBoard()->bothHaveMeeples() && m_roundCounter < m_maxRounds) {
		startNextRound();
	}
	else {
		finish();
	}
}

void MoveAttackState::updateUI()
{
	m_roundText->setString("Round " + std::to_string(m_roundCounter + 1) + " / " + std::to_string(m_maxRounds));
	sf::IntRect boardRect = GameManager::getInstance().getBoard()->getBoardRect();
	sf::Vector2u screenSize = GameManager::getInstance().getWindowSize();
	m_roundObj->setPosition((boardRect.left + boardRect.width) + 
		(screenSize.x - (boardRect.left + boardRect.width)) / 2.f - m_roundText->getLocalBounds().width / 2.f, 
		boardRect.top + 30.f);
}

void MoveAttackState::abort()
{
	
}

void MoveAttackState::finish()
{
	// release everything, then call regular ending of state
	abort();
	m_finishedCallback(this);
}

void MoveAttackState::update()
{
	updateUI();
	m_roundManager.update();
}

void MoveAttackState::draw(sf::RenderWindow& window)
{
	// display some info on how to play maybe
}