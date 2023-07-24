#include<iostream>
#include<cstdlib>
#include<limits>
#include "Automata.h"
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::system;
using std::numeric_limits;
using std::streamsize;
using std::set;
void message(string w){
    system("clear");
    cout << w << endl;
    system("read -p 'Press Enter to continue...' var");
}

int validar_numero(int n, string w=""){
    // Validación del valor de entrada
    while (!(cin >> n)) {
        system("clear");
        cout << "Invalid input. Please enter a valid number "<<w<<": ";
        cin.clear(); // Limpia la bandera de error de cin
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignora el resto de la línea incorrecta
    }
    return n;
}
string menu(){
    system("clear");
    string choice;
    cout << "1. Create automata" << endl;
    cout << "2. Run automata" << endl;
    cout << "3. Exit" << endl;
    cout << "Enter choice: ";
    cin >> choice;
    return choice;
}
bool run_automata(Automata* a){
    system("clear");
    string word = "";
    cout << "Enter word: ";
    cin >> word;
    return (a->run(word))?true:false;
}
char validar_char(int i){
    string c;
    do
    {
        system("clear");
        cout << "Enter symbol "<<i+1<<": ";
        cin>>c;
    } while (c.length() != 1);

    return c[0];
}
set<char> get_alphabet(){
    int n;
    cout << "Enter number of symbols: ";
    n = validar_numero(n, "of symbols");
    set<char> alphabet;
    for(int i = 0; i < n; i++){
        char c;
        c = validar_char(i);
        alphabet.insert(c);
    }
    return alphabet;
}
State* get_state(set<State*> states, int id){
    for(auto i: states){
        if(i->get_id() == id) return i;
    }
    return nullptr;
}
State* get_init(set<State*> states){
    int id = -1;
    while(!get_state(states, id)){
        system("clear");
        cout << "Enter initial state: ";
        id = validar_numero(id);
    }
    return get_state(states, id);
}
set<State*> get_finals(set<State*> states){
    set<State*> finals;
    set<int> ids;
    int id = -1;
    cout<<"Enter number of final states: ";
    int n = validar_numero(n, "of final states");
    while(n>states.size()){
        system("clear");
        cout<<"Number of final states must be less than or equal to number of states"<<endl;
        cout<<"Enter number of final states: ";
        n = validar_numero(n, "of final states");
    }
    for(int i = 0; i < n; i++){
        id = -1;
        while (!get_state(states, id) || ids.find(id) != ids.end()){
            system("clear");
            cout << "Enter final state "<<i+1<<": ";
            id = validar_numero(id);
        }
        finals.insert(get_state(states, id));
        ids.insert(id);
    }
    return finals;
}

set<State*> create_states() {
    cout << "Enter number of states: ";
    int n = validar_numero(n,"of states");
    set<State*> states;
    for (int i = 0; i < n; i++) {
        State* s = new State(i);
        states.insert(s);
    }
    return states;
}
Transition* make_function_transition(set<State*> states, set<char> alphabet){
    Transition* t = new Transition(states, alphabet);
    for(auto i: states){
        for(auto j: alphabet){
            system("clear");
            cout << "Enter next state for state " << i->get_id() << " and symbol " << j << ": ";
            int id;
            id = validar_numero(id);
            while (!t->make_transition(i, j, get_state(states, id))){
                message("Invalid transition");
                cout << "Enter next state for state " << i->get_id() << " and symbol " << j << ": ";
                id = validar_numero(id);
            }
            
        }
    }
    return t;
}
Automata* create_automata(){
    system("clear");
    set<State*> states = create_states();
    set<char> alphabet = get_alphabet();
    Transition* t = make_function_transition(states, alphabet);
    State* init = get_init(states);
    set<State*> finals = get_finals(states);
    Automata* a = a->getInstance(init, finals, alphabet, states, t);
    return a;
}

int main(){
    string opcion = menu();
    Automata* a = nullptr;
    while(opcion != "3"){
        if(opcion == "1"){
            if(a != nullptr) a->reset();
            a = create_automata();
        }
        else if(opcion == "2"){
            if(a == nullptr){
                message("Create automata first");
                opcion = menu();
                continue;
            }else{
                if(run_automata(a)) message("Accept");
                else message("Reject");
            }
            
        }
        else {
            message("Invalid option");
        }
        opcion = menu();
    }
}