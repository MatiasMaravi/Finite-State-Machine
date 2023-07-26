#include "Automata.h"
#include "tools.h"
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::numeric_limits;
using std::streamsize;
using std::set;

/**
 * @brief Valida y obtiene un número entero ingresado por el usuario.
 *
 * Esta función solicita al usuario ingresar un número entero y lo valida.
 * Si el usuario ingresa un valor no válido, se mostrará un mensaje de error y
 * se pedirá al usuario que ingrese nuevamente el número hasta que sea válido.
 *
 * @param n El número entero ingresado por el usuario (se modifica por referencia).
 * @param w Un mensaje opcional a mostrar antes de solicitar el número (por defecto es una cadena vacía).
 * @return El número entero validado ingresado por el usuario.
 */
int validar_numero(int &n, const string &w = "") {
    // Validación del valor de entrada
    while (!(cin >> n)) {
        clear_console();
        cout << "Invalid input. Please enter a valid number " << w << ": ";
        cin.clear(); // Limpia la bandera de error de cin
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignora el resto de la línea incorrecta
    }
    return n;
}
/**
 * @brief Valida y obtiene un número entero ingresado por el usuario.
 * Imprime un menu en consola y solicita al usuario seleccionar una opción
 * valida.
 * @return La opción seleccionada por el usuario.
 */
string menu(){
    clear_console();
    string choice;
    cout << "1. Create automata" << endl;
    cout << "2. Run automata" << endl;
    cout << "3. Exit" << endl;
    cout << "Enter choice: ";
    cin >> choice;
    return choice;
}
/**
 * @brief Ejecuta un autómata.
 * Solicita al usuario ingresar una palabra y la ejecuta en el autómata.
 * @param a El autómata a ejecutar.
 * @return true si la palabra es aceptada por el autómata, false en caso contrario.
 */
void run_automata(Automata* a,string op){
    clear_console();
    string word = "";
    cout << "Enter word: ";
    cin >> word;
    sleep_console("Processing word");
    if (op == "1"){
        if(a->fast_run(word)){
            message("Accepted word");
        }else{
            message("Rejected word");
        }
    } 
    else if (op == "2") {
        pair <string,bool> aux = a->pretty_run(word);
        if (!aux.second){
            cout << "Sorry, " << aux.first <<" is not a final state" <<endl;
            std::this_thread::sleep_for(std::chrono::seconds(3));
            message("Rejected word");
        }else{
            cout << "Congratulations, " << aux.first <<" is a final state" <<endl;
            std::this_thread::sleep_for(std::chrono::seconds(3));
            message("Accepted word");
        }
    }
}

/** 
 * @brief Valida y obtiene un caracter ingresado por el usuario.
 * @param i El número de caracter a ingresar.
 * @return El caracter ingresado por el usuario.
 */
char validar_char(int i){
    string c;
    do
    {
        clear_console();
        cout << "Enter symbol "<<i+1<<": ";
        cin>>c;
    } while (c.length() != 1);

    return c[0];
}
/**
 * @brief Obtiene el alfabeto de un autómata.
 * Solicita al usuario ingresar el número de símbolos del alfabeto y luego
 * solicita al usuario ingresar cada símbolo.
 * @return El alfabeto del autómata.
 */
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
/**
 * @brief Obtiene un estado de un conjunto de estados.
 * Haciendo uso del id busca el estado en el conjunto de estados.
 * @param states El conjunto de estados.
 * @param id El identificador del estado a buscar.
 * @return El estado con el identificador ingresado.
*/
State* get_state(set<State*> states, int id){
    for(auto i: states){
        if(i->get_id() == id) return i;
    }
    return nullptr;
}
/**
 * @brief Obtiene el estado inicial de un autómata.
 * Solicita al usuario ingresar el identificador del estado inicial.
 * @param states El conjunto de estados del autómata.
 * @return El estado inicial del autómata.
*/
State* get_init(set<State*> states){
    int id = -1;
    while(!get_state(states, id)){
        clear_console();
        cout << "Enter initial state: ";
        id = validar_numero(id);
    }
    State* init = get_state(states, id);
    init->set_initial(true);
    return init;
}
/**
 * @brief Obtiene el conjunto de estados finales de un autómata.
 * Solicita al usuario ingresar el número de estados finales y luego
 * solicita al usuario ingresar cada estado final.
 * @param states El conjunto de estados del autómata.
 * @return El conjunto de estados finales del autómata.
*/
set<State*> get_finals(set<State*> states){
    set<State*> finals;
    set<int> ids;
    int id = -1;
    cout<<"Enter number of final states: ";
    int n = validar_numero(n, "of final states");
    while(n>states.size()){
        clear_console();
        cout<<"Number of final states must be less than or equal to number of states"<<endl;
        cout<<"Enter number of final states: ";
        n = validar_numero(n, "of final states");
    }
    for(int i = 0; i < n; i++){
        id = -1;
        while (!get_state(states, id) || ids.find(id) != ids.end()){
            clear_console();
            cout << "Enter final state "<<i+1<<": ";
            id = validar_numero(id);
        }
        State* s = get_state(states, id);
        s->set_final(true);
        finals.insert(s);
        ids.insert(id);
    }
    return finals;
}
/**
 * @brief Crea un conjunto de estados.
 * Solicita al usuario ingresar el número de estados y luego crea cada estado.
 * @return El conjunto de estados.
*/
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
/**
* @brief Crea una función de transición.
* Solicita al usuario ingresar el siguiente estado para cada estado y símbolo.
* @param states El conjunto de estados del autómata.
* @param alphabet El alfabeto del autómata.
* @return La función de transición.
*/
Transition* make_function_transition(set<State*> states, set<char> alphabet){
    Transition* t = new Transition(states, alphabet);
    for(auto i: states){
        for(auto j: alphabet){
            clear_console();
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
/**
 * @brief Crea un autómata.
 * Función "Builder" para crear el autómata.
 * @return El autómata creado.
*/
Automata* create_automata(){
    clear_console();
    set<State*> states = create_states();
    set<char> alphabet = get_alphabet();
    Transition* t = make_function_transition(states, alphabet);
    State* init = get_init(states);
    set<State*> finals = get_finals(states);
    Automata* a = a->getInstance(init, finals, alphabet, states, t);
    return a;
}
string menu_run(Automata* a){
    clear_console();
    string choice;
    cout<<"Running automata"<<endl;
    cout<<"1. Run fast"<<endl;
    cout<<"2. Run pretty"<<endl;
    cout<<"3. Back"<<endl;
    cout<<"Enter choice: ";
    cin>>choice;
    return choice;
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
                string op = menu_run(a);
                if(op == "1" || op == "2") run_automata(a,op);
                else if(op=="3"){
                    opcion = menu();
                    continue;
                }
                else message("Invalid option");
            }
        }
        else {
            message("Invalid option");
        }
        opcion = menu();
    }
}