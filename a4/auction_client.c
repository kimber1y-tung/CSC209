#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUF_SIZE 128

#define MAX_AUCTIONS 5
#ifndef VERBOSE
#define VERBOSE 0
#endif

#define ADD 0
#define SHOW 1
#define BID 2
#define QUIT 3

/* Auction struct - this is different than the struct in the server program
 */
struct auction_data {
    int sock_fd;
    char item[BUF_SIZE];
    int current_bid;
};

/* Displays the command options available for the user.
 * The user will type these commands on stdin.
 */

void print_menu() {
    printf("The following operations are available:\n");
    printf("    show\n");
    printf("    add <server address> <port number>\n");
    printf("    bid <item index> <bid value>\n");
    printf("    quit\n");
}

/* Prompt the user for the next command 
 */
void print_prompt() {
    printf("Enter new command: ");
    fflush(stdout);
}

/* Unpack buf which contains the input entered by the user.
 * Return the command that is found as the first word in the line, or -1
 * for an invalid command.
 * If the command has arguments (add and bid), then copy these values to
 * arg1 and arg2.
 */
int parse_command(char *buf, int size, char *arg1, char *arg2) {
    int result = -1;
    char *ptr = NULL;
    if(strncmp(buf, "show", strlen("show")) == 0) {
        return SHOW;
    } else if(strncmp(buf, "quit", strlen("quit")) == 0) {
        return QUIT;
    } else if(strncmp(buf, "add", strlen("add")) == 0) {
        result = ADD;
    } else if(strncmp(buf, "bid", strlen("bid")) == 0) {
        result = BID;
    } 
    ptr = strtok(buf, " "); // first word in buf

    ptr = strtok(NULL, " "); // second word in buf
    if (ptr != NULL) {
        strncpy(arg1, ptr, BUF_SIZE);
    } else {
        return -1;
    }
    ptr = strtok(NULL, " "); // third word in buf
    if(ptr != NULL) {
        strncpy(arg2, ptr, BUF_SIZE);
        return result;
    } else {
        return -1;
    }
    return -1;
}

/* Connect to a server given a hostname and port number.
 * Return the socket for this server
 */
int add_server(char *hostname, int port) {
    // Create the socket FD.
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("client: socket");
        exit(1);
    }
    
    // Set the IP and port of the server to connect to.
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    struct addrinfo *ai;
    
    /* this call declares memory and populates ailist */
    if (getaddrinfo(hostname, NULL, NULL, &ai) != 0) {
        close(sock_fd);
        return -1;
    }
    /* we only make use of the first element in the list */
    server.sin_addr = ((struct sockaddr_in *) ai->ai_addr)->sin_addr;

    // free the memory that was allocated by getaddrinfo for this list
    freeaddrinfo(ai);

    // Connect to the server.
    if (connect(sock_fd, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("client: connect");
        close(sock_fd);
        return -1;
    }
    if(VERBOSE){
        fprintf(stderr, "\nDebug: New server connected on socket %d.  Awaiting item\n", sock_fd);
    }
    return sock_fd;
}
/* ========================= Add helper functions below ========================
 * Please add helper functions below to make it easier for the TAs to find the 
 * work that you have done.  Helper functions that you need to complete are also
 * given below.
 */

/* Print to standard output information about the auction
 */
void print_auctions(struct auction_data *a, int size) {
    printf("Current Auctions:\n");

    /* TODO Print the auction data for each currently connected 
     * server.  Use the follosing format string:
     *     "(%d) %s bid = %d\n", index, item, current bid
     * The array may have some elements where the auction has closed and
     * should not be printed.
     */
    for (int i = 0; i < size; i++) {
        if (a[i].sock_fd != -1) {
            printf("(%d) %s bid = %d\n", i, a[i].item, a[i].current_bid);
        }
    }
}

/* Process the input that was sent from the auction server at a[index].
 * If it is the first message from the server, then copy the item name
 * to the item field.  (Note that an item cannot have a space character in it.)
 */
void update_auction(char *buf, int size, struct auction_data *a, int index) {
    
    // TODO: Complete this function
    
    // fprintf(stderr, "ERROR malformed bid: %s", buf);
    // printf("\nNew bid for %s [%d] is %d (%d seconds left)\n",           );
    const char *name = strtok(buf, " ");
    const char *bid = strtok(NULL, " ");
    const char *left = strtok(NULL, " ");
    if (name == NULL || bid == NULL || left == NULL) {
        fprintf(stderr, "ERROR malformed bid: %s\n", buf);
    } else {
        strcpy(a[index].item, name);
        a[index].current_bid = atoi(bid);
        printf("\nNew bid for for %s [%d] is %d (%d seconds left)\n", 
            name, index, a[index].current_bid, atoi(left));
    }
}

int main(void) {

    char name[BUF_SIZE];

    // Declare and initialize necessary variables
    // TODO
    char command[BUF_SIZE];
    char arg1[BUF_SIZE];
    char arg2[BUF_SIZE];
    int len_command;
    int num_server = 0;

    struct auction_data a[MAX_AUCTIONS];
    for (int i = 0; i < MAX_AUCTIONS; i++) {
        a[i].current_bid = 0;
        a[i].item[0] = '\0';
        a[i].sock_fd = -1;
    }

    // Get the user to provide a name.
    printf("Please enter a username: ");
    fflush(stdout);
    int num_read = read(STDIN_FILENO, name, BUF_SIZE);
    if(num_read <= 0){
        fprintf(stderr, "ERROR: read from stdin failed\n");
        exit(1);
    }
    print_menu();

    // TODO
    int maxfd = 2;
    fd_set all_fds;
    FD_ZERO(&all_fds);
    FD_SET(STDIN_FILENO, &all_fds);

    while(1) {
    	int index;
        print_prompt();

        // TODO
        if (select(maxfd + 1, &all_fds, NULL, NULL, NULL) == -1) {
            perror("server: select");
            exit(1);
        }

        if (FD_ISSET(STDIN_FILENO, &all_fds)) {
            len_command = read(STDIN_FILENO, command, BUF_SIZE);

            if (len_command == 0) {
                exit(0);
            }
            command[len_command] = '\0';

            int type = parse_command(command, BUFSIZ, arg1, arg2);
            switch (type) {
            int fd;
            case SHOW:
                print_auctions(a, MAX_AUCTIONS);
                break;
            case QUIT:
                exit(0);
                break;
            case ADD:
                if (num_server < MAX_AUCTIONS) {
                    fd = add_server(arg1, atoi(arg2));
                    a[num_server].sock_fd = fd;
                    maxfd = fd;
                    FD_SET(fd, &all_fds);
                    // write the user name to the server
                    if (write(a[num_server].sock_fd, name, num_read) == -1) {
                        fprintf(stderr, "Write to %d faild\n", a[num_server].sock_fd);
                        a[num_server].sock_fd = -1;
                        close(a[num_server].sock_fd);
                    }
                    num_server++;
                } else {
                    fprintf(stderr, "client: max auction servers\n");
                }
                break;
            case BID:
                index = atoi(arg1);
                if (a[index].sock_fd != -1) {
                    a[index].current_bid = atoi(arg2);
                    if (write(a[index].sock_fd, arg2, sizeof(a[index].current_bid)) == -1) {
                        fprintf(stderr, "Write to %d failed\n", a[index].sock_fd);
                    }
                } else {
                    fprintf(stderr, "There is no auction open at %d\n", index);
                }

                break;
            default:
                print_menu();
                break;
            }
        }

        // check all server
        for (int i = 0; i < MAX_AUCTIONS; i++) {
            if (a[i].sock_fd == -1) {
                continue;
            }
            if (FD_ISSET(a[i].sock_fd, &all_fds)) {
                char buf[BUF_SIZE];
                int size = read(a[i].sock_fd, buf, BUF_SIZE);
                if (strncmp("Auction closed", buf, strlen("Auction closed")) == 0) {
                    a[i].sock_fd = -1;
                    printf("%s", buf);
                    continue;
                } else {
                    update_auction(buf, size, a, i);
                }
            }
        }
    }
    return 0; // Shoud never get here
}

