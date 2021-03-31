// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"
#include "GameObjects/Components/ARenderableComponent.h"
#include "GameObjects/ObjectContainer.h"

class RenderManager : public ObjectContainer<ARenderableComponent*> {
public:
	static RenderManager& getInstance();
	
	void add(ARenderableComponent* object) override;
	void remove(ARenderableComponent* object) override;

	void drawAll(sf::RenderWindow& window);
	void objectsUnordered();

private:
	RenderManager();

	void sort();
	bool m_objectsUnordered;
};