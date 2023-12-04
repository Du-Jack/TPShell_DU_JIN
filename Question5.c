#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
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

double execute(char *message){
    struct timespec start, end;
    float elapsedTime = 0;
    
    // Get the start time    
    //clock_gettime(CLOCK_REALTIME, &start);

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

    // Get the end time
    if (clock_gettime(CLOCK_REALTIME, &end) == -1) {
        perror("clock_gettime");
        exit(EXIT_FAILURE);
    }
    // Calculate the execution time
    elapsedTime = ((end.tv_sec - start.tv_sec)*1000) + (end.tv_nsec - start.tv_nsec) / 1e6;

    return elapsedTime;
}


// Function to display status
void convertMessage(const char *output, int val, float elapsedTime) {
    // Write the message to the standard output
    char message[SIZE]; 
    snprintf(message, sizeof(message), "enseash [%s:%d|%fms] %% \n", output, val, elapsedTime);
    writeMessage(message);
}

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


void exitFunction(char *message, ssize_t readBytes){
    if(strncmp(message,"exit",4)==0 || readBytes == 0){ // 'exit' or <ctrl>+d
        writeMessage("Bye bye...\n");
        exit(EXIT_SUCCESS);
    } 
}

int main() {
    const size_t bufferSize = 128;  // Adjust of the size of the input command as needed
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


