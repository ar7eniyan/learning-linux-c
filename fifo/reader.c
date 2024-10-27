#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int fifo_fd = -1;

void sigint_handler(int ignore) {
    if (fifo_fd == -1) {
        exit(0);
    }
    if (close(fifo_fd) == -1) {
        perror("Can't close the FIFO");
    } else {
        printf("Closed!\n");
    }
    exit(EXIT_SUCCESS);
}

int main(void) {
    signal(SIGINT, sigint_handler);
    fifo_fd = open("./test_fifo_ipc", O_RDONLY);
    if (fifo_fd == -1) {
        perror("Can't open the FIFO file for reading");
        return EXIT_FAILURE;
    }

    printf("Opened!\n");
    // Infinite loop, the only way out is SIGINT handler
    for (;;) {
        char read_char;
        int ret = read(fifo_fd, &read_char, 1);
        if (ret == 0) {
            printf("read() returned zero\n");
        } else if (ret == -1) {
            perror("Error in read()");
            if (close(fifo_fd) == -1) {
                perror("Can't close the FIFO");
            }
            return EXIT_FAILURE;
        } else {
            printf("Got char: '%c' (0x%.2X)\n", read_char, read_char);
        }
        usleep(100 * 1000);
    }
}
