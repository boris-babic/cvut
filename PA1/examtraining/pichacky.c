#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct person {
    char * name;
    int from;
    int to;
} person;

person make_person(char * name, size_t length, int from, int to) {
    person result;
    result.name = (char *)strndup(name, length);
    result.name[strlen(result.name)-1] = '\0';
    result.from = from;
    result.to = to;
    return result;
}

typedef struct array {
    person * data;
    int length;
} array;

void append(array * list , person current, int count) {
    if(list->length <= count) {
        list->length = list->length * 3 / 2;
        person * new_data = (person *)realloc(list->data, sizeof(person) * list->length);
        list->data = new_data;
    }
    ((list->data)[count]) = current;
}

int get_time(int hours, int minutes) {
    return (60*hours + minutes);
}

void print_array(array * list) {
    for (int i = 0; i < list->length; i++) {
        printf("(%d): (%s), %d:%02d - %d:%02d\n", i, list->data[i].name, list->data[i].from/60, list->data[i].from%60, list->data[i].to/60, list->data[i].to%60);
    }
}
void free_array(array * list) {
    for (int i = 0; i < list->length; i++) {
        free(list->data[i].name);
    }
}

int count_people(array * list, int time) {
    int count =0;
    for (int i = 0; i < list->length; i++) {
        if ((list->data[i]).from <= time && (list->data[i]).to >= time) count++;
    }
    return count;
}

int check_time(int hours, int minutes) {
    if (hours >= 24 || hours < 0 || minutes >= 60 || minutes < 0) return 0;
    return 1;
}
int check_array(array * list) {
    for (int i = 0; i < list->length; i++) {
        if ((list->data[i]).name[0] == '\0') return 0;
    }
    return 1;
}
int main() {
    person current;
    int from_hours, from_minutes, to_hours, to_minutes;
    char * name = NULL;
    size_t length = 10;

    char character;
    char whitespace;
    int count = 0;
    
    array list;
    list.length = 10;
    list.data = (person *)malloc(sizeof(person) * list.length);
    printf("Zadejte udaje:\n");
    while (scanf(" %c", &character) && character == '{') {
        scanf("%c", &whitespace);
        getdelim(&name, &length, ',', stdin);
        if (name == NULL) {
            printf("Nespravny vstup.\n");
            free_array(&list);
            free(list.data);
            free(name);
            return 0;
        }
        scanf("%d:%d - %d:%d }", &from_hours, &from_minutes, &to_hours, &to_minutes);
        if (!check_time(from_hours, from_minutes) || !check_time(to_hours, to_minutes)) {
            printf("Nespravny vstup.\n");
            //free_array(&list);
            free(list.data);
            free(name);
            return 0;
        }
        current = make_person(name, length, get_time(from_hours, from_minutes), get_time(to_hours, to_minutes));
        //printf("%s: %d - %d\n", current.name, current.from, current.to);
        append(&list, current, count);
        count+=1;
    }
    
    person * new_data = (person *)realloc(list.data, sizeof(person)* count);
    list.data = new_data;
    list.length = count;


    int time_hour, time_minute, time;
    if (character == '?') {
        if(!check_array(&list)) {
            printf("Nespravny vstup.\n");
            free_array(&list);
            free(list.data);
            free(name);
            return 0;
        }
        while(scanf("%d:%d", &time_hour, &time_minute)) {
            if (feof(stdin)) break;
            time = get_time(time_hour, time_minute);
            if (!check_time(time_hour, time_minute)) {
                printf("Nespravny vstup.\n");
                free_array(&list);
                free(list.data);
                free(name);
                return 0;
            }
            printf("Celkem: %d\n", count_people(&list, time));
            if (feof(stdin)) break;
        }
    } else {
        printf("Nespravny vstup.\n");
        free_array(&list);
        free(list.data);
        free(name);
        return 0;
    }
    
    free_array(&list);
    free(list.data);
    free(name);
    return 0;
}