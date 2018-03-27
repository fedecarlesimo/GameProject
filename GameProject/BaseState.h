#pragma once
#include <SFML/Graphics.hpp>
class StateManager;

class BaseState {                                                              /*la classe che contiene tutti i metodi virtuali, i quali verranno utilizzati dagli altri stati
																			   ed eredita dalla StateManager. I vari metodi virtual void verranno ereditati dai vari stati*/
	friend class StateManager;                                                 /*utiliziamo una classe friend in quanto, questa può accedere ai membri privati e protetti della
																			   classe a cui è dichiarata friend : in questo caso StateManager è una friend di BaseState*/
public:
	BaseState(StateManager* lStateManager) :stateMgr(lStateManager),
		transparent(false), transcendent(false) {}
	virtual ~BaseState() {}

	virtual void OnCreate() = 0;                                               /*viene invocato dalla classe derivata, quando lo stato è creato e pushato sullo stack
																			   e poi viene rimosso dallo stack*/
	virtual void OnDestroy() = 0;

	virtual void Activate() = 0;                                               /*viene invocato dalla classe derivata, quando lo stato si muove dalla cima dello stack
																			   e quando viene tolto dal top dello stack*/
	virtual void Deactivate() = 0;

	virtual void Update(const sf::Time& lTime) = 0;                            /*viene invocato dalla classe derivata, per updare e disegnare lo stato con il suo contesto*/
	virtual void Draw() = 0;

	void SetTransparent(const bool& lTransparent) { transparent = lTransparent; }
	bool IsTransparent()const { return transparent; }
	void SetTranscendent(const bool& lTranscendence) { transcendent = lTranscendence; }
	bool IsTranscendent()const { return transcendent; }
	StateManager* GetStateManager() { return stateMgr; }

protected:
	StateManager * stateMgr;
	bool transparent;                                                          /*flag che indica se lo stato corrente ha bisogno di fare il render dello stato prima di esso*/
	bool transcendent;                                                         /*flag che indica se lo stato corrente ha bisogno di fare l'update dello stato prima di esso*/
};
                                                                               /*evita di dover creare tutte le combinazioni di stato, e permette quindi di recuperare gli stati*/
