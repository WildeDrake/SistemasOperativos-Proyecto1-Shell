#include "favs.cpp"
#include <cstring>
#include <limits.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

const int num_comandos = 4;
const char *comandos[4] = {"exit", "fav", "set", "test"}; // set = set recordatorio

// Color amarillo \033[1;33m
// Color azul \033[34m
// Color blanco \033[1;0m
void iniciar_shell() {
	system("clear");
	printf("\033[1;33m ~~~~~~~~~~~~~~~  ^-^\033[34m Bienvenido a Shellcito  \033[1;33m ^-^ "
			 "~~~~~~~~~~~~~~~ \033[1;0m\n");
}

void set_recordatorio(int segundos, string mensaje) {
	pid_t pid = fork();
	if (pid == 0) {
		sleep(segundos);
		cout << "\033[1;33mRecordatorio: %s\033[1;0m\n" << mensaje << endl;
		_exit(0);
	}
}

int contar_palabras(char *comando) {
	int count = 0;
	bool flag = false;
	for (int i = 0; i < strlen(comando); i++) {
		if (comando[i] != ' ' && flag == false) {
			count++;
			flag = true;
		} else {
			flag = false;
		}
	}
	return count;
}

char **parsear_comandos(char *comando) {
	int count = contar_palabras(comando);
	char **args = (char **)malloc(sizeof(char *) * (count + 1));
	char *arg = strtok(comando, " ");
	int i = 0;
	while (arg != NULL) {
		args[i] = arg;
		arg = strtok(NULL, " ");
		i++;
	}
	args[i] = NULL;
	return args;
}

int contar_pipes(char *comando) {
	int count = 0;
	for (int i = 0; i < strlen(comando); i++) {
		if (comando[i] == '|') {
			count++;
		}
	}
	return count;
}

char ***parsear_pipes(char **args, int n) { // n = numero de pipes

	char ***comandos = (char ***)malloc(sizeof(char **) * (n + 1));

	int i = 1;
	int npipes = 1;
	comandos[0] = args;
	while (args[i] != NULL) {
		if (strcmp(args[i], "|") == 0) {
			args[i] = NULL;
			if (args[i + 1] != NULL) {
				comandos[npipes] = args + i + 1;
				npipes++;
			}
		}
		i++;
	}

	return comandos;
}

void exit() {
	printf("\033[31m Saliendo de Shellcito\033[1;0m\n");
	sleep(1);
	system("clear");
}

void ejecutar_comando(char *arg) {
	int c = INT_MAX;
	for (int i = 0; i < num_comandos; i++) {
		if (strcmp(arg, comandos[i]) == 0) {
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
	char *comando;
	char **args;
	char ***comandos;
	bool condicion = true;
	Fav favs;
	while (condicion) {
		cout << "\033[34m Shellcito\033[0m:\033[1;33m~\033[0m$ ";

		if (scanf("%m[^\n]%*c", &comando) == 0) {
			scanf("%*c");
			continue;
		}

		while (comando[0] == ' ') { // Eliminar espacios al inicio
			comando++;
		}

		if (strcmp(comando, "exit") == 0) {
			exit();
			break;
		}

		int n = contar_pipes(comando);
		printf("Pipes %d\n", n);

		args = parsear_comandos(comando);
		comandos = parsear_pipes(args, n);

		for (int i = 0; i < n; i++) {
			favs.add_fav(comandos[i][0]);
		}

		/*vector<const char *> arg;
		n =  // largo de args
		for (int i = 0; i < n; i++) {
			arg.push_back(arg.c_str()); //  strcat(arg, arg.c_str());?
		}
		strcat(arg, nullptr); // Terminar el array con NULL //arg.push_back(nullptr)
		// imprimir char *arg[]
		for (int i = 0; i < args.size(); i++) {
			cout << arg[i] << endl;
		}

		// imprimir arg
		for (int i = 0; i < args.size(); i++) {
			cout << args[i] << endl;
		}*/

		/*if (n == 0) { // No hay pipes
			if (fork() == 0) {
				int sex = 696969;
				sex = execvp(comandos[0][0], comandos[0]);
				cout << "Comando no encontrado" << endl;
				favs.remove_fav(comandos[0][0]);
				return;
			} else {
				wait(NULL);
			}
		} else { // Multiples pipes
			for (int i = 0; i < n; i++) {
				int fd[2];
				pipe(fd);
				if (fork() == 0) {
					if (i != 0) {
						dup2(fd[0], STDIN_FILENO);
					}
					if (i != n - 1) {
						dup2(fd[1], STDOUT_FILENO);
					}
					close(fd[0]);
					close(fd[1]);
					execvp(comandos[i][0], comandos[i]);
					cout << "Comando no encontrado" << endl;
					favs.remove_fav(comandos[i][0]);
				} else {
					close(fd[0]);
					close(fd[1]);
					wait(NULL);
				}
			}
		}*/

		int FDs[2 * n];
		for (int i = 0; i <= n; i++) {
			if (i < n) {
				pipe(FDs + 2 * i);
			}
			if (fork() == 0) {
				if (i > 0) {
					dup2(FDs[2 * i - 2], STDIN_FILENO);
					close(FDs[2 * i - 2]);
					close(FDs[2 * i - 1]);
				}
				if (i < n) {
					dup2(FDs[2 * i + 1], STDOUT_FILENO);
					close(FDs[2 * i]);
					close(FDs[2 * i + 1]);
				}
				execvp(comandos[i][0], comandos[i]);
				cout << "Comando no encontrado" << endl;
				return;
			}
			if (i > 0) {
				close(FDs[2 * i - 2]);
				close(FDs[2 * i - 1]);
			}
			wait(NULL);
		}
	}
}

int main() {
	iniciar_shell();
	leer_comando();
	return 0;
}