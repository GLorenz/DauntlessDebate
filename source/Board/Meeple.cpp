// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#include "pch.h"
#include "Board/Meeple.h"
#include "Board/AttackHandler.h"
#include "GameManager.h"
#include "GameObjects/Components/RenderComponent.h"
#include "Foundation/ConfigHandler.h"

Meeple::Meeple(GameObject& gameObject, Attack attack, Type type, int health, bool isPlayerOne)
	: GameComponent::GameComponent(gameObject)
	, m_health(health)
	, m_maxHealth(health)
	, m_isPlayerOne(isPlayerOne)
	, m_type(type)
	, m_attack(attack)
	, m_currentSquare()
	, m_randomIdx(0)
{
}

void Meeple::move(std::shared_ptr<SquareComponent> square)
{
	m_currentSquare = square;
	auto& squareBounds = m_currentSquare->getBounds();
	sf::Vector2f newPos(squareBounds.left + squareBounds.width / 2.f, squareBounds.top + squareBounds.height / 2.f);
	m_gameObject.setPosition(newPos);
}


void Meeple::receiveDamage(int damage)
{
	if (m_health - damage <= 0) switchTeams();
	else m_health = m_health - damage;

	m_hpText->setString(std::to_string(m_health));
}


void Meeple::switchTeams()
{	
	m_isPlayerOne = !m_isPlayerOne;
	m_health = m_maxHealth / 2;

	m_teamCircle->setFillColor(getMyTeamColor());
}

Meeple::Type Meeple::getType()
{
	return m_type;
}

bool Meeple::isPlayerOne()
{
	return m_isPlayerOne;
}

std::shared_ptr<SquareComponent> Meeple::getCurrentSquare()
{
	return m_currentSquare;
}

Meeple::Attack Meeple::getAttack()
{
	return m_attack;
}

void Meeple::setRandomIdx(int index)
{
	m_randomIdx = index;
}

int Meeple::getRandomIdx()
{
	return m_randomIdx;
}

void Meeple::scaleToCurrentSquare(sf::FloatRect myBounds)
{
	auto sqBounds = getCurrentSquare()->getBounds();
	m_gameObject.setScale(sqBounds.width / myBounds.width, sqBounds.height / myBounds.height);
}

void Meeple::enableAttacking()
{
	m_disabledRect->setFillColor(sf::Color::Transparent);
}

void Meeple::disableAttacking()
{
	m_disabledRect->setFillColor(sf::Color(0,0,0, 128));
}

void Meeple::setDisabledRect(std::shared_ptr<sf::RectangleShape> rect)
{
	m_disabledRect = rect;
}

void Meeple::setCircle(std::shared_ptr<sf::CircleShape> teamCircle)
{
	m_teamCircle = teamCircle;
}

void Meeple::onSelect(bool isMove)
{
	GameManager::getInstance().getHighlighter().many(&getGameObject());
	/*if(isMove)
		m_teamCircle->setOutlineColor(ConfigHandler::getInstance().getSelectedColorMove());
	else
		m_teamCircle->setOutlineColor(ConfigHandler::getInstance().getSelectedColorDamage());
	*/
}

void Meeple::onDeselect()
{
	GameManager::getInstance().getHighlighter().toDefault();
	//m_teamCircle->setOutlineColor(sf::Color::Transparent);
}

const sf::Color Meeple::getMyTeamColor()
{
	return m_isPlayerOne ? ConfigHandler::getInstance().getP1Color() : ConfigHandler::getInstance().getP2Color();
}

void Meeple::setHpText(std::shared_ptr<sf::Text> text)
{
	m_hpText = text;
	m_hpText->setString(std::to_string(m_maxHealth));
}
