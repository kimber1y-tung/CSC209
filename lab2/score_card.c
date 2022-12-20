#include <stdio.h>
#include <stdlib.h>

/*
 * This function interprets score_card as an array of pointers with size elements.
 * Return the sum of the values pointed to by the elements of score_card.
 *
 * takes an array of pointers to integers,
 * and returns the sum of the integers being pointed to.
 */
int sum_card(int **score_card, int size) {
    // TODO: write the body of sum_card according to its description.
    int sum = 0;
    int i;

    for (i = 0; i < size; i++) {
        sum = sum + *score_card[i];
    }
    return sum;
}


/*
 * NOTE: don't change the main function!
 * Sample usage:
 * $ gcc -Wall -std=gnu99 -g -o score_card score_card.c
 * $ ./score_card 10 -3
 * Sum: 7
 */
int main(int argc, char **argv) {
    int size = argc - 1;
    int *score_card[size]; // score_card holds "size" pointers

    for (int i = 0; i < size; i++) {
    // NOTE: We haven't covered malloc yet, so don't worry about this line.
        score_card[i] = malloc(sizeof(int));
        *(score_card[i]) = strtol(argv[i + 1], NULL, 10);
    }

    printf("Sum: %d\n", sum_card(score_card, size));

    return 0;
}
