// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#include "pch.h"
#include "States/Rounds/AttackRoundState.h"
#include "GameManager.h"
#include "Foundation/InputManager.h"
#include "Foundation/Helpers.h"
#include "Board/AttackHandler.h"
#include "Board/SquareHighlighter.h"
#include "Foundation/ConfigHandler.h"
#include "Foundation/ResourceManager.h"
#include "GameObjects/Components/RenderComponent.h"

AttackRoundState::AttackRoundState()
	: m_playerMeeples()
	, m_curAttackDir(AttackDir::None)
	, m_isPlayerOneActive(true)
	, m_status(Status::Waiting)
	, m_alreadyAttackedMeeps()
{
}

void AttackRoundState::start(bool playerOneActive, finishCallback callBack)
{
	m_finishCallback = callBack;

	/*
	* register for clicks
	* 1. Loop all players meeples
	* 2. For each meeple:
	*	a. save as active meeple (selected)
	*	b. display 4 arrows
	*	c. update highlighted arrow according to mouse position
	*	d. on click
	*/

	m_playerMeeples = GameManager::getInstance().getBoard()->getMeeplesOfPlayer(playerOneActive);

	m_isPlayerOneActive = playerOneActive;
	if (m_uiObj == nullptr)
	{
		m_uiObj = std::make_shared<GameObject>("ui items");
		m_playerUI = std::make_shared<sf::Text>("", *ResourceManager::getInstance().getFont("ptsans_regular"), 30u);
		m_playerUI->setOutlineColor(sf::Color::Black);
		m_playerUI->setOutlineThickness(4.0);

		m_attackUI = std::make_shared<sf::Text>("Attack", *ResourceManager::getInstance().getFont("ptsans_regular"), 30u);
		m_attackUI->setOutlineColor(sf::Color::Black);
		m_attackUI->setOutlineThickness(4.0);
		m_uiObj->addComponent<RenderComponent>(m_playerUI, Layer::UI);
		m_uiObj->addComponent<RenderComponent>(m_attackUI, Layer::UI);
	}
	updateUI();
	m_uiObj->setActive(true);

	m_status = Status::Waiting;
	m_alreadyAttackedMeeps.clear();

	//m_meepIt = m_playerMeeples.begin();
	//(**m_meepIt).onSelect(false);

	startObserveBoardClicks();
}

void AttackRoundState::update()
{
	if (m_status == Status::MeeplePickedUp) {
		// update arrows & highlights for mouse position
		auto mousePos = InputManager::getInstance().getMousePos();
		auto& meepPos = m_selectedMeeple->getCurrentSquare()->getCenterPosition();
		auto mouseMeepVec = sf::Vector2f((float)mousePos.x - meepPos.x, (float)mousePos.y - meepPos.y);
		float angle = Helpers::Rad2Deg * Helpers::getAngle(mouseMeepVec, down); // just use down, trust me
		
		AttackDir newDir = getAttackDirOfAngle(angle);
		if (newDir != m_curAttackDir) {
			m_curAttackDir = newDir;
			m_curAttackOptions = AttackHandler::getAttackOptions(m_selectedMeeple, m_curAttackDir);
			GameManager::getInstance().getSquareHighlighter().damage(m_curAttackOptions);
		}
	}
}

void AttackRoundState::observerUpdate(Event message)
{
	if (m_status == Status::Waiting) {
		if (message.level == Event::MEEPLE_LEVEL) {
			auto meepClicked = std::static_pointer_cast<Meeple>(message.object);

			// ignore this event if clicked meeple is not in my team and not already attacked
			if (meepClicked->isPlayerOne() != m_isPlayerOneActive || Helpers::contains(m_alreadyAttackedMeeps, meepClicked))
				return;

			setSelectedMeeple(meepClicked);
		}
	}
	else if (m_status == Status::MeeplePickedUp) {
		bool clickedOnPossibleTarget = false;
		if (message.level == Event::MEEPLE_LEVEL) {
			auto meepClicked = std::static_pointer_cast<Meeple>(message.object);

			// ignore this event if clicked meeple is not in my team and not already attacked
			if (meepClicked->isPlayerOne() != m_isPlayerOneActive) {
				clickedOnPossibleTarget = false;
			}
			else {
				clickedOnPossibleTarget = true;

				if (Helpers::contains(m_alreadyAttackedMeeps, meepClicked)) {
					// display some error?
					return;
				}

				// if the same meeple is clicked twice, de-select it
				if (meepClicked == m_selectedMeeple) {
					resetSelectedMeeple();
				}
				else {
					setSelectedMeeple(meepClicked);
				}
			}
		}
		// also can attack when clicking on enemy meeple
		if(!clickedOnPossibleTarget) {
			attackWithSelected();
			resetSelectedMeeple();
			
			if (m_alreadyAttackedMeeps.size() >= m_playerMeeples.size()) {
				finish();
			}
		}
	}
}

void AttackRoundState::abort()
{
	GameManager::getInstance().getSquareHighlighter().toDefault();
	GameManager::getInstance().getHighlighter().toDefault();
	stopObserveBoardClicks();

	for (auto meep : m_alreadyAttackedMeeps)
		meep->enableAttacking();
}

void AttackRoundState::finish()
{
	// release everything, then call regular ending of state
	abort();
	m_finishCallback(this);
}

void AttackRoundState::startObserveBoardClicks()
{
	GameManager::getInstance().getBoard()->getClickManager().addObserver(*this);
}

void AttackRoundState::stopObserveBoardClicks()
{
	GameManager::getInstance().getBoard()->getClickManager().removeObserver(*this);
}

void AttackRoundState::resetSelectedMeeple()
{
	m_selectedMeeple->onDeselect();
	m_status = Status::Waiting;
	GameManager::getInstance().getSquareHighlighter().toDefault();
}

void AttackRoundState::setSelectedMeeple(std::shared_ptr<Meeple> meeple)
{
	ResourceManager::getInstance().getSound("button")->play();
	if (m_selectedMeeple != nullptr)
		m_selectedMeeple->onDeselect();

	m_selectedMeeple = meeple;
	m_status = Status::MeeplePickedUp;
	m_selectedMeeple->onSelect(true);
}

void AttackRoundState::attackWithSelected()
{
	ResourceManager::getInstance().getSound("attack")->play();
	AttackHandler::attack(m_selectedMeeple, m_curAttackOptions);
	GameManager::getInstance().getSquareHighlighter().toDefault();
	m_curAttackDir = AttackDir::None;
	m_curAttackOptions.clear();

	m_selectedMeeple->onDeselect();
	m_alreadyAttackedMeeps.push_back(m_selectedMeeple);
	m_selectedMeeple->disableAttacking();
}

AttackDir AttackRoundState::getAttackDirOfAngle(float angle)
{
	AttackDir result;

	if(angle >= 45 && angle <= 135) {
		result = AttackDir::Right;
	}
	else if (angle >= 135 && angle <= 225) {
		result = AttackDir::Down;
	}
	else if (angle >= 225 && angle <= 315) {
		result = AttackDir::Left;
	}
	else {
		result = AttackDir::Top;
	}

	return result;
}

void AttackRoundState::updateUI()
{
	sf::IntRect boardRect = GameManager::getInstance().getBoard()->getBoardRect();
	sf::Vector2u screenSize = GameManager::getInstance().getWindowSize();

	m_playerUI->setString(m_isPlayerOneActive ? "Player One" : "Player Two");
	m_playerUI->setFillColor(m_isPlayerOneActive ? ConfigHandler::getInstance().getP1Color() : ConfigHandler::getInstance().getP2Color());
	m_playerUI->setPosition(sf::Vector2f((boardRect.left + boardRect.width) +
		(screenSize.x - (boardRect.left + boardRect.width)) / 2.f - m_playerUI->getLocalBounds().width / 2.f,
		boardRect.top + screenSize.y / 14.f));

	m_attackUI->setPosition(sf::Vector2f((boardRect.left + boardRect.width) +
		(screenSize.x - (boardRect.left + boardRect.width)) / 2.f - m_attackUI->getLocalBounds().width / 2.f,
		boardRect.top + +screenSize.y / 9.f));
	m_attackUI->setFillColor(m_isPlayerOneActive ? ConfigHandler::getInstance().getP1Color() : ConfigHandler::getInstance().getP2Color());
}
