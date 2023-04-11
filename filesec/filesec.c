#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <stdlib.h>

// Function to encrypt/decrypt the input buffer
void process_buffer(char *buffer, ssize_t size, int operation) {
    for (int i = 0; i < size; i++) {
        buffer[i] = (operation == 1) ? buffer[i] + 100 : buffer[i] - 100;
    }
}

int main(int argc, char **argv) {
    if (argc != 3 || (strcmp(argv[1], "-e") != 0 && strcmp(argv[1], "-d") != 0)) {
        printf("Usage:\n");
        printf("./filesec -e|-d [filename.txt]\n");
        return -1;
    }

    int operation = strcmp(argv[1], "-e") == 0 ? 1 : 0; // 1 for encrypt, 0 for decrypt
    char *input_file_name = argv[2];
    char output_file_name[128];

    snprintf(output_file_name, sizeof(output_file_name), "%s_%s.txt",
             input_file_name, operation == 1 ? "enc" : "dec");

    int input_fd = open(input_file_name, O_RDONLY);
    if (input_fd < 0) {
        perror("Error opening input file");
        return -1;
    }

    int output_fd = open(output_file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output_fd < 0) {
        perror("Error opening output file");
        close(input_fd);
        return -1;
    }

    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);

    char buffer[1024];
    ssize_t read_count, write_count;
    int read_calls = 0, write_calls = 0;

    while ((read_count = read(input_fd, buffer, sizeof(buffer))) > 0) {
        read_calls++;

        process_buffer(buffer, read_count, operation);

        write_count = write(output_fd, buffer, read_count);
        if (write_count < 0) {
            perror("Error writing to output file");
            close(input_fd);
            close(output_fd);
            return -1;
        }
        write_calls++;
    }

    if (read_count < 0) {
        perror("Error reading from input file");
        close(input_fd);
        close(output_fd);
        return -1;
    }

    close(input_fd);
    close(output_fd);

    gettimeofday(&end_time, NULL);
    long elapsed_time = (end_time.tv_sec - start_time.tv_sec) * 1000000 + (end_time.tv_usec - start_time.tv_usec);

    printf("Read calls: %d\nWrite calls: %d\nElapsed time: %ld.%06ld seconds\n", read_calls, write_calls, elapsed_time / 1000000, elapsed_time % 1000000);

    return 0;
}
