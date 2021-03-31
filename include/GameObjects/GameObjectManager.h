// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"
#include "GameObjects/ObjectContainer.h"

class GameObject;

class GameObjectManager : public ObjectContainer<GameObject*> {
public:
	static GameObjectManager& getInstance();
	void updateAll();

private:
	GameObjectManager();
};