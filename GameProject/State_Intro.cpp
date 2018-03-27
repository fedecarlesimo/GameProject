#include "State_Intro.h"
#include"StateManager.h"


State_Intro::State_Intro(StateManager* l_stateManager)
	: BaseState(l_stateManager) {}


State_Intro::~State_Intro(){}

void State_Intro::OnCreate() {

	timePassed = 0.0f;

	sf::Vector2u windowSize = stateMgr->GetContext()->wind->GetRenderWindow()->getSize();    /*tramite l'uso del puntatore a state manager dalla base class, 
																						     otteniamo la dimensione della finestra.*/
	introTexture.loadFromFile("intro.png");
	introSprite.setTexture(introTexture);
	introSprite.setOrigin(introTexture.getSize().x / 2.0f,
		introTexture.getSize().y / 2.0f);

	introSprite.setPosition(windowSize.x / 2.0f, 0);

	font.loadFromFile("arial.ttf");
	text.setFont(font);
	text.setString({ "Press SPACE to continue" });
	text.setCharacterSize(15);
	sf::FloatRect textRect = text.getLocalBounds();                                          /*settiamo l'origine del text esattamente al centro dello schermo, tramite il dato 
																							 di tipo float rect, e chiamando il metodo getLocalBounds. I valori di top e left 
																							 del float rect rappresentano il top left corner del testo , che possono essere usati 
																							 per calcolare il centro, aggiungnedo ad esso metà della dimensione del rettangolo. */
	text.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	text.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

	EventManager* evMgr = stateMgr->GetContext()->eventManager;                              /* leghiamo il la barra spaziatrice al metodo continue: lo facciamo ottenendo la istanza
																							 event manager tramite lo shared context, e aggiungendo il callback.*/
	evMgr->AddCallback(StateType::Intro, "Intro_Continue", &State_Intro::Continue, this);

}

void::State_Intro::OnDestroy() {                                                             /*Uguale a AddCallback solo che chiama il metodo RemoveCallback. */
	EventManager* evMgr = stateMgr->GetContext()->eventManager;
	evMgr->RemoveCallback(StateType::Intro, "Intro_Continue");
}

void::State_Intro::Update(const sf::Time& lTime) {                                           /*Metodo che permette alla texture della intro di scorrere verticalmente
																							 fino al centro dello schermo.*/
	if (timePassed < 0.5f) {
		timePassed += lTime.asSeconds();
		introSprite.setPosition(introSprite.getPosition().x, 
			introSprite.getPosition().y + (48 * lTime.asSeconds()));
	}
}

void State_Intro::Draw() {                                                                    /* Dopo aver ottenuto un puntatore a window tramite lo sharedcontext, 
																							  disegnamo la sprite sullo schermo. Se passano più di 5 secondi, viene disegnato anche 
																							  il testo che dice di continuare.*/
	
	sf::RenderWindow* window = stateMgr->GetContext()->wind->GetRenderWindow();

	window->draw(introSprite);
	if (timePassed >= 5.0f) {
		window->draw(text);
	}
}

void State_Intro::Continue(EventDetails* l_details) {
		if (timePassed >= 5.0f) {                                                            /*Dopo aver controllato che siano passati 5 secondi, viene cambiato lo stato 
																							 con il menu, e inoltre viene rimosso lo stato intro dato che non ne abbiamo
																							 più bisogno.*/
			stateMgr->SwitchTo(StateType::MainMenu);
			stateMgr->Remove(StateType::Intro);
		}
	}

void State_Intro::Activate(){}
void State_Intro::Deactivate(){}