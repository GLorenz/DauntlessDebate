// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"
#include "GameObjects/TransformPack.h"

class GameObject;

class GameComponent {
public:
	using SPTR = std::shared_ptr<GameComponent>;
	/// <summary>
	/// When inheriting from this component,
	/// make sure to always pass the GameObject&amp; as the first parameter.
	/// This this necessary for the creation inside the GameObject::addComponent&lt;T&gt;method
	/// </summary>
	GameComponent(GameObject& gameObject);
	virtual GameObject& getGameObject();

	virtual void update();
	virtual void onDraw(sf::RenderWindow& window);

	virtual ~GameComponent();

protected:
	GameObject& m_gameObject;
};