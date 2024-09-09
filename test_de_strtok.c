#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char** parsear_comandos(char* comando) {
	int count = 0;
	int flag = 0; 
	for(int i = 0; i < strlen(comando); i++){
		if(comando[i] == ' ' && flag == 0){
			count++;
		} else {
			flag = 1;
		}
	}
	char** args = (char**)malloc(sizeof(char*) * (count + 1));
	char* arg = strtok(comando, " ");
	int i = 0;
	while(arg != NULL) {
		args[i] = arg;
		arg = strtok(NULL, " ");
		i++;
	}
	args[i] = NULL;
	return args;
}

int main() {

    char *sex;
    char **sexo;

    scanf("%m[^\n]", &sex);
    printf("%s\n", sex);

    sexo = parsear_comandos(sex);
    int i = 0;
    while(sexo[i] != NULL) {
        printf("%s\n", sexo[i]);
        i++;
    }

    return 0;
}