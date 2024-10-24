#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#define SIZE 128

// Function to display a message
void writeMessage(const char *message) {
    // Write the message to the standard output
    write(STDOUT_FILENO, message, strlen(message));
}

// Function to read user input
ssize_t readMessage(char *message, size_t size) {
    ssize_t readBytes = read(STDIN_FILENO, message, size - 1); // Read user input

    // Check if there's an input issue
    if (readBytes < 0) {
        writeMessage("Error: read\n");
        exit(EXIT_FAILURE);
    }

    // Null-terminate the string to avoid errors
    message[readBytes] = '\0';

    // Return the number of bytes read
    return readBytes;
}

void execute(char *message){
    // Create a child process
    pid_t pid = fork();

    // Check for errors
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    // Parent process
    else if (pid != 0) {
        int status;
        wait(&status);
    }

    else {
        execl("/bin/sh","sh","-c",message,(char*)NULL);
        /* execl is used to execute the current process with a new one we specify with other arguments.
        "/bin/sh" is the linux shell directory
        "sh" is the linux shell, it indicates to use the system shell
        "-c" indicates that the next input is a command it have to execute
        message is the new command that we type as an input in the shell
        */

        writeMessage("Error: execute\n"); // Error message displayed if not executed
        exit(EXIT_FAILURE);
    }
}

int main() {
    const size_t bufferSize = SIZE;  // Adjust of the size of the input command as needed
    char message[bufferSize];
    char *exit_message = "Bye bye...\n";

    // Display the welcome message at the beginning
    writeMessage("Welcome to ENSEA Tiny Shell.\nType 'exit' to quit.\n");

    // Main loop
    while (1) {
        // Display the shell prompt
        writeMessage("enseash % ");

        // Read the input message
        ssize_t readBytes = readMessage(message, bufferSize);

	// Exit function
        if(strncmp(message,"exit",4)==0){
            writeMessage(exit_message);
            break;
        }   

        // Execute the input message
        execute(message);
    }
    return EXIT_SUCCESS;
}

