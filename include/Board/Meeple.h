// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"
#include "GameObjects/Components/SquareComponent.h"

class Meeple : public GameComponent {
public:
	enum class Attack { Ranged, Melee, Random};
	enum class Type { A, B, C };

	using Square = std::shared_ptr<SquareComponent>;

	Meeple(GameObject& gameObject, Attack attack, Type type, int health, bool isPlayerOne);
	
	void move(std::shared_ptr<SquareComponent> square);

	void receiveDamage(int damage);

	void switchTeams();
	Meeple::Type getType();
	bool isPlayerOne();
	std::shared_ptr<SquareComponent> getCurrentSquare();
	Meeple::Attack getAttack();
	void setRandomIdx(int index);
	int getRandomIdx();

	void setCircle(std::shared_ptr<sf::CircleShape> teamCircle);
	void onSelect(bool isMove);
	void onDeselect();
	const sf::Color getMyTeamColor();

	void setHpText(std::shared_ptr<sf::Text> text);
	// questionable method but hey
	void scaleToCurrentSquare(sf::FloatRect myBounds);

	void enableAttacking();
	void disableAttacking();
	void setDisabledRect(std::shared_ptr<sf::RectangleShape> rect);

private:
	int m_health;
	int m_maxHealth;
	int m_randomIdx;

	bool m_isPlayerOne;
	Meeple::Attack m_attack;
	Meeple::Type m_type;
	std::shared_ptr<SquareComponent> m_currentSquare;
	std::shared_ptr<sf::CircleShape> m_teamCircle;
	std::shared_ptr<sf::RectangleShape> m_disabledRect;
	std::shared_ptr<sf::Text> m_hpText;
};