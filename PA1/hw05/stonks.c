#include <stdio.h>
#include <stdlib.h>

typedef struct day
{
    int value;
    int day;
} day;

day make_day(int value, int order)
{
    day res;
    res.value = value;
    res.day = order;
    return res;
}

void append(day ** array, int *length, day current)
{
    if (current.day >= *length)
    {
        int new_length = (*length *3/2 +2);
        *array = (day *)realloc(*array, new_length * sizeof(day));
        *length = new_length;
    }
    (*array)[current.day] = current;
}

void print_results (int min, int max, int begin_max, int end_max, int begin_min, int end_min)
{
    if (max == 0) printf("Nejvyssi zisk: N/A\n");
    else printf("Nejvyssi zisk: %d (%d - %d)\n", max, begin_max, end_max);
    if (min == 0) printf("Nejvyssi ztrata: N/A\n");
    else printf("Nejvyssi ztrata: %d (%d - %d)\n", -min, begin_min, end_min);
}

void find_extremes(day * array, int start, int end, int * min, int * max, int * max_start, int * max_end, int * min_start, int  * min_end)
{
    day max_value= array[start];
    day min_value= array[start];
    for(int i = start+1; i <=end; i++)
    {
        //max part
        if (array[i].value - min_value.value > *max)
        {
            *max = array[i].value -min_value.value;
            *max_start = min_value.day;
            *max_end = array[i].day;
        }
        //min part
        if(array[i].value - max_value.value < *min)
        {
            *min = array[i].value - max_value.value;
            *min_start = max_value.day;
            *min_end = array[i].day;
        } //moves the start of calculations
        if (array[i].value < min_value.value) min_value = array[i];
        if (array[i].value > max_value.value) max_value = array[i];
    }
}

int main()
{
    char action;
    day * array = (day *)calloc(10, sizeof(day));
    int length = 10, start, end, value, count = 0;
    if (array == NULL)
    {
        printf("chyba");
        return 2;
    }
    printf("Ceny, hledani:\n");
    while (!feof(stdin))
    {
        if (feof(stdin)) break;
        if (scanf(" %c", &action) != 1)
        {
            if (feof(stdin)) break;
            fail:
            printf("Nespravny vstup.\n");
            free(array);
            return 1;
        }
        if (action == '+')
        {
            if (scanf("%d", &value) != 1 || value < 0) goto fail;
            append(&array, &length, make_day(value, count));
            count++;
        }
        else if(action == '?')
        {
            if (scanf("%d %d", &start, &end) != 2) goto fail;
            if (start > count -1 || end > count -1 || start > end || start < 0 || end < 0) goto fail;
            if (start == end) print_results(0,0,0,0,0,0);
            else
            {
                int max = 0, min = 0, max_start = 0, max_end = 0, min_start = 0, min_end = 0;
                find_extremes(array, start, end, &min, &max, &max_start, &max_end, &min_start, &min_end);
                print_results(min, max, max_start, max_end, min_start, min_end);
            }
        }
        else
        {
            goto fail;
        }
    }
    free(array);
    return 0;
}