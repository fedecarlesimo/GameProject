#pragma once
#include "Window.h"
#include "EventManager.h"
struct SharedContext {                                                /*struct che serve per tenere i puntatori alla classe window principale e all'EventManager*/
	SharedContext() : wind(nullptr), eventManager(nullptr) {}         /*serve per tenere traccia dei puntatori dei nostri device context, ovvero classi e controller*/
	Window* wind;
	EventManager* eventManager;
}; 
