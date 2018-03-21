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

	   void BeginDraw(); //ripulire la finestra
	   void EndDraw();   //mostrare i cambiamenti
	   
	   void Update();
	   bool IsDone(); 
	   bool IsFullScreen(); 
	   bool IsFocused();

	   

	  

};
