#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void sigpipe_handler(int ignore) {
    printf("Got SIGPIPE!\n");
}

int main(void) {
    signal(SIGPIPE, sigpipe_handler);
    if (mknod("./test_fifo_ipc", S_IFIFO | 0666, 0) == -1) {
        if (errno != EEXIST) {
            perror("Can't create the FIFO file");
            return EXIT_FAILURE;
        }
    }
    int fifo_fd = open("./test_fifo_ipc", O_WRONLY | O_CREAT);
    if (fifo_fd == -1) {
        perror("Can't open the FIFO file for writing");
        return EXIT_FAILURE;
    }

    char line_buf[100];
    printf("Opened!\n");
    for (;;) {
        if (fgets(line_buf, sizeof line_buf, stdin) == NULL) {
            if (feof(stdin) != 0) {
                break;
            }
            printf("Stdin error\n");
            if (close(fifo_fd) == -1) {
                perror("Can't close the FIFO");
            }
            return EXIT_FAILURE;
        }
        if (strncmp(line_buf, "close\n", 7) == 0) {
            break;
        }

        int ret = write(fifo_fd, line_buf, strlen(line_buf));
        if (ret == 0) {
            printf("write() returned zero\n");
        } else if (ret == -1) {
            perror("Error in write");
        }
    }
    if (close(fifo_fd) == -1) {
        perror("Can't close the FIFO");
        return EXIT_FAILURE;
    }
    printf("Closed!\n");
    return EXIT_SUCCESS;
}
