#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
/*
1. create map
2. fill map
3. prefix sum
4. while loop with inputs
5. calculate sums
6. print
*/

typedef struct Map {
    int ** data;
    int width;
    int height;
} Map;

typedef struct Plot {
    int x;
    int y;
    int price;
} Plot; 

void print_map(Map * array) {
    for (int j = 0; j < array->height; j++) {
        for (int i = 0; i < array->width; i++) {
            printf("(%02d/%02d----    %02d) ", i, j, array->data[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
Map * create_map(int width, int height) {
    Map * result = (Map *)malloc(sizeof(Map));
    result->data = (int **)malloc(sizeof(int *) * width);
    for (int i = 0; i < width; i++) {
        result->data[i] = (int *)malloc(sizeof(int) * height);
    }
    result->height = height;
    result->width = width;
    return result;
}

Plot create_plot(int x, int y, int price) {
    Plot result;
    result.x = x;
    result.y = y;
    result.price = price;
    return result;
}

int fill_map(Map * array) {
    int value;
    for (int j = 0; j < array->height; j++) {
        for (int i = 0; i < array->width; i++) {
            if (scanf("%d", &value) != 1) return 1;
            else array->data[i][j] = value;
        }
    }
    return 0;
}

void free_map(Map * array) {
    for (int i = 0; i < array->width; i++) {
        free(array->data[i]);
    }
    free(array->data);
    free(array);
    return;
}

Map * get_prefix_sum(Map * array) {
    Map * result = create_map(array->width, array->height);
    for (int i = 0; i < array->width; i++) {
        for (int j = 0; j < array->height; j++) {
            result->data[i][j] = array->data[i][j];
            if (i > 0 && j > 0) {
                result->data[i][j] += result->data[i-1][j];
                result->data[i][j] += result->data[i][j-1];
                result->data[i][j] -= result->data[i-1][j-1];
            } else {
                if (i == 0 && j > 0) {
                result->data[i][j] += result->data[i][j-1];
                }
                if (j == 0 && i > 0) {
                    result->data[i][j] += result->data[i-1][j];
                }
            }
        }
    }
    return result;
}


int get_price(Map * sums, int x1, int y1, int x2, int y2) {
    int result = sums->data[x2][y2];
    if (x1 > 0 && y1 > 0) {
        result -= sums->data[x1-1][y2];
        result -= sums->data[x2][y1-1];
        result += sums->data[x1-1][y1-1];
    } else if (x1 == 0 && y1 > 0) {
        result -= sums->data[x2][y1-1];
    } else if (x1 > 0 && y1 == 0) {
        result -= sums->data[x1-1][y2];
    }
    return result;
}

void calculate_sum(Map * prefix_sum, int size){
    int minimum = INT_MAX;
    int index = 0;
    int current_price;
    int possible_combinations = (prefix_sum->width - size + 1) * (prefix_sum->height - size + 1);
    Plot * results = (Plot *)malloc(sizeof(Plot) * possible_combinations);
    for (int i = 0; i <= prefix_sum->width - size; i++) {
        for (int j = 0; j <= prefix_sum->height - size; j++) {
            current_price = get_price(prefix_sum, i, j, i + size - 1, j + size - 1);
            //printf("(%d %d) %d \n", i, j, current_price);
            results[index] = create_plot(i, j, current_price);
            index +=1;
            if (current_price < minimum) {
                minimum = current_price;
            }
        }
    }
    Plot * new_result = (Plot *)realloc(results, sizeof(Plot) * index);
    results = new_result;
    printf("Nejlevnejsi: %d\n", minimum);
    for (int i = 0; i < index; i++) {
        if (results[i].price == minimum) {
            printf(" * (%d,%d)\n", results[i].x, results[i].y);
        }
    }
    free(results);
    return;
}


int main() {
    int width = 0;
    int height = 0;

    printf("Mapa:\n");
    if(scanf("%d %d", &width, &height) != 2 || width < 1 || height < 1){
        printf("Nespravny vstup.\n");
    }
    Map * array = create_map(width, height);


    if (fill_map(array) || feof(stdin) ) {
        printf("Nespravny vstup\n");
        free_map(array);
        return 1;
    }
    Map * prefix_sums = get_prefix_sum(array);
    //print_map(array);
    //print_map(prefix_sums);

    printf("Pozemky:\n");
    int side;
    while(!feof(stdin)) {
        if ( (scanf("%d", &side) != 1 && !feof(stdin))  || (side < 1)  ){
            printf("Nespravny vstup\n");
            free_map(array);
            free_map(prefix_sums);
            return 1;
        }
        if (feof(stdin)) break;
        if (side > array->width || side > array->height ){
            printf("Neexistuje.\n");
        }
        else {
            calculate_sum(prefix_sums,  side);
        }

    }
    free_map(array);
    free_map(prefix_sums);
    return 0;
}