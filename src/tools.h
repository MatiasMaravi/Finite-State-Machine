#include <iostream>
#include<cstdlib>
#include<chrono>
#include<thread>
using std::cout;
using std::cin;
using std::endl;
using std::string;
#ifdef _WIN32
#define PAUSE_COMMAND "pause"
#else
#define PAUSE_COMMAND "read -p 'Press Enter to continue...' var"
#endif
/**
 * @brief Obtiene el sistema operativo en el que se está ejecutando el programa.
 * @return El sistema operativo en el que se está ejecutando el programa.
*/
string getOperatingSystem(){
    #ifdef _WIN32
        return "Windows";
    #elif __APPLE__
        return "macOS";
    #elif __linux__
        return "Linux";
    #else
        return "Unknown";
    #endif
}
/**
 * @brief Limpia la consola.
*/
void clear_console(){
    string os = getOperatingSystem();
    if(os == "Windows") system("cls");
    else system("clear");
}
/**
 * @brief Muestra un mensaje en la consola y espera a que el usuario presione Enter.
 * @param w El mensaje a mostrar.
*/
void message(string w){
    clear_console();
    cout << w << endl;
    system(PAUSE_COMMAND);
}
void sleep_console(string w){
    cout << w;
    for (int i = 0; i < 3; i++) {
        std::cout << '.';
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}