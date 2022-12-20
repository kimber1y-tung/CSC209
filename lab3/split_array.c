#include <stdio.h>
#include <stdlib.h>

/* Return a pointer to an array of two dynamically allocated arrays of ints.
   The first array contains the elements of the input array s that are
   at even indices.  The second array contains the elements of the input
   array s that are at odd indices.

   Do not allocate any more memory than necessary.
*/
//original = 0 1 2 3 4 -> length = 5
//split_0  = 0 2 4 -> even
//split_1  = 1 3   -> odd

//original = 0 1 2 3 -> length = 4
//split_0  = 0 2   -> even
//split_1  = 1 3   -> odd
int **split_array(const int *s, int length) {

    // allocate memory size
    int **split = malloc(sizeof(int*) * 2);
    if ((length % 2) != 0){
        split[0] = malloc(sizeof(int) * (length/2 + 1));
        split[1] = malloc(sizeof(int) * (length/2));
    } else {
        split[0] = malloc(sizeof(int) * (length/2));
        split[1] = malloc(sizeof(int) * (length/2));
    }
    //even
    for (int i = 0, n = 0; i < length; i+=2, n++) {
        split[0][n] = s[i];
    }
    //odd
    for (int i = 1, n = 0; i < length; i+=2, n++) {
        split[1][n] = s[i];
    }

//    printf("\n");
//    printf("testing[0]:\n");
//    for (int i = 0; i < (length + 1) / 2; i++) {
//        printf("%d ", split[0][i]);
//    }
//    printf("\n");
//
//    printf("testing[1]:\n");
//    for (int i = 0; i < length / 2; i++) {
//        printf("%d ", split[1][i]);
//    }
//    printf("\n");

    return split;
}

/* Return a pointer to an array of ints with size elements.
   - strs is an array of strings where each element is the string
     representation of an integer.
   - size is the size of the array
 */
int *build_array(char **strs, int size) {
    // allocate memory size
    int *arr = malloc(sizeof(int) * size);
    for (int i = 0; i < size; i++) {
        arr[i] = strtol(strs[i + 1], NULL, 10);
    }
    return arr;
}

int main(int argc, char **argv) {
    /* Replace the comments in the next two lines with the appropriate
       arguments.  Do not add any additional lines of code to the main
       function or make other changes.
     */
    int *full_array = build_array(argv, (argc - 1));
    int **result = split_array(full_array, (argc - 1));

    printf("Original array:\n");
    for (int i = 0; i < argc - 1; i++) {
        printf("%d ", full_array[i]);
    }
    printf("\n");

    printf("result[0]:\n");
    for (int i = 0; i < argc / 2; i++) {
        printf("%d ", result[0][i]);
    }
    printf("\n");

    printf("result[1]:\n");
    for (int i = 0; i < (argc - 1) / 2; i++) {
        printf("%d ", result[1][i]);
    }
    printf("\n");
    free(full_array);
    free(result[0]);
    free(result[1]);
    free(result);
    return 0;
}
