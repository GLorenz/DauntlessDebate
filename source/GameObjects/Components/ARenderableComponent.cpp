// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#include "pch.h"
#include "GameObjects/Components/ARenderableComponent.h"
#include "GameObjects/RenderManager.h"

ARenderableComponent::ARenderableComponent(GameObject& gameObject, Layer layer)
	: GameComponent::GameComponent(gameObject)
	, m_layer(layer)
{
	RenderManager::getInstance().add(this);
}

ARenderableComponent::~ARenderableComponent() {
	RenderManager::getInstance().remove(this);
}

void ARenderableComponent::setLayer(Layer layer) {
	m_layer = layer;
	// notifying render manager that layers aren't in order anymore
	RenderManager::getInstance().objectsUnordered();
}
Layer ARenderableComponent::getLayer() const {
	return m_layer;
}