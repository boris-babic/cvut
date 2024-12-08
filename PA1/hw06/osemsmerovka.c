#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



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
    if (*index >= *length) {
        int new_length = (*length * 3 / 2 + 2);
        *array = (char **)realloc(*array, new_length * sizeof(char *));
        *length = new_length;
    }

    (*array)[*index] = strdup(current); 
    (*index)++;
}

void print_table_char(char ** array, int rows, int line_length)
{
    printf("----------------char----------------\n");
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

void print_table_int(int ** array, int rows, int line_length)
{
    printf("----------------int-----------------\n");
    for (int i = 0; i < rows; i++)
    {
        printf("%d:    ", i);
        for (int j= 0; j< line_length; j++)
        {
            printf("%d ", array[i][j]);
        }
        printf("\n");
    }
}



int cross_words(char ** array, int ** found_table, int rows, int columns, char * word, int word_length)
{
    int result = 0;
    int match;
    for(int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            //printf("i:%d, j:%d-> %c, result %d\n", i,j, array[i][j], result);
            if (array[i][j] == word[0])
            {
                //up
                match = 1;
                for (int k = 1; k < word_length; k++)
                {
                    if(i-k < 0) break;
                    if (array[i-k][j] != word[k])
                    {
                        break;
                    } else match++;
                }
                if (match == word_length)
                {
                    result++;
                    for (int k = 0; k < word_length; k++)
                    {
                        found_table[i-k][j] = 1;
                    }
                }
                match = 1;
                //up-right
                for (int k = 1; k < word_length; k++)
                {
                    if(i-k < 0 || j+k >= columns) break;
                    if (array[i-k][j+k] != word[k])
                    {
                        break;
                    } else match++;
                }
                if (match == word_length)
                {
                    result++;
                    for (int k = 0; k < word_length; k++)
                    {
                        found_table[i-k][j+k] = 1;
                    }
                }
                match = 1;
                //right
                for (int k = 1; k < word_length; k++)
                {
                    if(j+k >= columns) break;
                    if (array[i][j+k] != word[k])
                    {
                        break;
                    } else match++;
                }
                if (match == word_length)
                {
                    result++;
                    for (int k = 0; k < word_length; k++)
                    {
                        found_table[i][j+k] = 1;
                    }
                }
                match = 1;
                //down-right
                for (int k = 1; k < word_length; k++)
                {
                    if(i+k >= rows || j+k >= columns) break;
                    if (array[i+k][j+k] != word[k])
                    {
                        break;
                    } else match++;
                }
                if (match == word_length)
                {
                    result++;
                    for (int k = 0; k < word_length; k++)
                    {
                        found_table[i+k][j+k] = 1;
                    }
                }
                match = 1;
                //down
                for (int k = 1; k < word_length; k++)
                {
                    if(i+k >= rows) break;
                    if (array[i+k][j] != word[k])
                    {
                        break;
                    } else match++;
                }
                if (match == word_length)
                {
                    result++;
                    for (int k = 0; k < word_length; k++)
                    {
                        found_table[i+k][j] = 1;
                    }
                }
                match = 1;
                //down-left
                for (int k = 1; k < word_length; k++)
                {
                    if(i+k >= rows || j-k < 0) break;
                    if (array[i+k][j-k] != word[k])
                    {
                        break;
                    } else match++;
                }
                if (match == word_length)
                {
                    result++;
                    for (int k = 0; k < word_length; k++)
                    {
                        found_table[i+k][j-k] = 1;
                    }
                }
                match = 1;
                //left
                for (int k = 1; k < word_length; k++)
                {
                    if(j-k < 0) break;
                    if (array[i][j-k] != word[k])
                    {
                        break;
                    } else match++;
                }
                if (match == word_length)
                {
                    result++;
                    for (int k = 0; k < word_length; k++)
                    {
                        found_table[i][j-k] = 1;
                    }
                }
                match = 1;
                //up-left
                for (int k = 1; k < word_length; k++)
                {
                    if(i-k < 0 || j-k < 0) break;
                    if (array[i-k][j-k] != word[k])
                    {
                        break;
                    } else match++;
                }
                if (match == word_length)
                {
                    result++;
                    for (int k = 0; k < word_length; k++)
                    {
                        found_table[i-k][j-k] = 1;
                    }
                }
                match = 1;
                
            }
        }
    }
    return result;
}

int find_words(char ** array, int ** found_table, int rows, int columns, char * word, int word_length)
{
    int result = 0;
    int match;
    for(int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            //printf("i:%d, j:%d-> %c, result %d\n", i,j, array[i][j], result);
            if (array[i][j] == word[0])
            {
                //up
                match = 1;
                for (int k = 1; k < word_length; k++)
                {
                    if(i-k < 0) break;
                    if (array[i-k][j] != word[k])
                    {
                        break;
                    } else match++;
                }
                if (match == word_length) result++;
                match = 1;
                //up-right
                for (int k = 1; k < word_length; k++)
                {
                    if(i-k < 0 || j+k >= columns) break;
                    if (array[i-k][j+k] != word[k])
                    {
                        break;
                    } else match++;
                }
                if (match == word_length) result++;
                match = 1;
                //right
                for (int k = 1; k < word_length; k++)
                {
                    if(j+k >= columns) break;
                    if (array[i][j+k] != word[k])
                    {
                        break;
                    } else match++;
                }
                if (match == word_length) result++;
                match = 1;
                //down-right
                for (int k = 1; k < word_length; k++)
                {
                    if(i+k >= rows || j+k >= columns) break;
                    if (array[i+k][j+k] != word[k])
                    {
                        break;
                    } else match++;
                }
                if (match == word_length) result++;
                match = 1;
                //down
                for (int k = 1; k < word_length; k++)
                {
                    if(i+k >= rows) break;
                    if (array[i+k][j] != word[k])
                    {
                        break;
                    } else match++;
                }
                if (match == word_length) result++;
                match = 1;
                //down-left
                for (int k = 1; k < word_length; k++)
                {
                    if(i+k >= rows || j-k < 0) break;
                    if (array[i+k][j-k] != word[k])
                    {
                        break;
                    } else match++;
                }
                if (match == word_length) result++;
                match = 1;
                //left
                for (int k = 1; k < word_length; k++)
                {
                    if(j-k < 0) break;
                    if (array[i][j-k] != word[k])
                    {
                        break;
                    } else match++;
                }
                if (match == word_length) result++;
                match = 1;
                //up-left
                for (int k = 1; k < word_length; k++)
                {
                    if(i-k < 0 || j-k < 0) break;
                    if (array[i-k][j-k] != word[k])
                    {
                        break;
                    } else match++;
                }
                if (match == word_length) result++;
                match = 1;
                
            }
        }
    }
    return result;
}

void free_table(char **table, int rows) {
    if (table) {
        for (int i = 0; i < rows; i++) {
            free(table[i]); // Free each string
        }
        free(table); // Free the top-level array
    }
}

void free_table_found(int **table_found, int rows) {
    if (table_found) {
        for (int i = 0; i < rows; i++) {
            free(table_found[i]); // Free each row
        }
        free(table_found); // Free the top-level array
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
        printf("Nespravny vstup.\n");
        free(line);
        free_table(table, rows);
        return 1;
    }
    if (!check_valid_line(line, line_length))
    {
        printf("Nespravny vstup.\n");
        free(line);
        free_table(table, rows);
        return 1;
    }
    int i_index = 1;
    while ((number_of_characters = getline(&line, &len, stdin)) > 1)
    {
        //printf("nacital som %s\n", line);
        if (number_of_characters != line_length)
        {
            printf("Nespravny vstup.\n");
            free(line);
            free_table(table, rows);
            return 1;
        }
        if (!check_valid_line(line, line_length))
        {
            printf("Nespravny vstup.\n");
            free(line);
            free_table(table, rows);
            return 1;
        }
        append_line(&table, &rows, &i_index, line);
    }

    //printf("skoncil som nacitanie\n");
    for (int i = 0; i < i_index; i++)
    {
        table[i][line_length-1] = '\0';
    }
    //print_table_char(table, i_index, line_length);

    int size_i = i_index, size_j = line_length-1;
    int ** table_found = (int**)calloc(size_i, sizeof(int *));
    for (int i= 0; i <size_i; i++)
    {
        table_found[i] = (int *)calloc(size_j, sizeof(int));
    }
    for (int i = 0; i < size_j; i++)
    {
        table_found[0][i] = 0;
    }
    //print_table_int(table_found, size_i, size_j);

    char operation = ' ';
    char  word[1000];
    char enter = '\n';
    while(!feof(stdin))
    {
        if(feof(stdin)) break;
        if(scanf("%c", &operation) != 1) break;
        //printf("---------------------operation (%c)\n", operation);
        if(operation == '#')
        {
            scanf(" %999s", word);
            if (strlen(word) < 2)
            {
                printf("Nespravny vstup.\n");
                free(line);
                free_table(table, size_i);
                free_table_found(table_found, size_i);
                return 1;
            }
            printf("%s: %dx\n",word,find_words(table, table_found, size_i, size_j, word, (int)strlen(word)));
            scanf("%c", &enter);
        } else if (operation == '-')
        {
            scanf(" %999s", word);
            if (strlen(word) < 2)
            {
                printf("Nespravny vstup.\n");
                free(line);
                free_table(table, size_i);
                free_table_found(table_found, size_i);
                return 1;
            }
            printf("%s: %dx\n",word,cross_words(table, table_found, size_i, size_j, word, (int)strlen(word)));
            scanf("%c", &enter);
        } else if (operation == '?')
        {
            scanf("%c", &enter);
            if(enter != '\n')
            {
                printf("Nespravny vstup.\n");
                free(line);
                free_table(table, size_i);
                free_table_found(table_found, size_i);
                return 1;
            }
            int count = 0;
            int what_to_print = 0;
            printf("Tajenka:\n");
            for (int i = 0; i < size_i; i++)
            {
                for (int j = 0; j < size_j; j++)
                {
                    if (table_found[i][j] == 0 && table[i][j] != '.')
                    {
                        printf("%c", table[i][j]);
                        count++;
                        what_to_print = 1;
                        if (count == 60)
                        {
                            printf("\n");
                            count = 0;
                        }
                    } 
                }
            }
            if (what_to_print) printf("\n");
        } else
        {
            printf("Nespravny vstup.\n");
            free(line);
            free_table(table, size_i);
            free_table_found(table_found, size_i);
            return 1;
        }
        //print_table_int(table_found, size_i, size_j);
    }
    free(line);
    free_table(table, size_i);
    free_table_found(table_found, size_i);
    return 0;
}