// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"
#include "GameObjects/Components/ARenderableComponent.h"
#include "GameObjects/Components/GameComponent.h"

class LayerComponent : public ARenderableComponent
{
public:
	LayerComponent(GameObject& gameObject, Layer layer = Layer::Default);
	virtual void draw(sf::RenderWindow& window) override;
	void add(std::shared_ptr<sf::Sprite> sprite);

protected:
	std::vector<std::shared_ptr<sf::Sprite>> m_sprites;
};