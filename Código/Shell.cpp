#include "favs.hpp"
#include <cstring>
#include <limits.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

// Color amarillo \033[1;33m
// Color azul \033[34m
// Color blanco \033[1;0m
// Imprime la bienvenida a la shell.
void iniciar_shell() {
	system("clear");
	cout << "\033[1;33m ~~~~~~~~~~~~~~~  ^-^\033[34m Bienvenido a Shellcito  \033[1;33m ^-^ "
			 "~~~~~~~~~~~~~~~ \033[1;0m\n"<< endl;
}

// Cuenta la cantidad de palabras en un comando separadas por " " o multiples " ".
int contar_palabras(char *comando) {
	int count = 0; 			// Contador de palabras.
	bool flag = false; 		// Bandera para saber si ya se leyo un espacio.
	for (int i = 0; i < strlen(comando); i++) { 
		if (comando[i] != ' ' && flag == false) { // Si no es un espacio y la bandera es falsa.
			count++;
			flag = true;
		} else { // Si es un espacio.
			flag = false;
		}
	}
	return count;
}

// Parsea los comandos segun los " ".
char **parsear_comandos(char *comando) {
	int count = contar_palabras(comando);		// Cantidad de palabras.
	char **args = (char **)malloc(sizeof(char *) * (count + 1)); // Arreglo de palabras.
	char *arg = strtok(comando, " "); // Tokenizacion de palabras.
	int i = 0;
	while (arg != NULL) { // Mientras haya palabras.
		args[i] = arg; // Se guarda la palabra en el arreglo.
		arg = strtok(NULL, " "); // Se sigue tokenizando.
		i++;
	}
	args[i] = NULL;
	return args;
}

// Cuenta la cantidad de pipes en un comando.
int contar_pipes(char *comando) {
	int count = 0; // Contador de pipes.
	for (int i = 0; i < strlen(comando); i++) { 
		if (comando[i] == '|') { // Si es un pipe.
			count++;
		}
	}
	return count;
}

// Parsea los comandos segun los pipes.
char ***parsear_pipes(char **args, int n) { // n = numero de pipes
	char ***comandos = (char ***)malloc(sizeof(char **) * (n + 1));  // Arreglo de comandos.
	int i = 1;
	int npipes = 1; // Numero de pipes.
	comandos[0] = args;
	while (args[i] != NULL) {
		if (strcmp(args[i], "|") == 0) { // Si es un pipe.
			args[i] = NULL;
			if (args[i + 1] != NULL) {
				comandos[npipes] = args + i + 1; // Se guarda el comando en el arreglo.
				npipes++;
			}
		}
		i++;
	}

	return comandos;
}

// Imprime un mensaje de salida y cierra la shell.
void exit() {
	cout << "\033[31m Saliendo de Shellcito\033[1;0m\n" << endl;
	sleep(1);
	system("clear");
}

// Crea un proceso hijo que se encarga de recordar un mensaje en un tiempo determinado.
void set_recordatorio(int segundos, char *mensaje) {
	cout << "Recordatorio en " << segundos << " segundos" << endl;
	pid_t pid = fork(); // Crear proceso hijo.
	if (pid == 0) {
		sleep(segundos); // Esperar los segundos.
		cout << "\n \032[1;33mRecordatorio: \033[1;0m \n" << mensaje << endl; // Imprimir mensaje.
		_exit(0); // Salir del proceso hijo.
	}
}

// Ejecuta los comandos internos de nuestra shell.
bool ejecutar_comando(char **args, Fav favs) {
	if (args[0] == NULL) {
		cout << "Comando no encontrado" << endl;
	}

	// favs.
	if (strcmp(args[0], "favs") == 0) {
		if (args[1] == NULL) {
			cout << "favs: Faltan argumentos" << endl;
			return true;
		}
		// favs crear.
		if (strcmp(args[1], "crear") == 0) {
			if (args[2] != NULL) {
				favs.crear(args[2]);
			} else {
				cout << "favs: Faltan argumentos" << endl;
			}
		// favs agregar.
		} else if (strcmp(args[1], "mostrar") == 0) {
			favs.print_favs();
		// favs eliminar.
		} else if (strcmp(args[1], "eliminar") == 0) {
			if (args[2] != NULL || args[3] != NULL) {
				favs.remove_fav(atoi(args[2]), atoi(args[3]));
			} else {
				cout << "favs: Faltan argumentos" << endl;
			}
		// favs buscar.
		} else if (strcmp(args[1], "buscar") == 0) {
			if (args[2] != NULL) {
				favs.scan_favs(args[2]);
			} else {
				cout << "favs: Faltan argumentos" << endl;
			}
		} else if (strcmp(args[1], "borrar") == 0) {
			favs.remove_favs();
		} else if (strcmp(args[1], "cargar") == 0) {
			favs.load_favs();
		} else if (strcmp(args[1], "guardar") == 0) {
			favs.save_favs();
		} else if (args[2] != NULL) {
			if (strcmp(args[2], "ejecutar") == 0) {

				// se reemplaza "ejecutar" con el comando por el favorito.
				args[2] = args[0];
				strcpy(args[2], favs.get_fav(atoi(args[1])).c_str());
				
				if (fork() == 0) {
					execvp(args[2], args + 2);
					cout << "favs: Comando no encontrado" << endl;
				}

			} else {
				cout << "favs: Comando no encontrado" << endl;
			}
		} else {
			cout << "favs: Comando no encontrado" << endl;
		}

	// set recordatorio.
	} else if (strcmp(args[0], "set") == 0) {
		if (args[1] == NULL) {
			cout << "set: Faltan argumentos" << endl;
			return false;
		}
		if (strcmp(args[1], "recordatorio") == 0) {
			if (args[2] == NULL) {
				cout << "set: Faltan argumentos" << endl;
				return false;
			}
			int segundos = atoi(args[2]);
			char mensaje[100];
			for (int i = 3; args[i] != NULL; i++) {
				strcat(mensaje, args[i]);
				strcat(mensaje, " ");
			}
			set_recordatorio(segundos, mensaje);
			return true;
		}
	} else {
		return false;
	}
	return true;
}

// Lee los comandos ingresados por el usuario.
void leer_comando() {
	char *comando;
	char **args;
	char ***comandos;
	bool condicion = true;
	Fav favs;
	while (condicion) {
		// prompt.
		cout << "\033[34m Shellcito\033[0m:\033[1;33m~\033[0m$ ";
		// Leer comando.
		if (scanf("%m[^\n]%*c", &comando) == 0) {
			scanf("%*c");
			continue;
		}
		// Eliminar espacios al inicio.
		while (comando[0] == ' ') {
			comando++;
		}
		// en caso de que el comando sea exit, se cierra la shell.
		if (strcmp(comando, "exit") == 0) {
			exit();
			break;
		}
		// Parsea comandos segun sus espacios.
		args = parsear_comandos(comando);
		
		// Si no existe un comando interno de nuestra shell, se ejecuta los comandos de UNIX.
		if (ejecutar_comando(args, favs) == false) {
			// Se cuentan los pipes.
			int n = contar_pipes(comando);
			// Se parsean los pipes.
			comandos = parsear_pipes(args, n);
			// Agregamos  el comando a favs en caso de que no se haya usado antes.
			for (int i = 0; i <= n; i++) {
				favs.add_fav(comandos[i][0]);
			}
			// Se crea un espacio para todos los file descriptors.
			int FDs[2 * n];
			// Se crean los procesos hijos dependiendo de la cantidad de pipes.
			for (int i = 0; i <= n; i++) { 
				if (i < n) { 
					pipe(FDs + 2 * i); // se abre una pipe en todas las iteraciones menos en la ultima.
				}
				if (fork() == 0) { // Proceso hijo.

					// redirección de la entrada (se omite el primer comando)
					if (i > 0) { 
						dup2(FDs[2 * i - 2], STDIN_FILENO);
						close(FDs[2 * i - 2]);
						close(FDs[2 * i - 1]);
					}

					// redirección de la salida (se omite el último comando)
					if (i < n) {
						dup2(FDs[2 * i + 1], STDOUT_FILENO);
						close(FDs[2 * i]);
						close(FDs[2 * i + 1]);
					}

					// Ejecutar comando.
					execvp(comandos[i][0], comandos[i]);

					// En caso de que la ejecución falle.
					favs.remove_fav(comandos[i][0]);
					cout << "Comando no encontrado" << endl;
					return;
				}
				if (i > 0) {
					// cerrar los file descriptors que no se usan.
					close(FDs[2 * i - 2]);
					close(FDs[2 * i - 1]);
				}
				wait(NULL); // Esperar a que termine el proceso hijo.
			}
		}
	}
}

int main() {
	iniciar_shell(); // Imprime la bienvenida a la Shell.
	leer_comando(); // Lee los comandos ingresados por el usuario, hace toda la magia.
	return 0;
}