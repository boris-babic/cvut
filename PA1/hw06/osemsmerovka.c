#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int check_valid_line(char *text, int length)
{
    for (int i = 0; i < length-1; i++)
    {
        if (!(text[i] >= 'a' && text[i] <= 'z') && text[i] != '.')
        {
            printf("%d\n", i);
            return 0;
        }
    }
    return 1;
}


void append_line(char ***array, int *length, int *index, char *current) {
    // Check if we need to resize the table
    if (*index >= *length) {
        int new_length = (*length * 3 / 2 + 2);  // Increase size by 50%
        *array = (char **)realloc(*array, new_length * sizeof(char *));
        *length = new_length;
    }

    // Create a copy of the current line and add it to the table
    (*array)[*index] = strdup(current);  // Make a copy of the line
}

void print_table(char ** array, int rows, int line_length)
{
    for (int i = 0; i < rows; i++)
    {
        printf("%d:    ", i);
        for (int j= 0; j< line_length; j++)
        {

            printf("(%c):(%03d) ", array[i][j], array[i][j]);
        }
        printf("\n");
    }
}

int main() 
{
    char ** table;
    int rows = 5;
    char *line = NULL;
    size_t len = 0;
    int number_of_characters;
    printf("Osmismerka:\n");

    int line_length = getline(&line, &len, stdin);
    table = (char **)malloc(sizeof(char *) * rows);
    table[0] = strdup(line);
    if (table[0][0] == '\n')
    {
        fail:
        printf("Nespravny vstup\n");
        free(table);
        return 1;
    }
    if (!check_valid_line(line, line_length)) goto fail;
    int i_index = 1;
    while ((number_of_characters = getline(&line, &len, stdin)) > 1)
    {
        //printf("nacital som %s\n", line);
        if (number_of_characters != line_length)
        {
            printf("%d, %d\n", number_of_characters, line_length);
            goto fail;
        }
        if (!check_valid_line(line, line_length)) goto fail;
        append_line(&table, &rows, &i_index, line);
        i_index++;
    }
    printf("skoncil som nacitanie\n");
    print_table(table, i_index, line_length);
    for (int i = 0; i < i_index; i++)
    {
        table[i][line_length-1] = '\0';
    }
    print_table(table, i_index, line_length);
    return 0;
}