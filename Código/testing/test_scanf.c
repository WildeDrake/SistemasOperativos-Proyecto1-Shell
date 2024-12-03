#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

	char *w;

	while (1) {
		printf(">");
		if (scanf("%m[^\n]%*c", &w) == 0) {
			scanf("%*c");
			printf("~");
			continue;
		}
		if (strcmp(w, "exit") == 0)
			break;
	}

	return 0;
}