#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

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

int main() {

    char *sex;
    char **sexo;
	char ***sexos;

    scanf("%m[^\n]", &sex);
    printf("%s\n", sex);
	int n = contar_pipes(sex);

    sexo = parsear_comandos(sex);
    int i = 0;
    while(sexo[i] != NULL) {
        printf("%s\n", sexo[i]);
        i++;
    }

	sexos = parsear_pipes(sexo, n);
	for(int i = 0; i <= n; i++) {
		int j = 0;
		while(sexos[i][j] != NULL) {
			printf("%s ", sexos[i][j]);
			j++;
		}
		printf("\n");
	}

    return 0;
}