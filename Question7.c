#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>
#define SIZE 128


// Function to display a message
void writeMessage(const char *message) {
    // Write the message to the standard output
    write(STDOUT_FILENO, message, strlen(message));
}

// Function to display status
void convertMessage(const char *output, int val, float elapsedTime) {
    // Write the message to the standard output
    char message[SIZE]; 
    snprintf(message, sizeof(message), "enseash [%s:%d|%fms] %% \n", output, val, elapsedTime);
    writeMessage(message);
}

// Display the return code/signal of the previous command and the shell prompt
void displayPreviousCommand(float elapsedTime){
    int status;

    wait(&status);

    if (WIFEXITED(status)){
        convertMessage("exit", WEXITSTATUS(status), elapsedTime);
    }

    else if (WIFSIGNALED(status)){
        convertMessage("sign", WTERMSIG(status), elapsedTime);
    }

    writeMessage("enseash % ");
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
    message[strcspn(message, "\n")] = '\0';

    // Return the number of bytes read
    return readBytes;
}

// Function to manage complex command with arguments 
void tokenize(char *message, char *args[], size_t *argc) {
    // break down into tokens (~word/argument)
    char *token = strtok(message, " ");
    while (token != NULL) {
        args[(*argc)++] = token;
        token = strtok(NULL, " ");
    }
    args[*argc] = NULL;
}

// Function to manage redirections
void redirection(char *args[], int argc){

    char *input = NULL;
    char *output = NULL;

    // Check for input and output redirection
    for (size_t i = 0; i < argc; i++) {
        
        // Input redirection
        if (strcmp(args[i], "<") == 0) {
            input = args[i + 1];
            args[i] = NULL; 

            // Open the input file for reading
            int fd = open(input, O_RDONLY);
            if (fd == -1) {
                perror("Error: handleRedirection (Input)\nopen");
                exit(EXIT_FAILURE);
            }

            // Redirect standard input to the file
            if (dup2(fd, STDIN_FILENO) == -1) {
                perror("Error: handleRedirection (Input)\ndup2");
                close(fd);
                exit(EXIT_FAILURE);
            }
            
            close(fd);
        }
        
        // Output redirection
        else if (strcmp(args[i], ">") == 0) {
            output = args[i + 1];
            args[i] = NULL; 

            // Open the output file for writing
            int fd = open(output, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
            if (fd == -1) {
                perror("Error: handleRedirection (Output)\nopen");
                exit(EXIT_FAILURE);
            }

            // Redirect standard output to the file
            if (dup2(fd, STDOUT_FILENO) == -1) {
                perror("Error: handleRedirection (Output)\ndup2");
                close(fd);
                exit(EXIT_FAILURE);
            }
            
            close(fd);
        }
    }
}



// Execute function
double execute(char *message){
    struct timespec start, end;
    float elapsedTime = 0;
    
    // Get the start time 
    if (clock_gettime(CLOCK_REALTIME, &start) == -1) {
        perror("clock_gettime");
        exit(EXIT_FAILURE);
    }

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
        char *args[SIZE];
        size_t argc = 0;

        tokenize(message, args, &argc);
        redirection(args, argc);
        execvp(args[0], args);

        // Error message displayed if not executed
        writeMessage("Error: execute\n"); 
        exit(EXIT_FAILURE);     
    }

    // Get the end time
    if (clock_gettime(CLOCK_REALTIME, &end) == -1) {
        perror("clock_gettime");
        exit(EXIT_FAILURE);
    }

    // Calculate the execution time
    elapsedTime = ((end.tv_sec - start.tv_sec)*1000) + (end.tv_nsec - start.tv_nsec) / 1e6;

    return elapsedTime;
}

// Manage the exit
void exitFunction(char *message, ssize_t readBytes){
    if(strncmp(message,"exit",4)==0 || readBytes == 0){ // 'exit' or <ctrl>+d
        writeMessage("Bye bye...\n");
        exit(EXIT_SUCCESS);
    } 
}



int main() {
    const size_t bufferSize = SIZE;  // Adjust of the size of the input command as needed
    char message[bufferSize];
    float elapsedTime = 0;
    
    // Display the welcome message at the beginning
    writeMessage("Welcome to ENSEA Tiny Shell.\nType 'exit' to quit.\n");

    // Main loop
    while (1) {

        // Display the return code/signal of the previous command and the shell prompt
        displayPreviousCommand(elapsedTime);

        // Read the input message
        ssize_t readBytes = readMessage(message, bufferSize);

        // Execute the input message and manage the exit
        exitFunction(message, readBytes);
        elapsedTime = execute(message);
    }
    return EXIT_SUCCESS;
}

