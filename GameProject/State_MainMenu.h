#pragma once
#include"BaseState.h"


class State_MainMenu : public BaseState { 
public:
	State_MainMenu(StateManager* lStateManager);
	~State_MainMenu();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& lTime);
	void Draw();
	
	void buttonClick(EventDetails* lDetails);

private:
	sf::Text text;
	sf::Font font; 

	sf::Vector2f buttonSize;
	sf::Vector2f buttonPos;
	unsigned int buttonPadding;

	sf::RectangleShape rects[2];
	sf::Text labels[2];


};

