// This C program performs encryption and decryption on a given file using a simple algorithm.
// It takes two command-line arguments:
// an operation flag (-e for encryption or -d for decryption)
// and the name of the file to be processed.
// Preston Duffield - April 12 2023 - CSCI 347

// Include necessary header files for the functions used in the program.
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <stdlib.h>

// Function to encrypt/decrypt the input buffer
void process_buffer(char *buffer, ssize_t size, int operation)
{
    for (int i = 0; i < size; i++)
    {
        buffer[i] = (operation == 1) ? buffer[i] + 100 : buffer[i] - 100;
    }
}

int main(int argc, char **argv)
{
    // check if the program received the correct number of command-line arguments and
    // if the operation flag is valid (-e or -d).
    // If not, display usage instructions and exit the program with an error code.
    if (argc != 3 || (strcmp(argv[1], "-e") != 0 && strcmp(argv[1], "-d") != 0))
    {
        printf("Usage:\n");
        printf("./filesec -e|-d [filename.txt]\n");
        return -1;
    }

    int operation = strcmp(argv[1], "-e") == 0 ? 1 : 0; // 1 for encrypt, 0 for decrypt
    char *input_file_name = argv[2];
    char output_file_name[128];

    // Extract the input file name from the command-line arguments and generate the output file name based on the input file name and the operation

    // Find the last dot in the input_file_name
    char *dot_position = strrchr(input_file_name, '.');
    // If a dot is found, calculate its position relative to the input_file_name
    size_t dot_index = dot_position ? dot_position - input_file_name : strlen(input_file_name);
    // Copy the first part of the input_file_name until the last dot
    snprintf(output_file_name, dot_index + 1, "%s", input_file_name);
    // Append the "_enc" or "_dec" suffix
    strcat(output_file_name, operation == 1 ? "_enc" : "_dec");

    // Append the remaining part of the input_file_name after the last dot (including the dot)
    if (dot_position)
    {
        strcat(output_file_name, dot_position);
    }

    int input_fd = open(input_file_name, O_RDONLY);
    if (input_fd < 0)
    {
        perror("Error opening input file");
        return -1;
    }

    int output_fd = open(output_file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output_fd < 0)
    {
        perror("Error opening output file");
        close(input_fd);
        return -1;
    }

    // Get the current time and store it in the start_time variable (used later to calculate the elapsed time).
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);

    // Initialize a buffer to read the input file in chunks and
    // variables to store the number of read/write calls and the number of bytes read/written.
    char buffer[1024];
    ssize_t read_count, write_count;
    int read_calls = 0, write_calls = 0;

    // Read the input file in chunks using a while loop.
    // In each iteration, read a chunk of data from the input file into the buffer,
    // process the buffer using the process_buffer function.
    while ((read_count = read(input_fd, buffer, sizeof(buffer))) > 0)
    {
        printf("Read %ld bytes from input file\n", read_count); // Add this line
        // Update the read/write call counters accordingly.
        read_calls++;

        process_buffer(buffer, read_count, operation);

        write_count = write(output_fd, buffer, read_count);
        if (write_count < 0)
        {
            perror("Error writing to output file");
            close(input_fd);
            close(output_fd);
            return -1;
        }
        printf("Wrote %ld bytes to output file\n", write_count); // Add this line
        write_calls++;
    }

    if (read_count < 0)
    {
        perror("Error reading from input file");
        close(input_fd);
        close(output_fd);
        return -1;
    }
    // Close both the input and output files after processing.
    close(input_fd);
    close(output_fd);

    // Calculate the elapsed time in microseconds.
    gettimeofday(&end_time, NULL);
    long elapsed_time = (end_time.tv_sec - start_time.tv_sec) * 1000000 + (end_time.tv_usec - start_time.tv_usec);

    // Print the number of read/write calls and the elapsed time in seconds.
    printf("Read calls: %d\nWrite calls: %d\nElapsed time: %ld.%06ld seconds\n", read_calls, write_calls, elapsed_time / 1000000, elapsed_time % 1000000);

    // Return 0 to indicate successful execution of the program.
    return 0;
}
