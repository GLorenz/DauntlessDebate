#pragma once
#include "pch.h"
#include "GameObjects/GameObject.h"
#include "Layer.h"
#include "GameObjects/Components/RenderComponent.h"

class Highlighter {
public:
	Highlighter();
	void one(ARenderableComponent* renderable);
	void many(GameObject* go);
	void toDefault();
	void setSize(sf::FloatRect rect);

private:
	std::shared_ptr<GameObject> m_highlightBackgroundObj;
	std::vector<std::tuple<ARenderableComponent*,Layer>> m_curHighlights;
	std::shared_ptr<sf::RectangleShape> m_rect;
};