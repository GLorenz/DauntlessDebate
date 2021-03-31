// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#include "pch.h"
#include "GameObjects/Components/LayerComponent.h"
#include "GameObjects/RenderManager.h"
#include "GameObjects/GameObject.h"

LayerComponent::LayerComponent(GameObject& gameObject, Layer layer)
	: ARenderableComponent::ARenderableComponent(gameObject, layer)
{
}

void LayerComponent::draw(sf::RenderWindow& window)
{
	for (auto sprite : m_sprites)
	{
		window.draw(*sprite /*, m_gameObjectPtr->getTransform()*/);
	}
}

void LayerComponent::add(std::shared_ptr<sf::Sprite> sprite)
{
	m_sprites.push_back(sprite);
}