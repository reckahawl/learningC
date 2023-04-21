#include <stdio.h>

int main(int argc, char *argv[]){
	int i = 0;
	for (i=1; i<argc; i++){
		printf("%d: %s\n", i, argv[i]);}
	char *states[] = {"nairobi","Kisumu","Mombasa","\0"};
	int num = 4;
	for (i=0; i<num; i++){
		printf("%d: %s\n", i, states[i]);}
	return 0;
}


