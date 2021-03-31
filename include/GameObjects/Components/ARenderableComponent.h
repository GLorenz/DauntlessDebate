// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"
#include "Foundation/Layer.h"
#include "GameObjects/Components/GameComponent.h"

class ARenderableComponent : public GameComponent {
public:
	ARenderableComponent(GameObject& gameObject, Layer layer);

	ARenderableComponent(const ARenderableComponent&) = delete;
	ARenderableComponent& operator=(const ARenderableComponent&) = delete;
	virtual ~ARenderableComponent();

	virtual void draw(sf::RenderWindow& window) = 0;

	void setLayer(Layer layer);
	Layer getLayer() const;
protected:
	Layer m_layer;
};