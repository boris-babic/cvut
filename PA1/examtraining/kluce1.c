#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
/*my way of returning nullptr even tho thay are not in C*/
#ifndef nullptr
#define nullptr NULL
#endif
 
int check_valid_key(int minlocks, char ** locks, char * key, int length) {
    printf("vstupil som\n");
    int index = 0;
    int can_open = 0;
    while (locks[index] != nullptr) {
        printf("==============tocim sa ========\n");
        int current = 0;
        for (int i = 0; i < length; i++) {
            if ((locks[index][i] == '-') || (locks[index][i] == key[i]) ) {
                continue;
            } else {
                current = 1;
                break;
            }
        }
        if (!current) can_open += 1;
        index += 1;
    }
    if (can_open >= minlocks) return 1;
    else return 0;
}
/*get number of keys*/
void countKeys_recursive(int minlocks, char ** locks, int index, int length , char * key, int * result) {
    printf("tocim sa %d\n", index);
    for (int i = 0; i < 5; i++) {
        key[index] = 'A' + i;
        if (index + 1 == length) { //si na konci kluca
            if (check_valid_key( minlocks, locks, key, length)) {
                (*result) +=1;
            }
        } else { //vyrobit dalsi zubok
            countKeys_recursive(minlocks, locks, index + 1, length, key, result);
        }
    }
}

int countKeys(int minLocks, char **locks) {
    int length = strlen(locks[0]);
    int index = 0;
    char * key = (char *)calloc(length+1, sizeof(char));
    int result;
    countKeys_recursive(minLocks, locks, index, length, key, &result);

    free(key);
    return result;
}
 
/*get arr of keys*/
char **findKeys(int minLocks, char **locks) {
 
    //dummy solution
    return nullptr;
}
void test_countKeys() {
    // Test 1: Jednoduchý test s jedním zámkem
    char *locks1[] = {"A-B", nullptr};
    assert(countKeys(1, locks1) == 5); // Klíče: AAB, ABB, ACB, ADB, AEB
 
 
    // Test 2: Dva zámky, minLocks = 2
    char *locks2[] = {"A--", "--B", nullptr};
    assert(countKeys(2, locks2) == 5); // Klíče: AAB, ABB, ACB, ADB, AEB
 
    // Test 3: Zámek s více pomlčkami
    char *locks3[] = {"A---", nullptr};
    assert(countKeys(1, locks3) == 125); // 5^3 (možnosti pro 3 libovolné pozice)
 
    // Test 4: Více zámků s různými vzory
    char *locks4[] = {"ABCD", "--CD", "A---", nullptr};
    assert(countKeys(2, locks4) > 0); // Počet klíčů, které odemykají alespoň 2 zámky
 
    // Test 5: Více zámků s různými vzory
    char *locks5[] = {"BC--D", nullptr};
    assert(countKeys(1, locks5) == 25); // Ze zadání
}
 
/* matchStrings() neumim, můžeš si ho udělat doma pomocí quicksortu
 * na zkoušce se myslím používali 2 foor loops tho
 */
 
/*na zkoušce se doopravy kontrolovalo, jestli jsem došli ke stejným klíčům jako poli se správnými klíči*/
/*
void test_findKeys() {
    // Test 1: Jednoduchý test s jedním zámkem
    char *locks1[] = {"A-B", nullptr};
    char **keys1 = findKeys(1, locks1);
    int count1 = 0;
    while (keys1[count1] != nullptr) count1++;
    assert(count1 == 5); // Klíče: AAB, ABB, ACB, ADB, AEB
 
    // Test 2: Dva zámky, minLocks = 2
    char *locks2[] = {"A--", "--B", nullptr};
    char **keys2 = findKeys(2, locks2);
    int count2 = 0;
    while (keys2[count2] != nullptr) count2++;
    assert(count2 == 5); // Klíče: AAB, ABB, ACB, ADB, AEB
 
    // Test 3: Zámek s více pomlčkami
    char *locks3[] = {"A---", nullptr};
    char **keys3 = findKeys(1, locks3);
    int count3 = 0;
    while (keys3[count3] != nullptr) count3++;
    assert(count3 == 125); // 5^3 (možnosti pro 3 libovolné pozice)
 
    // Ověření, že výsledky obsahují klíče
    assert(strcmp(keys3[0], "AAAA") == 0);
 
    // Uvolnění alokované paměti
    for (int i = 0; keys1[i] != nullptr; i++) free(keys1[i]);
    free(keys1);
    for (int i = 0; keys2[i] != nullptr; i++) free(keys2[i]);
    free(keys2);
    for (int i = 0; keys3[i] != nullptr; i++) free(keys3[i]);
    free(keys3);
}
*/
int main() {
    printf("idem\n");
    char * locks1[] = {"ABCD", "--CD", "A---", nullptr};
    printf("%d-\n", countKeys(2, locks1));
    //test_countKeys();
    //test_findKeys();
    printf("Všechny testy proběhly úspěšně.\n");
    return 0;
}