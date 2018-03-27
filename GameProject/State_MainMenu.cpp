#include "State_MainMenu.h"
#include"StateManager.h"



State_MainMenu::State_MainMenu(StateManager* lStateManager)
	: BaseState(lStateManager) {}

State_MainMenu::~State_MainMenu() {}

void State_MainMenu::OnCreate() {

	font.loadFromFile("arial.ttf");
	text.setFont(font);
	text.setString(sf::String("MAIN MENU: "));
	text.setCharacterSize(18);

	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	text.setPosition(400, 100);

	buttonSize = sf::Vector2f(300.0f, 32.0f);
	buttonPos = sf::Vector2f(400, 200);
	buttonPadding = 4;

	std::string str[2];
	str[0] = "PLAY";
	str[1] = "EXIT";

	for (int i = 0; i < 2; ++i) {
		sf::Vector2f buttonPosition(buttonPos.x, buttonPos.y + (i*(buttonSize.y + buttonPadding)));
		rects[i].setSize(buttonSize);
		rects[i].setFillColor(sf::Color::Red);
		rects[i].setOrigin(buttonSize.x / 2.0f, buttonSize.y / 2.0f);
		rects[i].setPosition(buttonPosition);

		labels[i].setFont(font);
		labels[i].setString(sf::String(str[i]));
		labels[i].setCharacterSize(12);

		sf::FloatRect rect = labels[i].getLocalBounds();
		labels[i].setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f);
		labels[i].setPosition(buttonPosition);
	}

	EventManager* evMgr = stateMgr->GetContext()->eventManager;
	evMgr->AddCallback(StateType::MainMenu, "Button_Click", &State_MainMenu::buttonClick, this);
}

void State_MainMenu::OnDestroy() {
	EventManager* evMgr = stateMgr->GetContext()->eventManager;
	evMgr->RemoveCallback(StateType::MainMenu, "Button_Click");
}

void State_MainMenu::Activate() {
	if (stateMgr->HasState(StateType::Game) && labels[0].getString() == "PLAY") {
		labels[0].setString(sf::String("RESUME"));
		sf::FloatRect rect = labels[0].getLocalBounds();
		labels[0].setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f);
	}	
}

///