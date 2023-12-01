#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define BUFSIZE 128

int main(){
    char *message = "Welcome to ENSEA Tiny Shell. \nType 'exit' to quit. \nenseash % \n";
    write(1, message, strlen(message));
    // fd : STDOUT_FILENO == 1, STDIN_FILENO == 0, STRERR_FILENO ==2
    }

