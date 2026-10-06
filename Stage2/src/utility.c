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
#include <sys/wait.h>
#include <fcntl.h>

#ifndef PATH_MAX  // indef prevents that the header to be used multiple times
#define PATH_MAX 1024
#endif

#include "simpleshell.h"

/* extern char **environ;
Gives access to all environment variables

strtok
Splits the input line into words, separated by spaces/tabs/newlines.
First token = the command

Command handling

Command	What it does
----quit----    Exits the shell
----clr----	    Clears the terminal using system("clear")
----cd----	    Changes directory with chdir(); prints PWD if no argument; updates PWD environment var
----dir----	    Lists directory contents using ls -al via system()
----environ----	Prints all environment variables one per line
----echo----	Prints all remaining tokens separated by space
----pause----	Waits for user to press Enter
----help----	Opens the manual using more (reads manual/readme.txt)
----unknown----	Prints "Unknown command: <input>" if the command isn’t recognized.*/

extern char **environ; // access to environment variables

#define MAX_ARGS 64

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

        // Get the next token directory argument
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
            sprintf(syscommand, "ls -al %s", dir);  // Build command: ls -al <directory>
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
            printf("%s\n", environ[i]); // Print each environment variable 
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
        getchar(); // Wait user to press Enter before continuing
    }

    /* help command */
    else if (strcmp(command, "help") == 0)
    {
        // Display the contents of manual/readme.txt using the 'more' pager
        // Allow user to scroll through the manual 
        system("more manual/readme.txt");
    }   

    // ===== External commands (fork + exec, I/O redirection, background) =====
else
{
    // split the input line into individual words/tokens.
    // tokens go into args[] which execvp() needs.
    char *args[MAX_ARGS];
    int i = 0;
    args[i++] = command;  // first token is the command itself ("ls")
    char *token;
    while ((token = strtok(NULL, " \t\n")) != NULL)
        args[i++] = token;  // add remaining tokens (arguments)
    args[i] = NULL; // terminate array with NULL because execvp expects it

    // flags for I/O redirection and background execution
    int redirect_in = -1;   // index of "<" if reading from a file
    int redirect_out = -1;  // index of ">" or ">>" if writing to a file
    int append_out = 0;     // flag for ">>" append mode
    int dont_wait = 0;      // flag for background execution "&"

    // Scan through args[] to find special symbols like <, >, >>, &
    for (int j = 0; args[j] != NULL; j++)
    {
        if (strcmp(args[j], "<") == 0 && args[j+1] != NULL)
        {
            redirect_in = j; // position of input file token
        }
        else if (strcmp(args[j], ">") == 0 && args[j+1] != NULL)
        {
            redirect_out = j; // position of output file token
            append_out = 0;   // normal overwrite mode
        }
        else if (strcmp(args[j], ">>") == 0 && args[j+1] != NULL)
        {
            redirect_out = j; // position of output file token
            append_out = 1;   // append mode
        }
        else if (strcmp(args[j], "&") == 0)
        {
            dont_wait = 1;    // mark as background process
            args[j] = NULL;   // remove & from args so execvp doesn't see it
        }
    }

    // create a new process using fork()
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork failed"); // fork returns <0 if error
        return;
    }

    if (pid == 0) // CHILD process
    {
        // CHILD handle input/output redirection before running the command

        // Input redirection "<"
        if (redirect_in > 0)
        {
            freopen(args[redirect_in + 1], "r", stdin); // replace stdin with file
            args[redirect_in] = NULL; // remove "< filename" from args
        }

        // Output redirection ">" or ">>"
        if (redirect_out > 0)
        {
            if (append_out)
                freopen(args[redirect_out + 1], "a", stdout); // append to file
            else
                freopen(args[redirect_out + 1], "w", stdout); // overwrite file
            args[redirect_out] = NULL; // remove "> filename" from args
        }

        // Execute the actual command (like "ls", "cat", "sleep", etc.)
        execvp(args[0], args); // replace child process with new program
        perror("exec failed");  // if execvp returns something went wrong
        exit(1);                // exit child if exec failed
    }
    else // PARENT process
    {
        // If command not running in background wait for the child to finish
        if (!dont_wait)
        {
            int status;
            waitpid(pid, &status, 0); // parent waits until child is done
        }
        // If dont_wait is 1 parent continues immediately 
    }
}
/*
Name: Maxim Circiumaru
Student Number: 35644
I confirm this work complies with the DCU Academic Integrity Policy.
*/