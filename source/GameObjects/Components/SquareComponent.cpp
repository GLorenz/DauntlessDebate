// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#include "pch.h"
#include "GameObjects/Components/SquareComponent.h"
#include "Foundation/ResourceManager.h"
#include "GameObjects/GameObject.h"
#include "Board/BoardComponent.h"
#include "GameManager.h"

std::string SquareComponent::defaultTexName = "tile";
std::string SquareComponent::highlightTexName = "tile_highlight";
std::string SquareComponent::damageHighlightTexName = "tile_damage";

SquareComponent::SquareComponent(GameObject& gameObject, Layer layer, sf::Vector2i boardCoordinates, sf::IntRect bounds)
	: ARenderableComponent::ARenderableComponent(gameObject, layer)
	, m_transform{}
	, m_absolutePosition(sf::Vector2i(bounds.left, bounds.top))
	, m_bounds(bounds)
	, m_boardCoordinates(boardCoordinates)
{
	m_transform.translate(sf::Vector2f(m_absolutePosition));
	setStatus(Status::Default);
	auto spriteBounds = m_curSprite->getLocalBounds();
	m_transform.scale((float)bounds.width / spriteBounds.width, (float)bounds.height / spriteBounds.height);
}

void SquareComponent::setStatus(Status status)
{
	std::string texName = defaultTexName;

	switch (status) {
		case Status::MoveHighlight:
			texName = highlightTexName;
			break;
		case Status::DamageHighlight:
			texName = damageHighlightTexName;
			break;
		case Status::Default:
			texName = defaultTexName;
			break;
	}

	m_curSprite = ResourceManager::getInstance().getSprite(texName);
	m_status = status;
}

const sf::Transform& SquareComponent::getTransform()
{
	return m_transform;
}

const sf::Vector2i& SquareComponent::getAbsolutePosition()
{
	return m_absolutePosition;
}

const sf::Vector2i& SquareComponent::getBoardCoordinates()
{
	return m_boardCoordinates;
}

const sf::IntRect& SquareComponent::getBounds()
{
	return m_bounds;
}

const sf::Vector2i SquareComponent::getCenterPosition()
{
	return sf::Vector2i(m_bounds.left + m_bounds.width / 2, m_bounds.top + m_bounds.height / 2);
}

void SquareComponent::draw(sf::RenderWindow& window)
{
	window.draw(*m_curSprite, m_transform);
}

void SquareComponent::setTextureNames(std::string defaultTexName, std::string highlightTexName, std::string damageHighlightTexName)
{
	SquareComponent::defaultTexName = defaultTexName;
	SquareComponent::highlightTexName = highlightTexName;
	SquareComponent::damageHighlightTexName = damageHighlightTexName;
}