#include <stdio.h> // for scanf and printf

int main() {

    char phone[11];
    int second;
    // the first is a 10 character string, and the second is an integer
    scanf("%s%d", phone, &second);

    if (second == -1){
        printf("%s\n", phone);
        return 0;
    } else if (second < -1 || second > 9){
        printf("ERROR\n");
        return 1;
    } else {
        printf("%c\n", phone[second]);
    }
    return 0;
}