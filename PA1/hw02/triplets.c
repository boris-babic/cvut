#include <stdio.h>
#include <math.h>
/*
1. take a random point from the interval
2. square it 
3. compute side c
3. check if in interval
4. print
*/
typedef struct {
	int a;
	int b;
	int c;
} Triplet;

int check_in_range(Triplet x, int min, int max) {
	if (x.a <= max && x.b <= max && x.c <= max && x.a >= min && x.b >= min && x.c >= min){
		return 1;
	} else {
		return 0;
	}
}

int gcd(int a, int b) {
	while (b != 0) {
		int temp = b;
		b = a % b;
		a = temp;
	}
	return a;
}

Triplet getting_triplet(int m, int n){
	Triplet x;
	x.a = n*n -m*m;
	x.b = 2*m*n;
	x.c = m*m+n*n;
	return x;
}

Triplet multiplying (Triplet x, int scalar){
	Triplet result;
	result.a = x.a * scalar;
	result.b = x.b * scalar;
	result.c = x.c * scalar;
	return result;
}


int main() {
	char symbol;
	char bracket;
	int min, max;
	int koniec = 0;
	Triplet current;
	printf("Problemy:\n");
	while (scanf("%c < %d ; %d %c ", &symbol, &min, &max, &bracket) == 4 && min<=max && min>0 && bracket == '>') {
		koniec = feof(stdin);
		//printf("koniec %d\n", koniec);
		int count = 0;
		int limit = sqrt(max)+1;
		if (symbol =='?') {
			for (int i = 1; i < limit; i++){
				for (int j = i+1; j < limit; j++) {		
					//printf("currently working on i=%d, j=%d\n", i,j);
					current = getting_triplet(i, j);
					//printf("current triple: %d, %d, %d\n", current.a, current.b, current.c);
					if (gcd(gcd(current.a, current.b),current.c)!=1){
						continue;
					} else if (check_in_range(current, min, max)){
						printf("* %d %d %d\n", current.a, current.b, current.c);
						count++;
					}
					if (current.c<max) {
						int active = 1;
						int k = 1;
						Triplet multiple;
						while (active) {
							//printf("nasobim \n");
							k++;
							multiple = multiplying(current, k);
							//printf("current triple: %d, %d, %d\n", multiple.a, multiple.b, multiple.c);
							if (multiple.c > max){
								active = 0;
							} else if (check_in_range(multiple, min, max)){
								printf("* %d %d %d\n", multiple.a, multiple.b, multiple.c);							
								count++;
							}
						}
					} else {
						break;
					}
					

				}
				
			}
			printf("Celkem: %d\n", count);
		} else if (symbol == '#') {
			for (int i = 1; i < limit; i++){
				for (int j = i+1; j < limit; j++) {		
					//printf("currently working on i=%d, j=%d\n", i,j);
					current = getting_triplet(i, j);
					//printf("current triple: %d, %d, %d\n", current.a, current.b, current.c);
					if (gcd(gcd(current.a, current.b),current.c)!=1){
						continue;
					} else if (check_in_range(current, min, max)){
						//printf("* %d %d %d\n", current.a, current.b, current.c);
						count++;
					}
					if (current.c<max) {
						int active = 1;
						int k = 1;
						Triplet multiple;
						while (active) {
							//printf("nasobim \n");
							k++;
							multiple = multiplying(current, k);
							//printf("current triple: %d, %d, %d\n", multiple.a, multiple.b, multiple.c);
							if (multiple.c > max){
								active = 0;
							} else if (check_in_range(multiple, min, max)){
								//printf("* %d %d %d\n", multiple.a, multiple.b, multiple.c);							
								count++;
							}
						}
					} else {
						break;
					}
				}
			}
			printf("Celkem: %d\n", count);
		} else {
			printf("Nespravny vstup.\n");
		}

	}
	if (koniec==0) {
		printf("Nespravny vstup.\n");
	}
	return 0;
}
