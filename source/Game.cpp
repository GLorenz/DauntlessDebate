// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#include "pch.h"
#include <assert.h>
#include "States/GameStateManager.h"
#include "States/MenuState.h"
#include "States/PlayingState.h"
#include "States/ScoreState.h"
#include "States/TutorialState.h"
#include "GameObjects/GameObject.h"
#include "Foundation/InputManager.h"
#include "Foundation/ResourceManager.h"
#include "GameObjects/GameObjectManager.h"
#include "GameManager.h"
#include <TGUI/TGUI.hpp>

class Game
{
public:
	void Run()
	{
		if (!init())
			return;

		while (m_window->isOpen())
		{
			Time::onNewFrame();

			sf::Event event;
			InputManager::getInstance().setWindow(&*m_window);
			while (m_window->pollEvent(event))
			{
				if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Escape))
					m_window->close();

				GameManager::getInstance().getGui()->handleEvent(event);
				InputManager::getInstance().update(event);
			}
			
			update();
			draw();
		}

		shutdown();
	}

private:

	bool init()
	{		
		//auto windowType = sf::Style::Titlebar | sf::Style::Close;
		//auto windowType = sf::Style::Fullscreen;
		m_window = std::make_shared<sf::RenderWindow>();
		auto windowType = sf::Style::None;
		m_window->create(sf::VideoMode::getDesktopMode(), "Dauntless Debate", windowType);
		m_window->setPosition(sf::Vector2i(0, 0));
		
		//TODO Always Check that the right one here is not commented out
		//sf::Vector2u screenSize(1920, 1080);
		sf::Vector2u screenSize(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);
		GameManager::getInstance().setWindowSize(screenSize);
		GameManager::getInstance().setGui(std::make_shared<tgui::Gui>(*m_window));
		GameManager::getInstance().setWindow(m_window);

		m_stateManager.addState("menu", std::make_shared<MenuState>());
		m_stateManager.addState("playing", std::make_shared<PlayingState>());
		m_stateManager.addState("score", std::make_shared<ScoreState>());
		m_stateManager.addState("tutorial", std::make_shared<TutorialState>());

		m_stateManager.setState("menu");

		std::srand((unsigned) time(0));

		ResourceManager::getInstance().getMusic("music")->setLoop(true);
		ResourceManager::getInstance().getMusic("music")->play();


		return true;
	}

	void update()
	{
		m_stateManager.update();
		GameObjectManager::getInstance().updateAll();
		InputManager::getInstance().updateBindings();
	}

	void draw()
	{
		m_window->clear();
		RenderManager::getInstance().drawAll(*m_window);
		m_stateManager.draw(*m_window);
		GameManager::getInstance().getGui()->draw();
		m_window->display();
	}

	void shutdown()
	{
	}

	std::shared_ptr<sf::RenderWindow> m_window;
	GameStateManager m_stateManager;
};


int main()
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Game game;
	game.Run();

	//assert(!_CrtDumpMemoryLeaks());
	return 0;
}

