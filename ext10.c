#include <stdio.h>

int
main(int argc, char *argv[]){
	int num[4] = {0};
	char name[4] = {'a'};
	printf("numbers %d %d %d %d\n", num[0],num[1],num[2],num[3]);
	printf("name each: %c %c %c %c\n", name[0],name[1],name[2],name[3]);
	printf("name %s\n", name);
	num[0] = 1;
	num[1] = 2;
	num[2] = 3;
	num[3] = 4;

	name[0] = 'D';
	name[1] =  'e';
	name[2] = 'l';
	name[3] = 'w';
	printf("num %d %d %d %d\n",num[0],num[1],num[2],num[3]);

	printf("name each: %c %c %c %c\n",name[0],name[1], name[2],name[3]);

// print thename like a string

	printf("name %s", name);
// anotherway to use name

	char *another = "Zed";

	printf("another %s\n", another);

	printf("another each: %c %c %c %c\n",another[0],another[1],another[2],another[3]);



	return 0;
}
