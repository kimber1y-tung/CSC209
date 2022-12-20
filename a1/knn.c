#include <stdio.h>
#include <math.h>    // Need this for sqrt()
#include <stdlib.h>
#include <string.h>

#include "knn.h"

/* Print the image to standard output in the pgmformat.
 * (Use diff -w to compare the printed output to the original image)
 */
void print_image(unsigned char *image) {
    printf("P2\n %d %d\n 255\n", WIDTH, HEIGHT);
    for (int i = 0; i < NUM_PIXELS; i++) {
        printf("%d ", image[i]);
        if ((i + 1) % WIDTH == 0) {
            printf("\n");
        }
    }
}

/* Return the label from an image filename.
 * The image filenames all have the same format:
 *    <image number>-<label>.pgm
 * The label in the file name is the digit that the image represents
 */
unsigned char get_label(char *filename) {
    // Find the dash in the filename
    char *dash_char = strstr(filename, "-");
    // Convert the number after the dash into a integer
    return (char) atoi(dash_char + 1);
}

/* ******************************************************************
 * Complete the remaining functions below
 * ******************************************************************/

/* Read a pgm image from filename, storing its pixels
 * in the array img.
 * It is recommended to use fscanf to read the values from the file. First,
 * consume the header information.  You should declare two temporary variables
 * to hold the width and height fields. This allows you to use the format string
 * "P2 %d %d 255 "
 *
 * To read in each pixel value, use the format string "%hhu " which will store
 * the number it reads in an an unsigned character.
 * (Note that the img array is a 1D array of length WIDTH*HEIGHT.)
 */
void load_image(char *filename, unsigned char *img) {
    // Open corresponding image file, read in header (which we will discard)
    FILE *f2 = fopen(filename, "r");
    //printf("this is f2 in load %s\n", f2);
    if (f2 == NULL) {
        perror("fopen");
        printf("file could not open in load image");
        exit(1);
    }
    // read in width and height
    int col, row;
    if (fscanf(f2, "P2 %d %d 255 ", &col, &row) != EOF){
        for (int i = 0; i < col * row; i++){
            int a = fscanf(f2, "%hhu ", &img[i]);
        }
    }

    // for (int i = 0; i < col * row; i++){
    //     fscanf(f2, "%hhu ", &img[i]);
    // }
    fclose(f2);
}

/**
 * Load a full dataset into a 2D array called dataset.
 *
 * The image files to load are given in filename where
 * each image file name is on a separate line.
 *
 * For each image i:
 *  - read the pixels into row i (using load_image)
 *  - set the image label in labels[i] (using get_label)
 *
 * Return number of images read.
 */
int load_dataset(char *filename,
                 unsigned char dataset[MAX_SIZE][NUM_PIXELS],
                 unsigned char *labels) {
    // We expect the file to hold up to MAX_SIZE number of file names
    int temp = 0;
    FILE *f1 = fopen(filename, "r");
    if (f1 == NULL) {
        exit(1);
    }
    char line[500];
    int i = 0;
    while ((fscanf(f1, "%[^\n]", line))!= EOF) {
        fgetc(f1);
        //printf("%s\n", line);
        load_image(line, dataset[i]);
        labels[i] = get_label(line);
        i ++;
        temp += 1;
    }

    fclose(f1);
    return temp;
}

/**
 * Return the euclidean distance between the image pixels in the image
 * a and b.  (See handout for the euclidean distance function)
 *
 * Remember the images are stored as a 1D array.
 */
double distance(unsigned char *a, unsigned char *b) {

    double ans;
    double sum = 0;
    for (int i = 1; i <= NUM_PIXELS; i++) {
        sum += (b[i] - a[i]) * (b[i] - a[i]);
    }
    ans = sqrt(sum);
    return ans;
}

/**
 * Returns the index of the smallest number in an array,
 * and changes the number in the array to 256 * 784,
 * so that if we call this function multiple times
 * we wont count the same index twice.
 */
int FindMinIndex(double arr[], int length) {
    int smallest_index = 0; // start with index 0 to compare with
    for (int i = 0; i < length; i++) {
        if (arr[i] < arr[smallest_index]) {
            smallest_index = i;  // new minimum
        }
    }
    arr[smallest_index] = 256 * 784;
    return smallest_index;
}

/**
 * Returns the most repeated number in an array,
 * if tie, return the smallest number
 */
int FindMostRepeated(const int arr[], int length) {
    int i, j, count;
    int tie = 0;
    int smallest_tie = 0;
    for (i = 0; i < length; i++) {
        count = 1;
        for (j = i + 1; j < length; j++) {
            if (arr[j] == arr[i]) {
                count++;
            }
        }
        if (count > tie) {
            tie = count;
            smallest_tie = arr[i];
        } else {
            // if tie frequency, return the smallest number
            if (count == tie) {
                if (arr[i] < smallest_tie) {
                    smallest_tie = arr[i];
                }
            }
        }
    }
    return smallest_tie;
}

/**
 * Return the most frequent label of the K most similar images to "input"
 * in the dataset
 * Parameters:
 *  - input - an array of NUM_PIXELS unsigned chars containing the image to test
 *  - K - an int that determines how many training images are in the
 *        K-most-similar set
 *  - dataset - a 2D array containing the set of training images.
 *  - labels - the array of labels that correspond to the training dataset
 *  - training_size - the number of images that are actually stored in dataset
 *
 * Steps
 *   (1) Find the K images in dataset that have the smallest distance to input
 *         When evaluating an image to decide whether it belongs in the set of
 *         K closest images, it will only replace an image in the set if its
 *         distance to the test image is strictly less than all of the images in
 *         the current K closest images.
 *   (2) Count the frequencies of the labels in the K images
 *   (3) Return the most frequent label of these K images
 *         In the case of a tie, return the smallest value digit.

 */
int knn_predict(unsigned char *input, int K,
                unsigned char dataset[MAX_SIZE][NUM_PIXELS],
                unsigned char *labels,
                int training_size) {

    //int k_closest[K];
    double all_distances[training_size];
    int index[K];
    int label[K];

    // find all distances and store it in all_distances
    for (int k = 0; k < training_size; k++){ //training_size
        double temp_distance = distance(input, dataset[k]);
        all_distances[k] = temp_distance;
    }

    // finding the index of K smallest in all_distances
    for (int j = 0; j < K; j++){
        index[j] = FindMinIndex(all_distances, training_size);
        label[j] = labels[index[j]];
    }

    return FindMostRepeated(label, K);
}

