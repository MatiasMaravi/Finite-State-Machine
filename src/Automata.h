#include<set>
#include<vector>
#include<string>
using namespace std;

/**
 * @class State
 * @brief Clase que representa un estado de un autómata
*/
class State{
    int id;
public:
    State(int id);
    int get_id();
};
State::State(int id){
    this->id = id;
}
int State::get_id(){
    return this->id;
}
/**
 * @class Transition
 * @brief Clase que representa una función de transición de un autómata
*/
class Transition{
    set<State*> states;// Conjunto de estados del autómata
    set<char> alphabet;// Conjunto de símbolos
    vector<pair<pair<State*, char>, State*>> transitions;// Transiciones
    bool alltransitions;// Indica si se han creado todas las transiciones
    public:
    Transition(set<State*> states, set<char> alphabet);
    bool make_transition(State* s, char c, State* s2);
    bool complete();
    State* next_state(State* s, char c);
};

Transition::Transition(set<State*> states, set<char> alphabet){
    this->states = states;
    this->alphabet = alphabet;
    alltransitions = false;
}
bool Transition::make_transition(State* s, char c, State* s2){
    if(transitions.size() == states.size()*alphabet.size()) {
        alltransitions = true;
    }
    if(states.find(s) == states.end() || states.find(s2) == states.end() || alphabet.find(c) == alphabet.end()){
        return false;
    }
    if(!alltransitions) {
        transitions.push_back(make_pair(make_pair(s, c), s2));
        return true;
    }else{
        return false;
    }
}
bool Transition::complete(){
    return alltransitions;
}
State* Transition::next_state(State* s, char c){
    if(states.find(s) == states.end() || alphabet.find(c) == alphabet.end()){
        return nullptr;
    }
    for(auto i: transitions){
        if(i.first.first == s && i.first.second == c) return i.second;
    }
    return nullptr;
}
/**
 * @class Automata
 * @brief Clase que representa un autómata
*/
class Automata{
    set<State*> states;// Conjunto de estados del autómata
    State* initialState;// Estado inicial
    set<State*> finalStates;// Conjunto de estados finales
    set<char> alphabet;// Conjunto de símbolos
    Transition* function_transition;// Función de transición
    static Automata* instance;// Instancia del autómata
    Automata(State* init, set<State*> final, set<char> alphabet, set<State*> states, Transition* function_transition){
        this->initialState = init;
        this->finalStates = final;
        this->alphabet = alphabet;
        this->states = states;
        this->function_transition = function_transition;
    }
public:
    Automata* getInstance(State* init, set<State*> final, set<char> alphabet, set<State*> states, Transition* function_transition){
        if(instance == 0){
            instance = new Automata(init, final, alphabet, states, function_transition);
        }
        return instance;
    }
    bool run(string word){
        State* current_state = initialState;
        for(auto i: word){
            current_state = function_transition->next_state(current_state, i);
            if(current_state == nullptr) return false;
        }
        return (finalStates.find(current_state) != finalStates.end())?true:false;
    }

    void reset(){
        instance = nullptr;
    }
};

Automata* Automata::instance = 0;