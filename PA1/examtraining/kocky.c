#ifndef __TRAINER__
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct TDice {
    struct TDice * m_Next;
    int m_Sides;
} TDICE;
#endif

void count_recursive(TDICE * dice, int from, int to, int sum, size_t * count) {
    if (dice->m_Next == NULL) {
        for (int i = 1; i <= dice->m_Sides; i++) {
            if (sum + i >= from && sum + i <= to) {
                *count +=1;
            }
        }
    } else {
        for (int i = 1; i <= dice->m_Sides; i++) {
            if (sum + i <= to) {
                count_recursive(dice->m_Next, from, to, sum + i, count);
            }
        }
    }
}

size_t countCombinations(TDICE * dice, int from, int to) {
    size_t count = 0;
    if (dice == NULL){
        if (from > 0) return 0;
        else return 1;
    }
    count_recursive(dice, from, to, 0, &count);
    return count;
}

#ifndef __TRAINER__
void freeDice(TDICE * dice) {
    while(dice) {
        TDICE * tmp = dice;
        dice = dice->m_Next;
        free(tmp);
    }
}

TDICE * newDice(int sides, TDICE * next) {
    TDICE * result = (TDICE *)malloc(sizeof(TDICE));
    result->m_Sides = sides;
    result->m_Next = next;
    return result;
}

int main () {
    TDICE * a = newDice(6, newDice(6, NULL));

    assert(countCombinations(a, 6, 6) == 5);
    assert(countCombinations(a, 3, 3) == 2);
    assert(countCombinations(a, 11, 12) == 3);
    assert(countCombinations(a, 2, 3) == 3);
    assert(countCombinations(a, 1, 7) == 21);
    assert(countCombinations(a, 0, 12) == 36);
    assert(countCombinations(a, 0, 0) == 0);
    assert(countCombinations(a, 13, 13) == 0);
    freeDice(a);

    TDICE * b = newDice(6, newDice(4, newDice(4, NULL)));
    assert(countCombinations(b, 6, 6) == 10);
    assert(countCombinations(b, 3, 3) == 1);
    assert(countCombinations(b, 11, 12) == 16);
    assert(countCombinations(b, 2, 3) == 1);
    assert(countCombinations(b, 1, 7) == 33);
    assert(countCombinations(b, 0, 12) == 92);
    assert(countCombinations(b, 0, 0) == 0);
    assert(countCombinations(b, 13, 13) == 3);
    freeDice(b);
    
    return 0;
}
#endif