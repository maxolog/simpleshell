/*
Name: Maxim Circiumaru
Student Number: 35644

CSC1021 Operating Systems
Utility functions for SimpleShell
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>  // PATH_MAX

#ifndef PATH_MAX  // indef prevents that the header to be used multiple times
#define PATH_MAX 1024
#endif

#include "simpleshell.h"

/* extern char **environ;
Gives access to all environment variables

strtok
Splits the input line into words, separated by spaces/tabs/newlines
First token = the command

Command handling

Command	What it does
----quit----    Exits the shell
----clr----	    Clears the terminal using system("clear"
----cd----	    Changes directory with chdir(); prints PWD if no argument; updates PWD environment var
----dir----	    Lists directory contents using ls -al via system()
----environ----	Prints all environment variables one per line
----echo----	Prints all remaining tokens separated by space
----pause----	Waits for user to press Enter
----help----	Opens the manual using more (reads manual/readme.txt)
----unknown----	Prints "Unknown command: <input>" if the command isn’t recognizedgit*/

extern char **environ; // Gives access to environment variables

void execute_command(char *line)
{
    // Get first token (command) from input line
    char *command = strtok(line, " \t\n"); 


    // Ignore empty lines
    if (command == NULL)
        return;


    // ===== Handle each internal command =====

    /* quit command */
    if (strcmp(command, "quit") == 0)
    {
        exit(0); // Quit the shell
    }

    /* clear screen */
    else if (strcmp(command, "clr") == 0)
    {
        system("clear"); // Clear terminal screen
    }

    /* change directory */
    else if (strcmp(command, "cd") == 0)
    {

        // Get the next token (directory argument)
        char *dir = strtok(NULL, " \t\n");

        if (dir == NULL) // No argument provided
        {
            char cwd[PATH_MAX];
            getcwd(cwd, sizeof(cwd));
            printf("%s\n", cwd); // Print current directory
        }
        else
        {
            if (chdir(dir) != 0)
            {
                perror("cd error");  // Invalid directory
            }
            else
            {
                char cwd[PATH_MAX];
                getcwd(cwd, sizeof(cwd)); // Update current directory
                setenv("PWD", cwd, 1);  // Update PWD environment variable
            }
        }
    }
/* list directory contents */
else if (strcmp(command, "dir") == 0)
{
    // Get optional directory argument (next token from input)
    char *dir = strtok(NULL, " \t\n");

    if (dir == NULL)
    {
        // No argument provided, list current directory contents
        // -al option: show all files including hidden, long format
        system("ls -al");
    }
    else
    {
        // Directory argument provided, list its contents
        char syscommand[1024];                  // Buffer to hold system command
        sprintf(syscommand, "ls -al %s", dir);  // Build command: "ls -al <directory>"
        system(syscommand);                     // Execute command in shell
    }
}

/* display environment variables */
else if (strcmp(command, "environ") == 0)
{
    int i = 0;

    // Loop through the environment array until NULL terminator
    while (environ[i] != NULL)
    {
        printf("%s\n", environ[i]); // Print each environment variable (NAME=value)
        i++;
    }
}

/* echo command */
else if (strcmp(command, "echo") == 0)
{
    char *text;

    // Loop through all remaining tokens and print them separated by space
    while ((text = strtok(NULL, " \t\n")) != NULL)
    {
        printf("%s ", text);
    }

    printf("\n"); // Add newline at the end of the echoed text
}

/* pause command */
else if (strcmp(command, "pause") == 0)
{
    printf("Shell paused. Press Enter to continue..."); // Inform user
    getchar(); // Wait for user to press Enter before continuing
}

/* help command */
else if (strcmp(command, "help") == 0)
{
    // Display the contents of manual/readme.txt using the 'more' pager
    // Allows user to scroll through the manual if it's long
    system("more manual/readme.txt");
}

/* unknown command */
else
{
    // If command does not match any internal commands
    printf("Unknown command: %s\n", command); // Inform the user
}
/*
Name: Maxim Circiumaru
Student Number: 35644
I confirm this work complies with the DCU Academic Integrity Policy.
*/