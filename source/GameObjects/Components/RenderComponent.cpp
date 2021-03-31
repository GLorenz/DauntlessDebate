// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#include "pch.h"
#include "GameObjects/Components/RenderComponent.h"
#include "GameObjects/GameObject.h"

RenderComponent::RenderComponent(GameObject& gameObject, std::shared_ptr<sf::Drawable> renderablePtr, Layer layer)
	: ARenderableComponent::ARenderableComponent(gameObject, layer)
	, m_renderablePtr(renderablePtr)
{
	
}

void RenderComponent::draw(sf::RenderWindow& window) {
	if (m_renderablePtr != nullptr)
		window.draw(*m_renderablePtr, m_gameObject.getTransform());
}

void RenderComponent::setRenderable(std::shared_ptr<sf::Drawable> renderable) {
	m_renderablePtr = renderable;
}