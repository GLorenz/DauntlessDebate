// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#include "pch.h"
#include "States/Rounds/RoundManager.h"
#include "States/Rounds/AttackRoundState.h"
#include "States/Rounds/MoveRoundState.h"
#include "States/Rounds/PauseRoundState.h"
#include "GameManager.h"
#include "Foundation/ResourceManager.h"
#include "GameObjects/Components/RenderComponent.h"
#include "Foundation/Helpers.h"
#include "Foundation/ConfigHandler.h"

RoundManager::RoundManager()
	: m_state(nullptr)
	, m_player1IsActive(true)
	, m_stateAfterTransition(nullptr)
	, m_inTransition(false)
	, m_curTransScale(0)
	, m_targetScale(1.f)
	, m_transSpeed(3.f)
	, m_patternRend()
	, m_patternRendNext()
{
	createTransitionText();

	m_gui = tgui::Group::create();
	GameManager::getInstance().getGui()->add(m_gui);

	m_nextRandomPatternIdx = std::rand() % ConfigHandler::getInstance().getRandomVecSize();

	m_patternRend.setPosition({ "&.width/6*5 - width/2", "&.height/5*2.2 - height/2" });
	m_patternRend.setSize({ "&.width / 9", "&.width / 9" });

	m_patternRendNext.setPosition({ "&.width/6*5 - width/2", "&.height/5*3.05 - height/2" });
	m_patternRendNext.setSize({ "&.width / 14", "&.width / 14" });

	auto label1 = tgui::Button::create("Current \nRandom \nPattern");
	label1->setRenderer(ResourceManager::getInstance().getTheme()->getRenderer("Button2"));
	label1->setPosition("89.05%", "&.height/5*2.2 - height/2");
	label1->setTextSize(25);
	m_gui->add(label1);

	auto label2 = tgui::Button::create("Next \nRandom \nPattern");
	label2->setRenderer(ResourceManager::getInstance().getTheme()->getRenderer("Button2"));
	label2->setPosition("87.05%", "&.height/5*3.05 - height/2");
	label2->setTextSize(25);
	m_gui->add(label2);

	auto endTurn = tgui::Button::create("Skip");
	endTurn->setRenderer(ResourceManager::getInstance().getTheme()->getRenderer("Button"));
	endTurn->setPosition("&.width/2 - width/2", "95%");
	endTurn->connect("clicked", [=]() {
		ResourceManager::getInstance().getSound("button")->play();
			m_state->abort();
			onStateFinished(&*m_state);
		});
	endTurn->setTextSize(30);
	m_gui->add(endTurn, "endTurnButton");
}

void RoundManager::update()
{
	if (m_inTransition) {

		m_curTransScale = Helpers::lerp(m_curTransScale, m_targetScale, m_transSpeed * Time::delta());
		m_transitionTextObj->setScale(m_curTransScale, m_curTransScale);

		if (abs(m_targetScale - m_curTransScale) < 0.1f) {
			// if scale is at max, scale back to 0
			if (m_targetScale > 0.001f) // targetScale == 0.f
				m_targetScale = 0.f;
			// if already scaled back to 0 
			else
				onTransitionComplete();
		}
	}
	else if (m_state != nullptr) {
		m_state->update();
	}
}

void RoundManager::startNewRound(Action onRoundFinishedCB)
{
	m_currRandomPatternIdx = m_nextRandomPatternIdx;
	m_nextRandomPatternIdx = std::rand() % ConfigHandler::getInstance().getRandomVecSize();
	m_patternRend.setPattern(ConfigHandler::getInstance().getRandomPattern(m_currRandomPatternIdx));
	m_patternRendNext.setPattern(ConfigHandler::getInstance().getRandomPattern(m_nextRandomPatternIdx));

	for (auto meep : GameManager::getInstance().getBoard()->getAllMeeples())
	{
		if (meep->getAttack() == Meeple::Attack::Random)
			meep->setRandomIdx(m_currRandomPatternIdx);
	}

	for (auto widget : m_gui->getWidgets())
	{
		widget->setVisible(true);
		widget->setEnabled(true);
	}

	m_onRoundFinishedCB = onRoundFinishedCB;

	m_player1IsActive = true;
	m_stateAfterTransition = std::make_unique<MoveRoundState>();
	startTransition(true);
}

void RoundManager::onStateFinished(IRoundState* s)
{
	if (typeid(*s) == typeid(MoveRoundState)) {
		m_stateAfterTransition = std::make_unique<AttackRoundState>();
		startTransition(false);
	}
	else if (typeid(*s) == typeid(AttackRoundState)) {
		// if its player 2's turn
		if (m_player1IsActive && GameManager::getInstance().getBoard()->bothHaveMeeples()) {
			m_player1IsActive = false;
			m_stateAfterTransition = std::make_unique<MoveRoundState>();
			startTransition(true);
		}
		else {
			onRoundFinished();
		}
	}
}

void RoundManager::startNextState()
{
	m_state = std::move(m_stateAfterTransition);
	m_state->start(m_player1IsActive, [mgr = this](IRoundState* s) { mgr->onStateFinished(s); });
}

void RoundManager::onRoundFinished()
{
	m_state.reset();
	m_stateAfterTransition.reset();

	m_onRoundFinishedCB();
}

void RoundManager::createTransitionText()
{
	m_transitionTextObj = std::make_unique<GameObject>("RoundTransitionText");
	m_transitionText = std::make_shared<sf::Text>("", *ResourceManager::getInstance().getFont("ptsans_regular"), 80u);
	m_transitionTextObj->addComponent<RenderComponent>(m_transitionText, Layer::UI);
	m_transitionText->setFillColor(sf::Color::White);
	m_transitionText->setOutlineColor(sf::Color::Black);
	m_transitionText->setOutlineThickness(5.0);
	m_transitionTextObj->setActive(false);
}

void RoundManager::onTransitionComplete()
{
	m_inTransition = false;
	startNextState();
	// hide transition object
	m_transitionTextObj->setActive(false);
	m_gui->get("endTurnButton")->setEnabled(true);
}

void RoundManager::startTransition(bool isMovingState)
{
	m_gui->get("endTurnButton")->setEnabled(false);
	std::string transText;
	if (isMovingState)
		if (m_player1IsActive)
			transText = "Player 1 Move!";
		else
			transText = "Player 2 Move!";
	else
		if (m_player1IsActive)
			transText = "Player 1 Attack!";
		else
			transText = "Player 2 Attack!";
	
	auto screenSize = GameManager::getInstance().getWindowSize();
	m_transitionText->setString(transText);
	m_transitionTextObj->setOrigin(m_transitionText->getLocalBounds().width / 2, m_transitionText->getLocalBounds().height / 2);
	m_transitionTextObj->setPosition(screenSize.x / 2.f, screenSize.y / 2.f);
	m_transitionTextObj->setActive(true);
	m_transitionText->setFillColor(m_player1IsActive ? ConfigHandler::getInstance().getP1Color() : ConfigHandler::getInstance().getP2Color());
	
	m_curTransScale = 0.f;
	m_targetScale = 1.f;
	m_transSpeed = 3.f;

	m_transitionTextObj->setScale(m_curTransScale, m_curTransScale);

	m_inTransition = true;
}

RoundManager::~RoundManager()
{
	m_patternRend.remove();
	m_patternRendNext.remove();
	m_gui->removeAllWidgets();
	GameManager::getInstance().getGui()->remove(m_gui);
}