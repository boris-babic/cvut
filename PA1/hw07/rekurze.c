#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
1.dorobit nacitanie jednotlivych cisel na konecny check
2.dorobit input check

*/


char * remove_char(char * string, int length, char c)
{
    char * result = (char *)calloc(length, sizeof(char));
    int index = 0;
    int write_index = 0;
    while(string[index])
    {
        if (string[index] != c)
        {
            result[write_index] = string[index];
            write_index++;
        }
        index++;
    }
    result[index] = '\0';
    return result;
}

void print_string(char * input, int length)
{
    for (int i = 0; i < length; i++)
    {
        printf("<---(%c):%03d--->", input[i], input[i]);
    }
    printf("\n");
}

int compare_chars(char * input_a, char * input_b)
{
    int length_a = strlen(input_a);
    int length_b = strlen(input_b);
    int max;
    if(length_a < length_b) max = length_b;
    else max = length_a;
    char * a = (char *)calloc(max+1, sizeof(char));
    char * b = (char *)calloc(max+1, sizeof(char));
    for (int i = 0; i < max - length_a; i++)
    {
        a[i] = '0';
    }
    for (int i = 0; i < max - length_b; i++)
    {
        b[i] = '0';
    }
    for (int i = 0; i < length_a; i++)
    {
        a[max - length_a + i] = input_a[i];
    }
    for (int i = 0; i < length_b; i++)
    {
        b[max - length_b + i] = input_b[i];
    }

    for (int i = 0; i < max; i++)
    {
        if (a[i] < b[i])
        {
            free(a);
            free(b);
            return 1;
        } else if(a[i] > b[i])
        {
            free(a);
            free(b);
            return 0;
        }
    }
    free(a);
    free(b);
    return 1;
}

int is_even(char * number)
{
    if (number[strlen(number)-1] == '0' ||
        number[strlen(number)-1] == '2' ||
        number[strlen(number)-1] == '4' ||
        number[strlen(number)-1] == '6' ||
        number[strlen(number)-1] == '8') return 1;
    else return 0;
}
int check_if_valid(char * data, int length)
{
    char * pt1;
    char * pt2;
    int result = 1;

    pt1 = strtok(data, ",");
    pt2 = strtok(NULL, ",");
    
    if( !pt2)
    {
        //printf("a1\n");
        return 1;
    }
    //printf("\n");
    //print_string(pt1, strlen(pt1)+1);
    //print_string(pt2, strlen(pt2)+1);

    if(!is_even(pt1)) result = 1;
    else result = compare_chars(pt1, pt2);
    if(!result)
    {
        //printf("a2\n");
        return 0;
    }

    pt1 = pt2;
    while ((pt2 = strtok(NULL, ",")))
    {
        //print_string(pt2, strlen(pt2)+1);

        if(!is_even(pt1)) result = 1;
        else result = compare_chars(pt1, pt2);

        pt1 = pt2;
        if (!result)
        {
            //printf("a3\n");
            return 0;
        }
    }
    //printf("a4 je to dobre\n");
    return result;
}



int check_input(char * input_data, int length)
{
    // returns 1 if the input is valid
    if (input_data[0] != '?' && input_data[0] != '#') return 0;
    int index = 1;
    while (isspace(input_data[index])) index++;
    if (index >= length) return 0;
    int result = index;
    for ( ; index < length-1; index++)
    {
        if(!isdigit(input_data[index])) return 0;
    }
    return result;
}

void get_combinations_recursive(char * result,
                                char * data,
                                int length,
                                int current_index,
                                int * count,
                                int write)
{
    if (length == current_index+2)// so there is no ',' at the end
    {
        
        result[current_index] = data[current_index];
        char * formated_result;
        char * formated_result2;
        formated_result = remove_char(result, length, '-');
        formated_result2 = remove_char(result, length, '-');
        //printf("\n\n\n\n-------------------------------------------------------------------------------------------------------------------\n");
        //print_string(formated_result, strlen(formated_result)+1);
        if(check_if_valid(formated_result2, strlen(formated_result2)+1))
        {
            (*count)++;
            if (write) printf("* %s\n", formated_result);
        }
        //printf("%d------------------%d\n", *count, check_if_valid(formated_result, strlen(formated_result)+1));
        free(formated_result);
        free(formated_result2);
    } else
    {
        result[current_index] = data[current_index];
        result[current_index+1] = '-';
        get_combinations_recursive(result, data, length, current_index+2, count, write);
        result[current_index+1] = ',';
        get_combinations_recursive(result, data, length, current_index+2, count, write);
    }
}

void get_combinations(char * input,int number_of_characters, int write)
{
    int length = number_of_characters * 2 - 2;
    int count = 0;
    char * data = (char *) calloc (length, sizeof(char));
    char * result = (char *) calloc (length, sizeof(char));
    for (int i = 0; i < length; i++)
    {
        if (i%2) data[i] = '_';
        else data[i] = input[i/2];
    }
    data[length - 1] = '\0'; 
    //print_string(input, length);
    //printf("length: %d ", length);
    //print_string(data, length);
    get_combinations_recursive(result, data, length, 0, &count, write);
    printf("Celkem: %d\n", count);
    //print_string(result, length);
    free(data);
    free(result);

}

void test_compare()
{
    char reku[] = {'1', '4','5', '\0'};
    char jako[] = {'4','5', '\0'};
    int result = compare_chars(reku, jako);
    printf("%d\n", result);
}

void test_check_if_valid()
{
    char array[] = {'1', '2', ',', '4', ',', '5', ',', '6','7','8', '9', '1', '3', '\0'};
    int result = check_if_valid(array, strlen(array));
    printf("%d\n", result);
}

void test_is_even()
{
    char array[] = {'1', '2', '\0'};
    printf("%d", is_even(array));
    char array1[] = {'1', '3', '\0'};
    printf("%d", is_even(array1));
    char array2[] = {'1', '4', '\0'};
    printf("%d", is_even(array2));
}

int main()
{   
    char * line = NULL;
    size_t len = 0;
    int number_of_characters = 0;
    int start = 0;
    printf("Posloupnost:\n");
    while (!feof(stdin))
    {
        number_of_characters = getline(&line, &len, stdin);
        if (number_of_characters < 1 || line == NULL) break;
        if ((start = check_input(line, number_of_characters)))
        {
            if (line[0] == '#') get_combinations(line + start, number_of_characters - start, 0);
            else if(line[0] == '?') get_combinations(line + start, number_of_characters - start, 1);
        } else 
        {
            printf("Nespravny vstup.\n");
            free(line);
            return 1;
        }
    }
    free(line);
    return 0;

}