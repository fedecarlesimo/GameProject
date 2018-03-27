#pragma once
#include"EventManager.h"
#include"BaseState.h"

class State_Intro : public BaseState
{
public:
	State_Intro(StateManager* l_stateManager);
	~State_Intro();

	// questi che seguono sono effettivamente i veri metodi che creano e distruggono uno stato, 
	// non costruttori e distruttori che non verranno usati.
	void OnCreate();                             
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& l_time);
	void Draw();

	void Continue(EventDetails* lDetails);         // metodo che serve per procedere al menu
private:
	sf::Texture introTexture;
	sf::Sprite introSprite;
	sf::Font font;
	sf::Text text;

	float timePassed;                              // usato per tenere traccia di quanto tempo siamo stati in questo stato

};

