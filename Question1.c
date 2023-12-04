#include <unistd.h>
#include <stdio.h>
#include <string.h>
#define SIZE 128

// Function to display a message
void writeMessage(const char *message) {
    // Write the message to the standard output
    write(STDOUT_FILENO, message, strlen(message));
}

int main(){
    writeMessage("Welcome to ENSEA Tiny Shell.\nType 'exit' to quit.\n");
    writeMessage("enseash % \n");
    }

