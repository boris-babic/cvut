#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

void remove_leading_zeros(char * str) {
    int i = 0;
    while (str[i] == '0' && str[i] != '\0') {
        i++;
    }
    memmove(str, str + i, strlen(str) - i + 1);
}


int compare_chars(char * input_a, char * input_b) {
    remove_leading_zeros(input_a);
    remove_leading_zeros(input_b);

    int length_a = strlen(input_a);
    int length_b = strlen(input_b);

    if (length_a > length_b) {
        return 0;  
    } else if (length_a < length_b) {
        return 1;  
    }

    for (int i = 0; i < length_a; i++) {
        if (input_a[i] < input_b[i]) {
            return 1;  
        } else if (input_a[i] > input_b[i]) {
            return 0;  
        }
    }
    return 1;
}
int is_even(char *number) {
    char last_digit = number[strlen(number) - 1];
    return (last_digit == '0' || last_digit == '2' || last_digit == '4' || last_digit == '6' || last_digit == '8');
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
        return 1;
    }
    if(!is_even(pt1)) result = 1;
    else result = compare_chars(pt1, pt2);
    if (!result) return 0;

    pt1 = pt2;
    while ((pt2 = strtok(NULL, ",")))
    {
        if(!is_even(pt1)) result = 1;
        else result = compare_chars(pt1, pt2);

        pt1 = pt2;
        if (!result) return 0;
    }
    return result;
}



int check_input(char * input_data, int length)
{
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
        if(check_if_valid(formated_result2, strlen(formated_result2)+1))
        {
            (*count)++;
            if (write) printf("* %s\n", formated_result);
        }
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

    get_combinations_recursive(result, data, length, 0, &count, write);
    printf("Celkem: %d\n", count);

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