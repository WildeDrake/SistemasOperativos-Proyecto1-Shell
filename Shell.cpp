#include <iostream>
#include <limits.h>
#include <signal.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <sys/wait.h>
#include <unistd.h>

#include "favs.cpp"

using namespace std;

// tenemos 1.1, 1.4, 1.6

const int num_comandos = 4;
const string comandos[4] = {"exit", "fav", "set", "test"}; // set = set recordatorio

// Color amarillo \033[1;33m
// Color azul \033[34m
// Color blanco \033[1;0m
void iniciar_shell(vector<string> *favs) {
	system("clear");
	cout << "\033[1;33m ~~~~~~~~~~~~~~~  ^-^\033[34m Bienvenido a Shellcito  \033[1;33m ^-^ "
			  "~~~~~~~~~~~~~~~ \033[1;0m"
		  << endl;
}

vector<string> parsear_comandos(string comando) {
	vector<string> args;
	stringstream ss(comando);
	string arg;
	while (ss >> arg) {
		args.push_back(arg);
	}
	return args;
}

int contar_pipes(string comando) {
	int count = 0;
	for (int i = 0; i < comando.length(); i++) {
		if (comando[i] == '|') {
			count++;
		}
	}
	return count;
}

void exit() {
	cout << "\033[31m Saliendo de Shellcito\033[1;0m" << endl;
	sleep(1);
	system("clear");
}

void ejecutar_comando(string arg) {
	int c = INT_MAX;
	for (int i = 0; i < num_comandos; i++) {
		if (arg.compare(comandos[i]) == true) {
			c = i;
		}
	}
	switch (c) {
	case 0:
		exit();
		break;
	case 1:
		cout << "Este es tu comando favorito" << endl;
		break;
	case 4:
		cout << "Este es un comando de prueba" << endl;
		break;
	default:
		cout << "Comando no encontrado" << endl;
		break;
	}
}

void leer_comando() {
	string comando;
	vector<string> args;
	bool condicion = true;
	Fav favs;
	while (condicion) {

		cout << "\033[34m Shellcito\033[0m:\033[1;33m~\033[0m$ ";

		getline(cin, comando);
		if (comando.compare("exit") == false) {
			exit();
			break;
		}
		if (comando.empty()) {
			continue;
		}
		args = parsear_comandos(comando);
		//favs.add_fav(args[0]);

		int n = contar_pipes(comando);
		
		vector<const char*> arg;
        for (const string &arg : args) {
            arg.push_back(arg.c_str());
        }
        arg.push_back(nullptr); // Terminar el array con NULL
		// imprimir char *arg[]
		for (int i = 0; i < args.size(); i++) {
			cout << arg[i] << endl;
		}

		//imprimir arg 
		for(int i = 0; i < args.size(); i++){
			cout << args[i] << endl;
		}
		if (n == 0) { // No hay pipess
			if (fork() == 0) {
				if (execvp(arg[0], const_cast<char *const *>(arg.data())) == -1){
					cout << "Command not found" << endl;
				}
			} else {
				wait(NULL);
			}
		} else { // Multiples pipes
			while (n > 0) {
				int fd[2];
				pipe(fd);
				if (fork() == 0) {
					close(fd[0]);
					dup2(fd[1], 1);
					if (execvp(arg[0], arg) == -1){
					cout << "Command not found" << endl;
				}
				} else {
					close(fd[1]);
					dup2(fd[0], 0);
					if (execvp(arg[2], const_cast<char *const *>(arg)) == -1){
					cout << "Command not found" << endl;
				}
				}
				n--;
			}
		}

	}
}

int main() {
	vector<string> *favs = new vector<string>();
	iniciar_shell(favs);
	leer_comando();
	return 0;
}