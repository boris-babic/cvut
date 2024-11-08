#include <stdio.h>
#include <math.h>
#include <float.h>
/*
0.Find if the points are colinear
1.Find Midpoint
2.Find vector Point-Midpoint
3.Add the vector to midpoint
4.Find what shape it is
*/
typedef struct {
	double x;
	double y;
} Point;

Point getVector(Point a, Point b) {
	Point v;
	v.x = b.x - a.x;
	v.y = b.y - a.y;
	return v;
}

int isEqual(double a, double b) {
	if (fabs(a - b)< DBL_EPSILON * 100 * (fmax(fabs(a),fabs(b)))) {
		return 1;
	} else {
		return 0;
	}
}

int checkIfColinear (Point a, Point b, Point c) {
	Point e = getVector(a, b);
	Point f = getVector(b, c);
	if (isEqual(e.x * f.y, e.y * f.x)) {
		return 1;
	} else {
		return 0;
	}
}

Point getMidpoint(Point a, Point b) {
	Point m;
	m.x = (a.x + b.x)/2;
	m.y = (a.y + b.y)/2;
	return m;
}

double getDistance(Point a) {
	double distance = sqrt(a.x*a.x + a.y*a.y);
	return distance;
}	

Point getPoint(Point a, Point b, Point c){
	Point m = getMidpoint(b, c);
	Point v = getVector(a, m);
	Point d;
	d.x = m.x + v.x;
	d.y = m.y + v.y;
	return d;
	
}

int getName(Point a, Point b, Point c, Point d){
	double side1 = getDistance(getVector(a, b));
	double side2 = getDistance(getVector(a, c));
	double diagonal1 = getDistance(getVector(a, d));
	double diagonal2 = getDistance(getVector(b, c));
	int result = 1;
	if(isEqual(side1, side2)) {
		result*=2;
	}
	if(isEqual(diagonal1, diagonal2)){
		result*=3;
	}
	if (result == 1) {
		printf("rovnobeznik\n");
	} else if(result == 2) {
		printf("kosoctverec\n");
	} else if(result == 3) {
		printf("obdelnik\n");	
	} else {
		printf("ctverec\n");	
	}
	return 0;
}	

int main(){
	Point points[3];
	char character;
	for (int i = 0; i < 3; i++){
		printf("Bod %c:\n",(i+'A'));
		if (scanf(" [ %lf , %lf %c", &points[i].x, &points[i].y, &character) != 3 || character != ']'){
			printf("Nespravny vstup.\n");
			return 1;
		}
	}
	if (checkIfColinear(points[0], points[1], points[2])) {
		printf("Rovnobezniky nelze sestrojit.\n");
		return 2;
	}
	for (int i = 0; i < 3; i++){
		Point d = getPoint(points[(i)%3], points[(i+1)%3], points[(i+2)%3]);
		printf("%c\': [%g,%g], ",(i+'A'), d.x, d.y);
		getName(points[i], points[(i+1)%3], points[(i+2)%3], d);
	}
	return 0;
}
