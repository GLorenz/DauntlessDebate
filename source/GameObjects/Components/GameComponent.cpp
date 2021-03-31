// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#include "pch.h"
#include "GameObjects/Components/GameComponent.h"

GameComponent::GameComponent(GameObject& gameObject) : m_gameObject(gameObject) { }

GameObject& GameComponent::getGameObject() { return m_gameObject; }

void GameComponent::update() { /* do nothing by default*/ }

void GameComponent::onDraw(sf::RenderWindow&) { /* do nothing by default*/ }

GameComponent::~GameComponent()
{
#ifdef _DEBUG
	std::cout << "deleting component of some go" << std::endl;
#endif // _DEBUG
}
