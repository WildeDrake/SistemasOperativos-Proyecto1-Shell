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

char **parsear_comandos(char *comando) {
	int count = 0;
	int flag = 0;
	for (int i = 0; i < strlen(comando); i++) {
		if (comando[i] == ' ' && flag == 0) {
			count++;
		} else {
			flag = 1;
		}
	}
	char **args = (char **)malloc(sizeof(char *) * (count + 1));
	char *arg = strtok(comando, " ");
	int i = 0;
	do {
		args[i] = arg;
		arg = strtok(NULL, " ");
		i++;
	} while (arg != NULL);
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

char ***separar_pipes(char **args, int n) { // n = numero de pipes

	char ***comandos = (char ***)malloc(sizeof(char **) * n);

	int i = 0;
	while (args[i] != NULL) {
		if (strcmp(args[i], "|") == 0) {
			args[i] = NULL;
			if (args[i + 1] != NULL) {
				comandos[i] = args + i + 1;
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
	char *comando = (char *)malloc(10000);
	// char **args;
	char ***comandos;
	bool condicion = true;
	Fav favs;
	while (condicion) {
		cout << "\033[34m Shellcito\033[0m:\033[1;33m~\033[0m$ ";
		scanf("%[^\n]", comando);
		cout  << "1|" << comando << "|1" << endl;
		if (strcmp(comando, "exit") == 0) {
			exit();
			break;
		}
		cout  << "2|" << comando << "|2" << endl;
		if (strlen(comando) != 0) {

			int n = contar_pipes(comando);

			// args = parsear_comandos(comando);
			comandos = separar_pipes(parsear_comandos(comando), n);

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

			if (n == 0) { // No hay pipes
				if (fork() == 0) {
					execvp(comandos[0][0], comandos[0]);
					cout << "Comando no encontrado" << endl;
					favs.remove_fav(comandos[0][0]);
				} else {
					wait(NULL);
				}
			} else { // Multiples pipes
						/*for (int i = 0; i < n; i++) {
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
						}*/
			}
		}
	}
}

int main() {
	iniciar_shell();
	leer_comando();
	return 0;
}