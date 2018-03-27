#include "StateManager.h"



StateManager::StateManager(SharedContext* lShared) : shared(lShared)
{
	RegisterState<State_Intro>(StateType::Intro);
	RegisterState<State_MainMenu>(StateType::MainMenu);
	RegisterState<State_Game>(StateType::Game);
	RegisterState<State_Paused>(StateType::Paused);
}

StateManager::~StateManager() {                                            /*itera su tutti gli stati ed elimina il secondo valore della coppia*/
	for (auto &itr : states) {
		itr.second->OnDestroy();
		delete itr.second;
	}
}

void StateManager::Update(const sf::Time& lTime) {                        /*il funzionamento è uguale a quello di draw, cambia solo il check per la flag della trascendent
																		  la quale updata*/
	if (states.empty()) { return; }
	if (states.back().second->IsTranscendent() && states.size() > 1) {
		auto itr = states.end();
		while (itr != states.begin()) {
			if (itr != states.end()) {
				if (!itr->second->IsTranscendent()) {
					break;
				}
			}
			--itr;
		}
		for (; itr != states.end(); ++itr) {
			itr->second->Update(lTime);
		}
	}
	else {
		states.back().second->Update(lTime);
	}
}

void StateManager::Draw() {
	if (states.empty()) { return; }                                       /*controlla se lo state container è vuoto, se non lo è, allora controlla anche la trasnsparency
																		  per controllare quanti stati sono nello stack*/
	if (states.back().second->IsTransparent() && states.size() > 1) {     /*se c'è solo uno stato nello stack oppure transparent è false, allora invochiamo il suo metodo draw*/
		auto itr = states.end();
		while (itr != states.begin()) {
			if (itr != states.end()) {
				if (!itr->second->IsTransparent()) {                      /*per renderizzare correttamente gli stati trasparenti, dobbiamo chiamare il loro rispettivo metodo draw
																		  dove il più vecchio stato sullo stack viene disegnato per ultimo, questo viene fatto backward, fino a trovare
																		  uno stato che non è trasparente*/
					break;
				}
			}
			--itr;
		}
		for (; itr != states.end(); ++itr) {                              /*dopo averlo trovato il for permette di invocare il draw su tutti gli stati a partire da quello trovato*/
			itr->second->Draw();
		}
	}
	else {
		states.back().second->Draw();
	}
}

SharedContext* StateManager::GetContext() { return shared; }

bool StateManager::HasState(const StateType& lType) {                    /*metodo che controlla se uno stato si trova o meno sullo stack*/
	for (auto itr = states.begin();
		itr != states.end(); ++itr)
	{
		if (itr->first == lType) {
			auto removed = std::find(toRemove.begin(), toRemove.end(), lType);
			if (removed == toRemove.end()) { return true; }
			return false;
		}
	}
	return false;
}

void StateManager::ProcessRequests() {                                  /*itera sul vettore toRemove e invoca il metodo primato removeState, il quale eseguirà la deallocazione,
																		e pulisce il vettore remove per renderlo vuoto*/
	while (toRemove.begin() != toRemove.end()) {
		RemoveState(*toRemove.begin());
		toRemove.erase(toRemove.begin());
	}
}

void StateManager::SwitchTo(const StateType& lType) {                  /*tramite lo shared accediamo all'eventManager e chiamiamo il metodo setCurrentState*/
	shared->eventManager->SetCurrentState(lType);                      /*accede all'eventManager con shared*/
	for (auto itr = states.begin();                                    /*iteriamo gli stati dove il corrente che sta per essere pushato indietro, verrà disattivato
																	   questo permette di finire le sue azioni prima di essere dismesso*/
		itr != states.end(); ++itr)
	{
		if (itr->first == lType) {
			states.back().second->Deactivate();
			StateType tmp_type = itr->first;                          /*creiamo due variabili temporanee per far mantenere le informazioni mentre avviene lo switch
																	  sia dello statetipe che dell'oggetto, questo ci permette di non perdere l'informazione quando
																	  viene chiamato l'erase*/
			BaseState* tmp_state = itr->second;
			states.erase(itr);
			states.emplace_back(tmp_type, tmp_state);
			tmp_state->Activate();                                     /*per passare allo stato che vogliamo, facciamo il push back nel vettore e gli passiamo le due variabili temporanee*/

		}
		return;
	}


if (!states.empty()) { states.back().second->Deactivate(); }           /*dobbiamo prima disattivare i metodi, successivamente prendiamo lo stato sul vettore più recente e lo attiviamo,
																       qualora il vettore degli stati non avesse quel vettore*/
CreateState(lType);
states.back().second->Activate();
}


void StateManager::Remove(const StateType& lType) {                     /*pusha uno state type nel vettore remove che sarà poi processato del processRequest*/
	toRemove.push_back(lType);
}

void StateManager::CreateState(const StateType& lType) {                /* Viene creato un iteratore stateFactory e viene checkatp per unirlo all'iteratore 
																		ritornato dal metodo end(), e ciò ci garantisce che uno stato con quel tipo possa essere creato.
																		Se può,  viene creato state, che prende il secondo valore di newState che sarebbe la funzione che 
																		restituisce un puntatore alla state class appena creata, dopodichè viene  lo pushamo back dallo state vector
																		e chiamiamo OnCreate(). */
	auto newState = stateFactory.find(lType);
	if (newState == stateFactory.end()) { return; }
	BaseState* state = newState->second();
	states.emplace_back(lType, state);
	state->OnCreate();
}

void StateManager::RemoveState(const StateType& lType) {                /*Come sempre iteriamo fino a che non viene trovata una corrispondenza. Rimuovere lo stato attuale inizia 
																		con la chiamata di OnDestroy su tale stato, dopodichè de-allochiamo la memoria usando il delete. Infine 
																		eliminiamo l'elemento dallo state vector e usciamo dal metodo.*/
	for (auto itr = states.begin();
		itr != states.end(); ++itr)
	{
		if (itr->first == lType) {
			itr->second->OnDestroy();
			delete itr->second;
			states.erase(itr);
			return;
		}
	}
}
