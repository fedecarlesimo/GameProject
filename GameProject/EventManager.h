#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include <functional>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Utilities.h"

enum class EventType{                      //enumeriamo i tipi di eventi possibili

	KeyDown = sf::Event::KeyPressed, 
	KeyUp = sf::Event::KeyPressed,
	WindowResized = sf::Event::Resized,
	GainedFocus = sf::Event::GainedFocus, 
	LostFocus = sf::Event::LostFocus, 
	Closed = sf::Event::Closed, 
	TextEntered = sf::Event::TextEntered, 
	Keyboard = sf::Event::Count + 1        //essendo gli enumeratori valori interi, l'ulitma riga previene 
                                           //ogni possibile "scontro" tra interi e impedisce che sia aggiunto 
	                                       //qualcosa di più grande di valore rispetto a EventType
};

struct EventInfo {                         //struttura che associa un valore ad un tasto ed alla sua tipologia

	EventInfo() {                          //associa ad ogni tipologia di evento il numero corrispettivo a code, 0 se default
		code = 0;
	}
	EventInfo(int event) {
		code = event;
	}
	union {                                //usiamo union per salvare il codice dell'evento 
		int code; 
	};
	

};
