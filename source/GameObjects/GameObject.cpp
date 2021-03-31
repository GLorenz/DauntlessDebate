// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#include "pch.h"
#include <string>
#include <typeinfo>
#include "GameObjects/GameObject.h"
#include "GameObjects/GameObjectManager.h"

GameObject::GameObject(std::string name, std::string tag)
	//: ObjectContainer::ObjectContainer()
	: m_name(name)
	, m_active(true)
	, m_tag(tag)
	, m_uid(0)
{
	GameObjectManager::getInstance().add(this);
}

GameObject::~GameObject() {
#ifdef _DEBUG
	std::cout << "deleting " << m_name << std::endl;
#endif // _DEBUG
	GameObjectManager::getInstance().remove(this);
}

std::string GameObject::getName() {
	return m_name;
}

bool GameObject::isActive() {
	return m_active;
}

void GameObject::setActive(bool active) {
	m_active = active;
}

std::string GameObject::getTag()
{
	return m_tag;
}

void GameObject::setTag(std::string tag)
{
	m_tag = tag;
}

void GameObject::clearComponents() {
	m_components.clear();
}

GameComponent::SPTR GameObject::addComponent(GameComponent::SPTR component) {
	m_components.push_back(component);
	return component;
}

bool GameObject::removeComponent(GameComponent::SPTR component) {
	auto itFound = std::find(m_components.begin(), m_components.end(), component);
	bool result = false;

	if (itFound != m_components.end()) {
		m_components.erase(itFound);
		result = true;
	}
	return result;
}

void GameObject::update() {
	// tried many variations
	// this worked the best when deleting objects inside loop
	// not caring at this point that after deletion some component might not get updated THIS frame
	for (size_t i = 0; i < m_components.size(); ++i) {
		if(i < m_components.size() && i >= 0)
			m_components[i]->update();
	}
}

void GameObject::onDraw(sf::RenderWindow& window) {
	// tried many variations
	// this worked the best when deleting objects inside loop
	// not caring at this point that after deletion some component might not get updated THIS frame
	for (size_t i = 0; i < m_components.size(); ++i) {
		if (i < m_components.size() && i >= 0)
			m_components[i]->onDraw(window);
	}
}