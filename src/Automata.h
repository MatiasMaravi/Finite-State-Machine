#include<set>
#include<vector>
#include<string>
#include<thread>
#include<chrono>
#include<iostream>
using namespace std;

/**
 * @class State
 * @brief Clase que representa un estado de un autómata
*/
class State{
    int id;
    bool is_initial;
    bool is_final;
public:
    State(int id);
    int get_id() const;
    void set_initial(bool i);
    void set_final(bool f);
    bool is_initial_state() const;
    bool is_final_state() const;
};
State::State(int id){
    this->id = id;
    this->is_initial = false;
    this->is_final = false;
}
int State::get_id() const{
    return this->id;
}
void State::set_initial(bool i){
    this->is_initial = i;
}
void State::set_final(bool f){
    this->is_final = f;
}
bool State::is_initial_state() const{
    return this->is_initial;
}
bool State::is_final_state() const{
    return this->is_final;
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
    Automata* getInstance(State* init, set<State*> final, set<char> alphabet, set<State*> s, Transition* f);
    pair<string,bool> pretty_run(string word);
    bool fast_run(string word);
    void reset();
};
Automata* Automata::instance = 0;
Automata* Automata::getInstance(State* init, set<State*> final, set<char> alphabet, set<State*> s, Transition* f){
    if(instance == nullptr){
        instance = new Automata(init, final, alphabet, s, f);
    }
    return instance;
}
pair<string,bool> Automata::pretty_run(string word){
    State* current = initialState;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    int aux = 1;
    for(auto i: word){
        current = function_transition->next_state(current, i);
        std::cout << "\x1b[2J\x1b[H"; // Limpia la pantalla y coloca el cursor en la posición (1,1)
        std::cout << "Word: " << word.substr(0,aux) << std::endl;
        std::cout << "Estado actual: " << "q" << current->get_id() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if(current == nullptr) return make_pair("qx",false);
        aux++;
    }
    bool flag = current->is_final_state();
    return make_pair("q"+to_string(current->get_id()),flag);
}
bool Automata::fast_run(string word){
    State* current = initialState;
    for(auto i: word){
        current = function_transition->next_state(current, i);
        if(current == nullptr) return false;
    }
    return current->is_final_state();
}
void Automata::reset(){
    instance = nullptr;
}