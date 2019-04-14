## Shell
02/2019
***
### To Run
The shell can be created by running `make`, and run with the command ./ish

Note: compiling this program produces some compiler warning related to lex.c, which was provided by the course. I did not attempt to debug them.

The shell has the following built-in commands:
 exit - exits the shell at the end of the main loop
 gcd - prints the greatest common divisor of two numbers, in decimal or hex
 args - prints the number and list of arguments passed to the command
 datetime - custom new command that prints the current date and time in full

***

The program runs in a do-while loop that prints the command prompt, then uses
getln() to get and tokenize user input. The program identifies whether the input
contains a pipe character, or & in the final position. It checks whether the
command is empty (and does nothing), if the command is cd (does not fork), or if
the command is exit (which exits the shell). Otherwise, the shell forks.

In the parent fork, the shell uses waitpid to suspend itself until the child
finishes. If the command ended with &, the shell executes the process in the
background and does not block while the process runs. If the parent fork waits,
the shell returns error messages if the child process is killed or exits on
failure.

In the child fork, the program opens file names immediately after a < to read
or after a > to write, and removes the character from the command. If the
command is a builtin (args, gcd, datetime) it runs in a copy of the parent
process. If not, it uses execvp on the arguments, printing an error and returning
a failure if necessary. If the child process is not a positive number, the shell
also prints an error.

***
	datetime

A command that prints the full date and time
Details: takes 0 arguments
Output:
Today is Monday, February 4th, 2019
09:00 p.m.
