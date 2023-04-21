#include <stdio.h>

int main(int argc, char *argv[]){
int i  = 0;
if (argc == 1){
	printf("you dump ass have one arg only.\n");
}else if(argc > 1 && argc < 4){
	for (i=0; i < argc; i++){
		printf("%s", argv[i]);}
}else{
	printf("you suck with too much argument");				}
	return 0;
}



