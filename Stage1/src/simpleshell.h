#ifndef SIMPLESHELL_H /* ifndef  is a header guard — it prevents this header from being included multiple times. */
#define SIMPLESHELL_H

#define MAX_LINE 1024 // Maximum length of input line from user or batch file

void execute_command(char *line); // Function to parse and execute a single command line
#endif


/* Essentially, this file tells the compiler what functions and constants exist in the shell. */