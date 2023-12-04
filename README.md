2G1TP6 - DU Jack, JIN Clémentine

Synthesis TP - ENSEA in the Shell

Question 1/
Welcome message in Shell
-> Use of the "write()" process :
- import with #include <unistd.h>
- write(int fd, const void *buf, size_t count);
with fd, a file descriptor
     buf, a pointer to the memory location
     count, a size in bytes
  

Question 2/
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


Question 3/
We use the function strncmp() (int strncmp( const char * first, const char * second, size_t length );) function from the string.h library.
It compares the first and second string argument with the exact same "length" and return 0 if it's the same.

Question 4/
We return the previous 
