SimpleShell User Manual
CSC1021 Operating Systems

This is a simple command line shell written in C.

The shell displays the current working directory as the prompt and allows the user to enter commands.

Internal Commands

----cd <directory>----
<<Changes the current directory to <directory>.
If no directory is specified    the current directory is displayed.>>

----clr----
<<Clears the screen.>>


----dir <directory>----
<<Lists the contents of the specified directory using the command ls -al.
If no directory is provided, the current directory contents are displayed.>>


----environ----
<<Displays all environment variables available to the shell.>>


----echo <text>----
<<Displays the text entered after the echo command.>>

----help----
<<Displays this user manual using the more command.>>

----pause----
<<Pauses the shell until the user presses Enter.>>

----quit----
<<Exits the shell.>>

----Batch Mode----

<<The shell can read commands from a file.

Example:
./bin/simpleshell batchfile.txt

Each line in the file will be executed as a command.
When the end of the file is reached, the shell exits.>>