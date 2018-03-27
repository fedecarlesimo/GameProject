#pragma once
#include <string>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "EventManager.h"

class Window {
public:
	Window();
	Window(const std::string& title, const sf::Vector2u& size);
	~Window();

	void BeginDraw();                                                     //ripulisce la finestra
	void EndDraw();                                                       //mostra i cambiamenti

	void Update();
	bool IsDone();
	bool IsFullScreen();
	bool IsFocused();

	void ToggleFullscreen(EventDetails* lDetails);                        /*si rapporta con l'EventManager per gestire la chiusura e il Fullscreen*/
	void Close(EventDetails* lDetails = nullptr);

	sf::RenderWindow* GetRenderWindow();
	EventManager* GetEventManager();                                      /*azioni standard di sfml per gestire la visualizzazione della finestra, e
																		  permette la gestione da parte di EventManager*/
	sf::Vector2u GetWindowSize();

private:
	void Setup(const std::string& title, const sf::Vector2u& size);
	void Create();

	sf::RenderWindow window;
	EventManager eventManager;
	sf::Vector2u windowSize;
	std::string windowTitle;
	bool isDone;
	bool isFullscreen;
	bool isFocused;
};