// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#include "pch.h"
#include "GameObjects/GameObject.h"

GameObjectManager::GameObjectManager() : ObjectContainer::ObjectContainer() { }

GameObjectManager& GameObjectManager::getInstance() {
	static GameObjectManager instance;
	return instance;
}

void GameObjectManager::updateAll() {
	// tried many variations
	// this worked the best when deleting objects inside loop
	// not caring at this point that after deletion some component might not get updated THIS frame
	for (size_t i = 0; i < m_objects.size(); ++i) {
		if (i < m_objects.size() && i >= 0 && m_objects[i]->isActive())
			m_objects[i]->update();
	}
}