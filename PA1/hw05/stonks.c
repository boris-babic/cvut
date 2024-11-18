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
        *array = (day *)realloc(*array, (*length *3/2 +2) * sizeof(*array[0]));
        *length = *length *3/2 +2;
    }
    (*array)[current.day] = current;
}

void print_array(day array[], int length)
{
    for (int i = 0; i <length; i++)
    {
        printf("%d:   %d %d\n", i,array[i].day, array[i].value);
    }
}

int compare(const void* a, const void* b) {
   return (*(int*)a - *(int*)b);
}

day * cut_array(day array[], int start, int end)
{
    day * result = (day *)calloc(end - start+1, sizeof(day));
    for (int i= start; i <= end; i++)
    {
        result[i-start] = array[i];
    }
    return result;
}

void print_results (int min, int max, int begin_max, int end_max, int begin_min, int end_min)
{
    if (max == 0) printf("Nejvyssi zisk: N/A\n");
    else printf("Nejvyssi zisk: %d (%d - %d)\n", max, begin_max, end_max);
    if (min == 0) printf("Nejvyssi ztrata: N/A\n");
    else printf("Nejvyssi ztrata: %d (%d - %d)\n", -min, begin_min, end_min);
}

int main()
{
    char action;
    day * array = (day *)calloc(10, sizeof(day));
    int length = 10;
    int start;
    int end;
    int value;
    int count = 0;

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
            if (scanf("%d", &value) != 1) goto fail;
            if (value < 0) goto fail;
            append(&array, &length, make_day(value, count));
            //print_array(array, length);
            count++;
        
        } else if(action == '?')
        {
            if (scanf("%d %d", &start, &end) != 2) goto fail;
            if (start > count -1 || end > count -1 || start > end || start < 0 || end < 0) goto fail;
            if (start == end) print_results(0,0,0,0,0,0);
            else
            {
                day max_value= array[start];
                day min_value= array[start];
                int max = 0, min = 0, max_start = 0, max_end = 0, min_start = 0, min_end = 0;
                for(int i = start+1; i <=end; i++)
                {
                    //max part
                    if (array[i].value -min_value.value > max)
                    {
                        max = array[i].value -min_value.value;
                        max_start = min_value.day;
                        max_end = array[i].day;
                    } //moves the start of calculations 
                    if (array[i].value < min_value.value) min_value = array[i];
                    //min part
                    if(array[i].value - max_value.value < min)
                    {
                        min = array[i].value - max_value.value;
                        min_start = max_value.day;
                        min_end = array[i].day;
                    } //moves the start of calculations 
                    if (array[i].value > max_value.value) max_value = array[i];
                }
                print_results(min, max, max_start, max_end, min_start, min_end);
            }
        } else
        {
            goto fail;
        }
        
    }
    free(array);
    return 0;
}