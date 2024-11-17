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
            return 1;
        }
        if (action == '+')
        {
            if (scanf("%d", &value) != 1) goto fail;
            append(&array, &length, make_day(value, count));
            //print_array(array, length);
            count++;
        
        } else if(action == '?')
        {
            if (scanf("%d %d", &start, &end) != 2) goto fail;
            if (start > count -1 || end > count -1) goto fail;
            //qsort(array, length, sizeof(day), compare);
            //print_array(array, length);
            //int found_upper = 0;
            //int found_lower = 0;
            //int lower, upper;
            int min = 0;
            int max = 0;
            int current;
            int begin_max = 0;
            int begin_min = 0;
            int end_max = 0;
            int end_min = 0;
            for (int i = start; i <= end; i++)
            {
                for (int j = i; j <= end; j++)
                {
                    
                    current = array[j].value - array[i].value;
                    if (current > max) 
                    {
                        begin_max = array[i].day;
                        end_max = array[j].day;
                        max = current;
                    }
                    else if (current < min)
                    {
                        begin_min = array[i].day;
                        end_min = array[j].day;
                        min = current;
                    }
                    //printf("%d %d: %d %d %d\n", i, j, current, min, max);

                }
            }
            if (max == 0) printf("Nejvyssi zisk: N/A\n");
            else printf("Nejvyssi zisk: %d (%d - %d)\n", max, begin_max, end_max);
            if (min == 0) printf("Nejvyssi ztrata: N/A\n");
            else printf("Nejvyssi ztrata: %d (%d - %d)\n", -min, begin_min, end_min);
        } else
        {
            goto fail;
        }
        
    }
    return 0;
}


/*                //finding extreme
                if (array[i].day >= start && array[i].day <= end && array[i].value > 0 && found_lower == 0)
                {
                    lower = array[i].value;
                    printf("%d vosiel som\n", i);
                }
                if (array[length - i - 1 ].day >= start && array[length - i-1].day <= end && found_upper == 0) 
                {
                    upper= array[length - i-1].value;
                    printf("%d vosiel som inde\n", length - i-1);
                }
*/