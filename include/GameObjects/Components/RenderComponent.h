// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"
#include "GameObjects/Components/GameComponent.h"
#include "GameObjects/Components/ARenderableComponent.h"
#include "GameObjects/RenderManager.h"

class RenderComponent : public ARenderableComponent
{
public:
	RenderComponent(GameObject& gameObject, std::shared_ptr<sf::Drawable> renderablePtr, Layer layer = Layer::Default);

	virtual void draw(sf::RenderWindow& window) override;

	void setRenderable(std::shared_ptr<sf::Drawable> renderable);

protected:
	std::shared_ptr<sf::Drawable> m_renderablePtr;
};