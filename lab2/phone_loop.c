#include <stdio.h> // for scanf and printf

int main() {

    char phone[11];
    int second;
    int temp; // when temp == 1, temp is true
    // the first is a 10 character string, and the second is an integer
    scanf("%s", phone);

    while (scanf("%d", &second) != EOF){
        if (second == -1){
            printf("%s\n", phone);
            temp = 0;
        } else if (second < -1 || second > 9){
            printf("ERROR\n");
            temp = 1;
        } else {
            printf("%c\n", phone[second]);
            temp = 0;
        }

    }
    return temp;
}