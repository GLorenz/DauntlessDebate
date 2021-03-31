// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#include "pch.h"
#include "GameObjects/Components/CameraComponent.h"

CameraComponent::CameraComponent(GameObject& gameObject) : GameComponent::GameComponent(gameObject)
	, m_view(sf::View())
	, m_target(nullptr) {}

CameraComponent::CameraComponent(GameObject& gameObject, sf::Window window) : GameComponent::GameComponent(gameObject)
	, m_view(sf::View(sf::Vector2f(0,0), static_cast<sf::Vector2f>(window.getSize())))
	, m_target(nullptr) {}

CameraComponent::CameraComponent(GameObject& gameObject, sf::FloatRect rect) : GameComponent::GameComponent(gameObject)
	, m_view(sf::View(rect))
	, m_target(nullptr) {}

sf::View& CameraComponent::getView() { return m_view; }

void CameraComponent::follow(std::shared_ptr<GameObject> target) {
	if (target == m_target) return;

	m_target = target;
}

void CameraComponent::update() {
	if(m_target != nullptr)
		m_view.setCenter(m_target->getPosition());
}

void CameraComponent::onDraw(sf::RenderWindow& window) {
	window.setView(getView());
}