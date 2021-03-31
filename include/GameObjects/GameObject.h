// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#pragma once
#include "pch.h"
#include <string>
#include <typeinfo>
#include "GameObjects/Components/GameComponent.h"
#include "GameObjects/GameObjectManager.h"

class GameObject : public sf::Transformable {
public:
	GameObject() = delete;
	GameObject(std::string name, std::string tag = "");
	~GameObject();

	std::string getName();
	bool isActive();
	void setActive(bool active);
	std::string getTag();
	void setTag(std::string tag);

	template <typename T>
	std::shared_ptr<T> getComponent() {
		std::vector<GameComponent::SPTR>::iterator itFound = std::find_if(
			m_components.begin(),
			m_components.end(),
			[](GameComponent::SPTR compPtr) { return typeid(*compPtr) == typeid(T); }
		);

		if (itFound == m_components.end())
			return nullptr;
		else
			return std::dynamic_pointer_cast<T> (*itFound);
	}

	template <typename T>
	std::vector<std::shared_ptr<T>> getAllComponents() {
		std::vector<std::shared_ptr<T>> foundComponents;
		for (auto component : m_components) {
			if (typeid(*component) == typeid(T)) {
				foundComponents.push_back(std::dynamic_pointer_cast<T> (component));
			}
		}
		return foundComponents;
	}

	/// <summary>
	/// Very important: when using this method to construct a component 
	/// you have to omit the GameObject&amp; as the first argument 
	/// since this is automatically inserted into the arguments here!
	/// </summary>
	/// <returns>The shared_ptr to the newly constructed component</returns>
	template <typename T, typename... Args>
	std::shared_ptr<T> addComponent(Args&&... args) {
		std::shared_ptr<T> newPtr = std::make_shared<T>(*this, std::forward<Args>(args)...);
		addComponent(newPtr);
		return newPtr;
	}
	/// <summary>
	/// Prefer the addComponent&lt;T&gt; overload.
	/// If not: make sure the component has 
	/// the correct reference to its gameobject
	/// since it will not be adapted inside this function
	/// </summary>
	GameComponent::SPTR addComponent(GameComponent::SPTR component);
	bool removeComponent(GameComponent::SPTR component);

	void clearComponents();

	void update();
	// called after drawing a renderable on this gameobject
	void onDraw(sf::RenderWindow& window);

private:
	bool m_active;
	std::vector<GameComponent::SPTR> m_components;
	int m_uid;
	std::string m_name;
	std::string m_tag;
};