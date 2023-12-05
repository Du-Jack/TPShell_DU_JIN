2G1TP6 - DU Jack, JIN Clémentine

###Synthesis TP - ENSEA in the Shell###

**Question 1/**
Welcome message in Shell
Use of the "write()" process :
- import with #include <unistd.h>
- write(int fd, const void *buf, size_t count);
with fd, a file descriptor
     buf, a pointer to the memory location
     count, a size in bytes
  

**Question 2/**
We have to code the REPL process : read - eval - print loop.
1. read : we create a "readMessage" function which have 2 parameters (char *message, size_t size)
The size is defined by a maximum BUFSIZE.
The message is the input written by the user.

we use the read(int fd, const void *buf, size_t count) process 
with :  fd : STDIN_FILENO, a standard input descriptor
	const void *buf : message, the input written by the user
	count : the input size  

2. execute : 
We create here a child process and if the process is the parent, it waits the child to end using wait. If it's the child process, it executes the function execl to execute the command.

The execl is defined as :
int execl (const char *path, const char *arg, ...) ; with execXY : defined as X=l, Y=None.

With X=l, the args are taken into account 1by1 and ends with the (Char*)NULL argument.
The arguments here are :
- "/bin/sh" which is the system shell path. -> execl will replace the current process with the shell executable. 
- "sh" : first argument which is the system shell call name.
- "-c" : second argument which indicates that the next argument is a command to be executed.
- message : is the command to be executed
- "(char*)NULL" indicates to be the last argument of execl.

3. print loop : with a while(1) in the main function.


**Question 3/**
We use the function strncmp() (int strncmp( const char * first, const char * second, size_t length );) function from the string.h library.
It compares the first and second string argument with the exact same "length" and return 0 if it's the same.

**Question 4/**
We return the code or signal of the previous program in the prompt.

1. To know if the program ended because of a signal, we use an if condition : 
- WIFEXITED(status) == true if the child ended normally
- WIFSIGNALED(status) == true if the child ended because of a signal

And WTERMSIG(status) return the number of the signal which ended the process.

2. Display the message.
To avoid using the printf function which could make issues with read()/write() functions, we have used snprintf which stores the string to display in a C string instead of printing it. Then, we write it with writeMessage function we made.

**Question 5/**
Measurement of the command execution time using the call clock_gettime

To measure the command execution time, we use the clock_gettime function from the time.h library. The base time we use is CLOCK_REALTIME, a clock from Linux kernel.
- clock_gettime(CLOCK_REALTIME, &start) launch the timer count.
- clock_gettime(CLOCK_REALTIME, &end) end the timer count
We then calculate to get the time in ms (which is the order of magnitude of cmd execution).

Then, we display it by adding a "elapsedTime" parameters in functions we use.

**Question 6/**
Execution of a complex command (with arguments)

To execute a complex command with different arguments, we have to differentiate the command from its arguments. To make that, we have used tokens to differentiate arguments separated by a " " and added them into a list.
Then, we have used execvp() function. All arguments are written once in the (char **) and the last argument is (char*)NULL.

**Question 7/**
Management of redirections to stdin and stdout with ’<’ and ’>’ ;

To manage redirections to stdin and stdout, we create a function redirection which takes into account "<" and ">" and removes it from the argument list to make the question 6 functional.
Then, to take info from the file, we use the open() function. 

Input redirection : "<"
- We replace "<" with "NULL" to erase it. 
- We open the file with read mode only. (O_RDONLY)
- We copy the standard input STDIN_FILENO to the file with **dup2** to the opened file. 
- We close it with close(fd).

Output redirection : ">"
- We replace ">" with "NULL" to erase it. 
- We open the file with read & write mode (O_WRONLY) by creating it (O_CREAT) if it does not exist or we make it shorter (O_TRUNC) if it does. 
- We copy the standard output STDOUT_FILENO to the file with **dup2** to the new file. 
- We close it with close(fd).