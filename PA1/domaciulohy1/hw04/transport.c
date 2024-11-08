#include <stdio.h>
#include <stdlib.h>
/*
0. make a datatype truck 
1. scan input and validate it
2. make an array of days 1 to max with capacity and price per day in it (maybe sum)
3. subtract start position from the sum until its good (binary search?)
4. return values
*/


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
        printf("%d: %d-%d, %d, %d \n",i, array[i].start, array[i].end, array[i].capacity, array[i].cost);
    }
}
int validate_truck(Truck truck) //returns 1 if valid, else 0
{
    if (truck.start < 0 || truck.end <0 || truck.start > truck.end || truck.capacity <= 0 || truck.cost <= 0)return 0;
    else return 1; 
}

typedef struct 
{
    int start;
    int amount;
} Order; 
Order make_order(int start, int amount)
{
    Order result;
    result.start = start;
    result.amount = amount;
    return result;
}

int compare(const void *a, const void *b)
{
    return (((Truck *)a)->start - ((Truck *)b)->start);
}
void sort_array(Truck array[], int size)
{
    qsort(array, size, sizeof(array[0]), compare);
}
/*
int bin_search(Truck array[], int size, int value)
{
    int index;
    int lower = 0;
    int upper = size;
    while (lower <= upper)
    {
        int mid = lower + (upper - lower) /2;
        if (array[mid].start <= value)
        {
            index = mid;
            lower = mid +1;
        } else
        {
            upper = mid -1;
        }
    }
    return index;
}
*/


int main()
{
    //initializing
    const int MAX = 100000;
    Truck array[100000] = {make_truck(0, 0, 0, 0)};
    Truck current;
    Order current_input;
    char bracket;
    int count = 0;
    int last_day = 0;
    printf("Moznosti dopravy:\n");
    if (scanf("%c", &bracket) != 1 && bracket != '{')
    {
        fail:
        printf("Nespravny vstup.\n");
        return 1;
    }
    //inputing trucks
    while (scanf(" [ %d - %d , %d , %d ] ", &current.start, &current.end, &current.capacity, &current.cost) == 4)
    {
        if (validate_truck(current))
        {
            array[count] = current;
            if (current.end > last_day) last_day = current.end;
            count ++;
        } else goto fail;
        //breaking points
        if (count > MAX) goto fail;
        if (scanf(" %c", &bracket) == 1 && bracket == ',') continue;
        else if (bracket == '}') break;
        else goto fail;
    }
    //print_trucks(array, MAX);
    //printf("-------------------------------------------------------------\n");
    sort_array(array, MAX);
    //print_trucks(array, MAX);
    //printf("%d\n", count);
    //inputing what we want to calculate
    //index = bin_search(array, MAX, 17);
    //printf("%d\n", index);
    printf("Naklad:\n");
    while(!feof(stdin))
    {
        //printf("-----------------------------------------------\n");
        if (scanf("%d %d", &current_input.start, &current_input.amount) != 2 && !feof(stdin)) goto fail;
        if(feof(stdin)) continue;
        if (current_input.start < 0 || current_input.amount <= 0) goto fail;
        int day = current_input.start;
        int total_cost = 0;
        int items_left = current_input.amount;
        int active = 1;
        while (active)
        {
            //printf("den %d\n", day);
            for (int i = 0; i < MAX; i++)
            {
                if (day >= array[i].start && day <= array[i].end)
                {
                    total_cost += array[i].cost;
                    items_left -= array[i].capacity;
                    //printf("total cena je %d a zostava %d\n", total_cost, items_left);
                }
            }
            if (items_left <=0)
            {
                printf("Konec: %d, cena: %d\n", day, total_cost);
                active = 0;
            }
            if (day > last_day)
            {
                active = 0;
                printf("Prilis velky naklad, nelze odvezt.\n");
            }
            day ++;

        }

    }
    
    return 0;
}