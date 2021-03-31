// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"
#include "GameObjects/Components/GameComponent.h"
#include "GameObjects/Components/ARenderableComponent.h"

class BoardComponent;

class SquareComponent : public ARenderableComponent {
public:
	enum class Status { Default, MoveHighlight, DamageHighlight };

	SquareComponent(GameObject& gameObject, Layer layer, sf::Vector2i boardCoordinates, sf::IntRect bounds = sf::IntRect{});

	virtual void draw(sf::RenderWindow& window) override;
	void setStatus(Status status);

	const sf::Transform& getTransform();
	const sf::Vector2i& getAbsolutePosition();
	const sf::Vector2i& getBoardCoordinates();
	const sf::IntRect& getBounds();
	const sf::Vector2i getCenterPosition();

	static void setTextureNames(std::string defaultTexName, std::string highlightTexName, std::string damageHighlightTexName);

private:
	Status m_status;
	sf::Vector2i m_absolutePosition;
	sf::Vector2i m_boardCoordinates;
	sf::IntRect m_bounds;
	sf::Transform m_transform;
	std::shared_ptr<sf::Sprite> m_curSprite;

	// share texture names across squares
	static std::string defaultTexName;
	static std::string highlightTexName;
	static std::string damageHighlightTexName;
};