#include "EventManager.h"

EventManager::EventManager() : hasFocus(true) { LoadBindings(); }        /*chiama il metodo LoadBindings che è usato per caricare
																		 le informazioni sui nostri legami da un file. */

EventManager::~EventManager() {
	for (auto &itr : bindings) {
		delete itr.second;
		itr.second = nullptr;
	}
}

bool EventManager::AddBinding(Binding *l_binding) {                       /*prende in ingresso un puntatore a Binding. Controlla se
																		  il Binding ha già un legame con lo stesso nome: se sì, il
																		  metodo ritorna false, altrimenti inserisce il nuovo legame
																		  nel container.*/
	if (bindings.find(l_binding->name) != bindings.end())
		return false;

	return bindings.emplace(l_binding->name, l_binding).second;
}

bool EventManager::RemoveBinding(std::string lName) {                      /*prende come argomento una stringa e cerca in Binding un
																		   riscontro per salvarlo in un'iteratore. Se trova un riscontro,
																		   prima elimina il secondo elemento nella coppia key-value, che
																		   è la memoria dinamicamente allocata per l'oggetto legato,
																		   e poi elimina l'entrata dal container per poi ritornare true.*/
	auto itr = bindings.find(lName);
	if (itr == bindings.end()) { return false; }
	delete itr->second;
	bindings.erase(itr);
	return true;
}

void EventManager::SetCurrentState(StateType lState)
{
	currentState = lState;
}

void EventManager::SetFocus(const bool& lFocus) { hasFocus = lFocus; }

void EventManager::HandleEvent(sf::Event& lEvent) {                      /*processa gli eventi SFML che sono trascinati in ogni iterazione
																		 per "guardarli" e vedere se ci fosse qualcosa in cui siamo interessati.*/

	for (auto &b_itr : bindings) {
		Binding* bind = b_itr.second;                                    /* Serve per iterare su tutti i Bindings e dentro ogni evento dentro il legame
																		 per controllare se il tipo dell'argomento coincide col tipo dell'evento binding
																		 che è attualmente in atto. */
		for (auto &e_itr : bind->events) {
			EventType sfmlEvent = (EventType)lEvent.type;                /*crea la variabile che contiene il tipo dell'evento passato
																		 come argomento*/
			if (e_itr.first != sfmlEvent) { continue; }
			if (sfmlEvent == EventType::KeyDown || sfmlEvent == EventType::KeyUp) {
				if (e_itr.second.code == lEvent.key.code) {
					// leghiamo l'evento alla pressione del pulsante.
					// Incremento count.
					if (bind->details.keyCode != -1) {
						bind->details.keyCode = e_itr.second.code;
					}
					++(bind->c);
					break;
				}
			}
			else {

				if (sfmlEvent == EventType::WindowResized) {
					bind->details.size.x = lEvent.size.width;
					bind->details.size.y = lEvent.size.height;
				}
				else if (sfmlEvent == EventType::TextEntered) {
					bind->details.textEntered = lEvent.text.unicode;
				}
				++(bind->c);
			}
		}
	}

}

void EventManager::Update() {
	if (!hasFocus) { return; }
	for (auto &b_itr : bindings) {
		Binding* bind = b_itr.second;
		for (auto &e_itr : bind->events) {
			switch (e_itr.first) {
			case(EventType::Keyboard):
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(e_itr.second.code))) {
					if (bind->details.keyCode != -1) {
						bind->details.keyCode = e_itr.second.code;
					}
					++(bind->c);
				}
				break;
			}
		}

		if (bind->events.size() == bind->c) {                           /*controlliamo se il numero di eventi nel container corrisponde col numero di
																		eventi che sono in funzione. Usiamo find per ottenere la map di tutti i callback 
																		per lo stato corrente. */
			
			auto stateCallbacks = callbacks.find(currentState);
			//Questo passa in ingresso un state Type di valore 0, che non è un numero valido. 
			// Facciamo ciò perchè anche nel caso in cui si abbiano stati multipli, 
			// vogliamo comunque processare callback globali per la classe Window
			auto otherCallbacks = callbacks.find(StateType(0));

			if (stateCallbacks != callbacks.end()){                          /* Usiamo il metodo find del secondo valore nell'iteratore che viene restituito 
																			 dalla prima ricerca, che è la nostra callback map attuale. Se viene trovato un riscontro
																			 allora viene invocata la funzione. */
				auto callItr = stateCallbacks->second.find(bind->name);
				if (callItr != stateCallbacks->second.end()) {
					//passo dentro informazioni riguardo l'evento
					callItr->second(&bind->details);
				}
			}
		}
		bind->c = 0;
		bind->details.Clear();
	}
}

void EventManager::LoadBindings() {                                     /*carichiamo da file dei legami (nome,codice tipo : codice evento)*/
	std::string delimiter = ":";

	std::ifstream bindings;                                             /*bindings è un oggetto di tipo ifstream che ci permette di eseguire operazioni
																		di input/output sul file associato(se presenti)*/
	bindings.open("keys.cfg");
	if (!bindings.is_open()) { std::cout << "! Failed loading keys.cfg." << std::endl; return; }
	std::string line;
	while (std::getline(bindings, line)) {
		std::stringstream keystream(line);                              /*stringstream è un oggetto che permette di leggere, mediante un buffer di stringhe,
																		i valori sul file pezzo per pezzo, usando l'operatore >>, trattandoli come stringhe*/
		std::string callbackName;
		keystream >> callbackName;
		Binding* bind = new Binding(callbackName);                      /*dopo aver ottenuto il nome del nostro legame, creiamo un nuovo oggetto"Binding"
																		e passiamo il nome al costruttore*/
		while (!keystream.eof()) {                                      /*eof indica End of File, ovvero eseguiamo il while fino a quando l'oggetto stringstream
																		non raggiunge la fine della linea che sta leggendo*/

			std::string keyval;                                         /*dopo aver letto i valori numerici da file, dobbiamo convertirli in due valori interi
																		i quali verranno salvari nelle rispettive variabili locali. Operazione eseguita dal delimiter*/
			keystream >> keyval;
			int start = 0;
			int end = keyval.find(delimiter);
			if (end == std::string::npos)                               /*se il non viene trovato nella stringa, allora l'istanza binding viene eliminata,
																		e passa alla linea successiva*/
			{
				delete bind;
				bind = nullptr;
				break;
			}
			EventType type = EventType(stoi(keyval.substr(start, end - start))); /*la conversione viene eseguita dal metodo stoi; mediante substr prendiamo la coppia
																				 keyvalue, partendo dall'inizio del codice ed arrivando fino alla coppia successiva.
																				 type contiene quindi la conversione in valore del tipo di evento che è stato passato
																				 tramite stringa*/
			int sCode = stoi(keyval.substr(end + delimiter.length(),
				keyval.find(delimiter, end + delimiter.length())));              /*il metodo find delimiter ci permette di trovare nella serie di valori i ":" presenti
																				 così da indicare a stoi quale parte del codice convertire, in quanto andrà da delimiter
																				 all fine + la lunghezza del delimiter stesso*/
			EventInfo eventInfo;
			eventInfo.code = sCode;                                              /*il valore appena convertito cioè sCode viene messo nel code di eventInfo*/
			bind->BindEvent(type, eventInfo);                                    /*creiamo il legame chiamando su bind il BindEvent passandogli type e eventInfo*/
		}

		if (!AddBinding(bind))                                                   /*proviamo ad aggiungere il bind all'event Manager. Utiliziamo un if, per evitare
																				 i problemi di scontro tra i vari binding, come la presenza dello stesso codice. Altrimenti
																				 lo eliminiamo*/
		{
			delete bind;
		}
		bind = nullptr;
	}
	bindings.close();
}