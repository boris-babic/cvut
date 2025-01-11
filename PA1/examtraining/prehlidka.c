#include <stdlib.h>
#include <stdio.h>

typedef struct data {
    int length;
    int * array;
} data;

void print_list(data list) {
    for (int i = 0; i < list.length; i++){
        printf("(%d):%d \n", i,list.array[i]);
    }
}

void append(data * lanes, int width, int order) {
    if (order >= lanes->length) {
        lanes->length = lanes->length * 3 / 2;
        int * new_data = (int *)realloc(lanes->array, lanes->length * sizeof(int));
        lanes->array = new_data;
    }
    lanes->array[order] = width;
}

long long int gcd(long long int a, long long int b) {
    if (b > a) {
        long long int temp = b;
        b = a;
        a = temp;
    }
    long long int remainder;
    while (b) {
        remainder = a % b;
        a = b;
        b = remainder;
    }
    return a;
}

long long int lcm(long long int a, long long int b) {
    return a/gcd(a,b)*b;
}
long long int find_lcm(data * lanes, int from, int to) {
    long long int result = lanes->array[from];
    //printf("pociatocny: %lld\n", result);
    for (int i = from; i < to; i++) {
        result = lcm(result, lanes->array[i]);
        //printf("result after %d is %lld\n", i+1, result);
    }
    return result;
}


int main() {
    printf("Pocty pruhu:\n");
    char bracket;
    scanf(" %c",  &bracket);
    if (bracket != '{') {
        printf("Nespravny vstup.\n");
        return 1;
    }
    int width;
    int order = 0;

    data lanes;
    lanes.array = (int *)malloc(10 * sizeof(int));
    lanes.length = 10;
    char comma;
    while (!feof(stdin)) {
        if(scanf(" %d", &width) != 1 && !feof(stdin)) {
            printf("Nespravny vstup.\n");
            free(lanes.array);
            return 1;
        }
        append(&lanes, width, order);
        order +=1;
        scanf(" %c", &comma);
        if (comma == ',') continue;
        else if (comma == '}') break;
        else {
            printf("Nespravny vstup.\n");
            free(lanes.array);
            return 1;
        }
    }
    int * new_data = (int *)realloc(lanes.array, sizeof(int) * order);
    lanes.array = new_data;
    lanes.length = order;
    //print_list(lanes);
    printf("Trasy:\n");
    int from, to;
    while (!feof(stdin)) {
        if(scanf("%d %d", &from, &to) != 2 || from >= to || from < 0 || to > lanes.length) {
            printf("Nespravny vstup.\n");
            free(lanes.array);
            return 1;
        }
        long long int result = find_lcm(&lanes, from, to);
        printf("Vozidel: %lld\n", result);
    }




    free(lanes.array);
    return 0;
}