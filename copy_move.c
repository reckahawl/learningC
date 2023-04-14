#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int input_fd, output_fd;
    ssize_t bytes_read, bytes_written;
    char buffer[BUFFER_SIZE];
    int option;
    char *src_file, *dst_file;

    if (argc != 4) {
        fprintf(stderr, "Usage: %s [copy/move] [source file] [destination file]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    option = strcmp(argv[1], "copy") == 0 ? O_CREAT | O_WRONLY | O_TRUNC : O_CREAT | O_WRONLY | O_TRUNC | O_EXCL;

    src_file = argv[2];
    dst_file = argv[3];

    input_fd = open(src_file, O_RDONLY);
    if (input_fd == -1) {
        fprintf(stderr, "Error opening input file %s\n", src_file);
        exit(EXIT_FAILURE);
    }

    output_fd = open(dst_file, option, 0644);
    if (output_fd == -1) {
        fprintf(stderr, "Error opening output file %s\n", dst_file);
        exit(EXIT_FAILURE);
    }

    while ((bytes_read = read(input_fd, buffer, BUFFER_SIZE)) > 0) {
        bytes_written = write(output_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            fprintf(stderr, "Error writing to output file %s\n", dst_file);
            exit(EXIT_FAILURE);
        }
    }

    if (bytes_read == -1) {
        fprintf(stderr, "Error reading from input file %s\n", src_file);
        exit(EXIT_FAILURE);
    }

    if (close(input_fd) == -1) {
        fprintf(stderr, "Error closing input file %s\n", src_file);
        exit(EXIT_FAILURE);
    }

    if (close(output_fd) == -1) {
        fprintf(stderr, "Error closing output file %s\n", dst_file);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "move") == 0) {
        if (unlink(src_file) == -1) {
            fprintf(stderr, "Error removing input file %s\n", src_file);
            exit(EXIT_FAILURE);
        }
    }

    printf("%s operation successful.\n", argv[1]);
    exit(EXIT_SUCCESS);
}
