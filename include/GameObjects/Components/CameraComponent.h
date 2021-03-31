// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"
#include "GameObjects/GameObject.h"

class CameraComponent : public GameComponent {
public:
	CameraComponent(GameObject& gameObject);
	CameraComponent(GameObject& gameObject, sf::Window window);
	CameraComponent(GameObject& gameObject, sf::FloatRect rect);
	sf::View& getView();
	void follow(std::shared_ptr<GameObject> target);

	void update() override;
	void onDraw(sf::RenderWindow& window) override;

private:
	sf::View m_view;
	std::shared_ptr<GameObject> m_target;
};