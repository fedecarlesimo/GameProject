#pragma once
#include <vector>
#include <unordered_map>
#include "State_Intro.h"
#include "State_MainMenu.h"
#include "State_Game.h"
#include "State_Paused.h"
#include "SharedContext.h"



enum class StateType { Intro = 1, MainMenu, Game, Paused, GameOver };                            /*enumerazione dei vari stati del gioco*/

using StateContainer = std::vector<std::pair<StateType, BaseState*>>;                            /*vettore che contiene due valori al tipo e allo stato, l'utilizzo di una mappa risultava
																								 peggiore in quanto una mappa non manterebbe l'ordine sullo stack, essenziale al funzionamento*/

using TypeContainer = std::vector<StateType>;                                                    /*vettore che contiene tutti i vari tipi di stato*/

using StateFactory = std::unordered_map<StateType, std::function<BaseState*(void)>>;             /*container che serve per modificare le funzioni, che serviranno per produrre oggetti di diverso tipo
																								 a seconda della funzione, usiamo una mappa per legare uno specifico state type ad una specifica funzione*/
class StateManager {
public:
	StateManager(SharedContext* lShared);
	~StateManager();

	void Update(const sf::Time& lTime);
	void Draw();

	void ProcessRequests();                                                                      /*rimuove gli stati quando non sono più in uso e verrà chiamato per ultimo*/

	SharedContext* GetContext();
	bool HasState(const StateType& lType);

	void SwitchTo(const StateType& lType);                                                        /*prende in ingresso uno state type e cambia lo stato corrente con quello che è stato passato in ingresso*/
	void Remove(const StateType& lType);                                                          /*rimuove lo stato dallo stack a seconda dell'ingresso*/

private:

	void CreateState(const StateType& lType);
	void RemoveState(const StateType& lType);

	template<class T>                                                                           /*il template lega la stateFactory ad una funzione che restituisce un puntatore alla memoria
																								appena allocata. Usiamo i template per ridurre il codice, in quanto adrebbe eseguito per ogni stato.
																								Il this viene passato in quanto ogni stato ha bisogno di un puntatore alla classe state manager*/
	void RegisterState(const StateType& lType) {
		stateFactory[lType] = [this]() -> BaseState*
		{
			return new T(this);
		};
	}

	SharedContext* shared;
	StateContainer states;
	TypeContainer toRemove;                                                                      /*tiene traccia dei vari stati che devono essere eliminati dallo stack in ordine*/
	StateFactory stateFactory;
}; 
