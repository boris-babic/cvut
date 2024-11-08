#include <stdio.h>
/*
0. make a datatype truck 
1. scan input and validate it
2. make an array of days 1 to max with capacity and price per day in it (maybe sum)
3. subtract start position from the sum until its good (binary search?)
4. return values
*/
const int MAX = 100000;

typedef struct 
{
    int start;
    int end;
    int capacity;
    int cost;
} Truck; 

Truck make_truck(int start, int end, int capacity, int cost)
{
    Truck vehicle;
    vehicle.start = start;
    vehicle.end = end;
    vehicle.capacity = capacity;
    vehicle.cost = cost;
    return vehicle;
}

void print_trucks( Truck array[], int size)
{
    for (int i = 0; i<size; i++)
    {
        printf("%d-%d, %d, %d \n", array[i].start, array[i].end, array[i].capacity, array[i].cost);
    }
}

int main()
{
    Truck array[MAX];
    Truck current;
    char bracket;
    int end = 0;
    int count = 0;
    printf("Moznosti dopravy:\n");
    if (scanf("%c", &bracket) != 1 && bracket != '{')
    {
        fail:
        printf("Nespravny vstup.\n");
        return 1;
    }
    while (scanf(" [ %d - %d , %d , %d ] ", &current.start, &current.end, &current.capacity, &current.cost) == 4)
    {
        array[count] = current;
        count ++;
        if (count > MAX) goto fail;
        if (scanf("%c", &bracket) == 1 && bracket == ',') continue;
        else if (bracket == '}') break;
        else goto fail;
    }
    print_trucks(array, count);
    printf("Naklad:\n");


    return 0;
}