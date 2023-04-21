#include <stdio.h>

//double radius = 1.0
double circularArea(double r);

int main(){
	double radius = 1.0;
	double area = 0.0;
	printf(" Area of Circle\n\n");
	printf(" Radius      Area\n");
	printf(" ------------------\n");

	area = circularArea( radius );
	printf("%10.1f          %10.2f\n", radius, area);
        return 0;
}

double circularArea(double r){
	const double pi = 3.1415926536;
	return pi * r * r;

}



