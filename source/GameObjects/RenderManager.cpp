#pragma once
// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#include "pch.h"
#include "GameObjects/Components/RenderComponent.h"
#include "Foundation/Helpers.h"
#include "GameObjects/GameObject.h"

RenderManager::RenderManager() : ObjectContainer::ObjectContainer(), m_objectsUnordered(true) { }

RenderManager& RenderManager::getInstance() {
	static RenderManager instance;
	return instance;
}

void RenderManager::add(ARenderableComponent* object) {
	ObjectContainer::add(object);
	objectsUnordered();
}

void RenderManager::remove(ARenderableComponent* object) {
	ObjectContainer::remove(object);
	objectsUnordered();
}

void RenderManager::drawAll(sf::RenderWindow& window) {
	if (m_objectsUnordered)
		sort();

	// tried many variations
	// this worked the best when deleting objects inside loop
	// not caring at this point that after deletion some component might not get updated THIS frame
	for (size_t i = 0; i < m_objects.size(); ++i) {
		if (i < m_objects.size() && i >= 0 && m_objects[i]->getGameObject().isActive()) {
			m_objects[i]->draw(window);
			m_objects[i]->getGameObject().onDraw(window);
		}
	}
}

void RenderManager::objectsUnordered() {
	m_objectsUnordered = true;
}

void RenderManager::sort() {
	std::sort(
		m_objects.begin(),
		m_objects.end(),
		[](ARenderableComponent* o1, ARenderableComponent* o2) {
			// sort by ascending layer (smallest to greatest layer value)
			// TODO sort by Layer and y position
			return Helpers::toUType(o1->getLayer()) < Helpers::toUType(o2->getLayer());
		}
	);
	m_objectsUnordered = false;
}