#include <iostream>
#include <limits.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

// tenemos 1.1, 1.4, 1.6

const int num_comandos = 3;
const string comandos[1] = {"exit", "fav", "set"}; // set = set recordatorio

void iniciar_shell() {
	clear();
	cout << "Bienvenido a Shellcito ^-^" << endl;
}

string *parsear_comandos(string comando) {
	int count = 0;
	bool flag = false;
	for (int i = 0; i < comando.length(); i++) {
		if (comando[i] == ' ' && flag == false) {
			flag = true;
			count++;
		} else {
			flag = false;
		}
	}
	string *args = new string[count + 1];
	int i = 0;
	istringstream ss(comando);
	while (ss >> comando && i < count + 1) {
		args[i] = comando;
		i++;
	}
	return args;
}

void ejecutar_comando(string arg) {
	int c = INT_MAX;
	for (int i = 0; i < num_comandos; i++) {
		if (compare(arg, comandos[i]) == true) {
			c = i;
		}
	}
	switch (c) {
	case 0:
		exit(0);
		break;
	case 1:
		cout << "Este es tu comando favorito" << endl;
		break;
	default:
		cout << "Comando no encontrado" << endl;
		break;
	}
}

void leer_comando() {
	string comando;
	string *args;
	bool condicion = true while (condicion) {
		cout << "Shellcito:~$ " << ;
		getline(cin, comando);
		if (compare(comando, 'exit') == 0) {
			exit(0); // Idea: cambiar a condicion = false
		}
		if (comando.empty()) {
			continue;
		}

		args = parsear_comandos(comandos);

		if (fork() == 0) {
			execvp(args[0], args);
			printf("Command not found\n");
			exit(0);
		} else {
			wait(NULL);
		}
	}
}

void exit(int status) {
	cout << "Saliendo de Shellcito" << endl;
	exit(status);
}

int main() {
	iniciar_shell();
	leer_comando();
	return 0;
}