#include "Window.h"

Window::Window() {
	Setup("Window", sf::Vector2u(640, 480));
}

Window::Window(const std::string& title, const sf::Vector2u& size)
{
	Setup(title, size);
}

Window::~Window() { window.close(); }

void Window::Setup(const std::string& title, const sf::Vector2u& size)
{
	windowTitle = title;
	windowSize = size;
	isFullscreen = false;
	isDone = false;
	isFocused = true;

	eventManager.AddCallback(StateType(0), "Fullscreen_toggle", &Window::ToggleFullscreen, this);           /*AddCallback ci permette di eseguire il setup dei tipi di eventi letti da file.
																							                 Invece this viene usato per riferirsi alla istanza corrente di window*/
	eventManager.AddCallback(StateType(0), "Window_close", &Window::Close, this);

	Create();
}

void Window::Create() {
	sf::Uint32 style = sf::Style::Default;                                                  /*crea lo style e lo mette come un Uint32, impostandolo di base come Default.
																							Se è fullscreen, allora lo style viene impostato solo a fullscreen*/
	if (isFullscreen)
	{
		style = sf::Style::Fullscreen;
	}

	window.create(sf::VideoMode(windowSize.x, windowSize.y, 32), windowTitle, style);       /* il 32 è richiesto da questa particolare tipologia di VideoMode, per successivamente
																							impostare il numero di bit presenti in un pixel*/
}

void Window::BeginDraw() { window.clear(sf::Color::Black); }
void Window::EndDraw() { window.display(); }

bool Window::IsDone() { return isDone; }
bool Window::IsFullScreen() { return isFullscreen; }
bool Window::IsFocused() { return isFocused; }

sf::RenderWindow* Window::GetRenderWindow() { return &window; }
EventManager* Window::GetEventManager() { return &eventManager; }
sf::Vector2u Window::GetWindowSize() { return windowSize; }

void Window::ToggleFullscreen(EventDetails* lDetails) {                                     /*metodo per togliere il fullscreen e viceversa */
	isFullscreen = !isFullscreen;
	window.close();
	Create();
}

void Window::Close(EventDetails* lDetails) { isDone = true; }

void Window::Update() {                                                                    /*serve per impostare se non c'è il focus sulla finestra, inoltre abbiamo una gestione basilare
																						   degli eventi per permettere gli update*/
	sf::Event event;

	while (window.pollEvent(event)) {
		if (event.type == sf::Event::LostFocus) { isFocused = false; eventManager.SetFocus(false); }
		else if (event.type == sf::Event::GainedFocus) { isFocused = true; eventManager.SetFocus(true); }
		eventManager.HandleEvent(event);
	}

	eventManager.Update();
}