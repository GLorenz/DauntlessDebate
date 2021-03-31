#include "pch.h"
#include "Foundation/Highlighter.h"
#include "GameManager.h"

Highlighter::Highlighter()
	: m_curHighlights()
	, m_highlightBackgroundObj(std::make_shared<GameObject>("HighlightBackgrd"))
{
	// just default values, size will get adapted
	m_rect = std::make_shared<sf::RectangleShape>(sf::Vector2f(1980.f, 1080.f));
	m_rect->setFillColor(sf::Color(0, 0, 0, 128));
	auto cmop = m_highlightBackgroundObj->addComponent<RenderComponent>(m_rect, Layer::SelectionBackground);
	m_highlightBackgroundObj->setActive(false);
}

void Highlighter::one(ARenderableComponent* renderable)
{
	if (renderable == nullptr)
		return;

	m_highlightBackgroundObj->setActive(true);
	m_curHighlights.emplace_back(std::make_tuple(renderable, renderable->getLayer()));

	if(renderable->getLayer() == Layer::Foreground)
		renderable->setLayer(Layer::SelectionForeground);
	else if (renderable->getLayer() == Layer::UI)
		renderable->setLayer(Layer::SelectionUI);
	else
		renderable->setLayer(Layer::SelectionObject);
}

void Highlighter::many(GameObject* go)
{
	if (go == nullptr)
		return;

	auto found = go->getAllComponents<RenderComponent>();
	
	for (auto r : found) {
		one(&*r);
	}
}

void Highlighter::toDefault()
{
	// reset highlights to original layers
	for (auto h : m_curHighlights) {
		std::get<0>(h)->setLayer(std::get<1>(h));
	}

	m_curHighlights.clear();
	m_highlightBackgroundObj->setActive(false);
}

void Highlighter::setSize(sf::FloatRect rect)
{
	m_rect->setSize(sf::Vector2f(rect.width, rect.height));
	m_highlightBackgroundObj->setPosition(rect.left, rect.top);
}