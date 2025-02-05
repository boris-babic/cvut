#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/*
int main() {
    int added;
    for (int j = 0; j < 10; j++){
    for (int i =10 + j; i < 100; i+= 10) {
        added = (i / 10) * ((i / 10)+1);
        printf("%d: %d ---- %d  a rozdiel je: %d", i, i * i, added * 100 + i , - i * i + (added * 100 + i));
        if ( i * i == added * 100 + i) {
            printf("          rovnaju sa");
        }
        printf("\n");
    }
    printf("\n\n\n\n");
    }
}
*/
typedef struct Map {
    int width;
    int height;
    int ** data;
} Map;

typedef struct Plot {
    int x;
    int y;
    int price;
} Plot;

Map * create_map(int width, int height) {
    Map * result =(Map *)malloc(sizeof(Map));
    result->data = (int **)malloc(sizeof(int *) * width);
    for ( int i = 0; i < width; i++) {
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

void print_map(Map * array) {
    for (int j = 0; j < array->height; j++) {
        for (int i = 0; i < array->width; i++) {
            printf("(%02d/%02d----    %02d) ", i, j, array->data[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void free_map(Map * array) {
    for (int i = 0; i < array->width; i++) {
        free(array->data[i]);
    }
    free(array->data);
    free(array);
}

int fill_map(Map * array) {
    int value;
    for (int j = 0; j < array->height; j++) {
        for (int i = 0; i < array->width; i++) {
            if (scanf("%d", &value) != 1 || feof(stdin)){
                return 1;
            }
            array->data[i][j] = value;
        }
    }
    return 0;
}

Map * get_prefix_sum(Map * array) {
    Map * sums = create_map(array->width, array->height);
    for (int i = 0; i < sums->width; i++) {
        for (int j = 0; j < sums->height; j++) {
            //printf("%d %d\n", i, j);
            sums->data[i][j] = array->data[i][j];
            if( i >=1 && j >= 1){
                // adds sums from the field above and to the left and subtracts sum to the rightleft (cos its there twice)
                sums->data[i][j] += sums->data[i-1][j];
                sums->data[i][j] += sums->data[i][j-1];
                sums->data[i][j] -= sums->data[i-1][j-1];
            }else {
                if (i == 0 && j > 0) {
                    // only adds from the field above
                  sums->data[i][j] += sums->data[i][j-1];  
                }
                if (j == 0 && i > 0) {
                    //only adds from the field to the right
                    sums->data[i][j] += sums->data[i-1][j];  
                }
            }

        }
    }
    return sums;
}

int get_price(Map * sums, int x1, int y1, int x2, int y2) {
    //printf("ratam %d %d ----- %d %d\n", x1, y1, x2, y2);
    int result = sums->data[x2][y2];
    if (x1 >= 1 && y1 >= 1){
        result -= sums->data[x1-1][y2];
        result -= sums->data[x2][y1-1];
        result += sums->data[x1-1][y1-1];
    } else {
        if (x1 == 0 && y1 > 0) {
            result -= sums->data[x2][y1 -1];
        }
        if (y1 == 0 && x1 > 0) {
            result -= sums->data[x1-1][y2];
        }
    }
    return result;
}

void print_minimum(Plot * result, int length, int minimum) {
    printf("Nejlevnejsi: %d\n", minimum);
    for (int i = 0; i < length; i++) {
        if (result[i].price == minimum) printf(" * (%d,%d)\n", result[i].x, result[i].y); 
    }
}

int compare(const void *a, const void *b)
{
    return (((Plot *)a)->price - ((Plot *)b)->price);
}

void get_square_sum(Map * array, Map * sums, int size) {
    int all_combinations = (array->width - size + 1) * (array->height - size + 1); // ak by to bol tight fit tak nech nevznikne nula
    Plot * results = (Plot *)malloc(sizeof(Plot) * all_combinations);
    int index = 0;
    int minimum = INT_MAX;
    int current;
    for (int i = 0; i <= array->width - size; i++) {
        for (int j = 0; j <= array->height - size; j++) {
            current = get_price(sums, i, j, i + size - 1, j + size - 1);
            results[index] = create_plot(i, j, current); // -1 lebo povodny index obsahuje uz jednu dlzku
            if (current < minimum) minimum = current;
            index +=1;
        }
    }
    Plot * new_pointer = (Plot *)realloc(results, sizeof(Plot) * index);
    results = new_pointer;
    print_minimum(results, index, minimum);
    free(results);
    return;
}

void calculate_rectangles(Map * array, Map * sums, int owned_x, int owned_y, int size_x, int size_y) {
    int all_combinations = size_x * size_y;
    Plot * results = (Plot *)malloc(sizeof(Plot) * all_combinations);
    int index = 0;
    int minimum = INT_MAX;
    int current;
    for (int i = 0; i < size_x; i++) {
        if (owned_x - i < 0) break;
        for ( int j = 0; j < size_y; j++){
            if (owned_y - j < 0) break;
            current = get_price(sums, owned_x - i, owned_y - j, owned_x - i + size_x -1 , owned_y - j + size_y - 1);
            results[index] = create_plot(owned_x - i, owned_y - j, current);
            if (current < minimum) minimum = current; 
            index +=1;        
        }
    }
    Plot * new_pointer = (Plot *)realloc(results, sizeof(Plot) * index);
    results = new_pointer;
    print_minimum(results, index, current);
    free(results);
    return;
}

int main(){
    //printf("fungujem\n");
    int width = 0;
    int height = 0;
    printf("Mapa:\n");
    if (scanf("%d %d", &width, &height) != 2 || width < 1 || height < 1) { //nespravny vstup
        printf("Nespravny vstup\n");
        return 1;
    }
    Map * array = create_map(width, height);
    if(fill_map(array)) { //nespravny vstup 
        printf("Nespravny vstup\n");
        free_map(array);
        return 1;
    }
    if(feof(stdin)) { //nespravny vstup 
        printf("Nespravny vstup\n");
        free_map(array);
        return 1;
    }
    Map * sums = get_prefix_sum(array);
    
    //print_map(array);   
    //print_map(sums);

    //part for squares cenova mapa 1

    int size;
    printf("Pozemky:\n");
    while(!feof(stdin)){
        if(feof(stdin)) break;
        if(scanf("%d", &size) != 1 && !feof(stdin)){

            printf("Nespravny vstup\n");
            free_map(array);
            free_map(sums);
            return 1;
        }
        if (size <= 0){
            printf("Nespravny vstup\n");
            free_map(array);
            free_map(sums);
            return 1;
        }
        
        if (feof(stdin)) break;
        if (size > array->width || size > array->height) {
            
            printf("Neexistuje\n");
        } else {
            get_square_sum(array, sums, size);
        }
        if(feof(stdin)) break;
    }

    //part for rectangles and specific plot cenova mapa 2
    /*
    int x, y, size_x, size_y;
    while(!feof(stdin)){

        if(scanf("%d %d %d %d", &x, &y, &size_x, &size_y) != 4 && !feof(stdin)){
            printf("Nespravny vstup\n");
            free_map(array);
            free_map(sums);
            return 1;
        }
        if(feof(stdin)) break;
        if (size_x > array->width || size_y > array->height) {
            printf("Neexistuje\n");
        } else {
            calculate_rectangles(array, sums, x, y, size_x, size_y);
        }
    }
    */
    free_map(array);
    free_map(sums);
    return 0;
}