/*
Name: Maxim Circiumaru
Student Number: 35644

CSC1021 Operating Systems
SimpleShell - Stage 1
*/

#include <stdio.h>  /* input/output functions (printf, fgets, etc.) */
#include <stdlib.h>  /* general utilities (exit, setenv) */
#include <string.h>  /* string functions (strtok, strcmp) */
#include <unistd.h>  /* UNIX standard functions (chdir, getcwd) */
#include <limits.h>  /* PATH_MAX definition */

#ifndef PATH_MAX
#define PATH_MAX 1024  /* header file */
#endif

#include "simpleshell.h"  /* includes the  code from simpleshell.h */

int main(int argc, char *argv[])
{
    char line[MAX_LINE];  // Stores user input command
    char cwd[PATH_MAX];     // Stores current working directory for prompt
    FILE *input = stdin;    // // Default input is keyboard (interactive mode)


    /* Batch mode */  

    /* If the shell is run with a filename open the file for reading commands instead of waiting for user input.
open(argv[1], "r") opens the batch file.
If it fails, the shell prints an error and exits.*/

    if (argc > 1)
    {
        input = fopen(argv[1], "r");

        if (input == NULL)
        {
            perror("Error opening batch file");
            exit(1);
        }
    }



    /* Set shell environment variable */
    /* realpath(argv[0], path) finds the full path to the shell executable.
setenv("shell", path, 1)  sets shell=/full/path/to/simpleshell.*/

    char path[PATH_MAX];
    realpath(argv[0], path);     // Get full path of this program
    setenv("shell", path, 1);   // Add shell=/full/path to environment


    // main loop
    while (1)
    {
        /* Show prompt only for interactive mode */
        if (input == stdin)
        {
            getcwd(cwd, sizeof(cwd));
            printf("%s > ", cwd);
        }

        /* Read command */
        if (fgets(line, MAX_LINE, input) == NULL)
            break;

        execute_command(line);
    }

    return 0;
}

/*
Name:
Student Number: 35644
I confirm this work complies with the DCU Academic Integrity Policy.
*/