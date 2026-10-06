SimpleShell User Manual
CSC1021 Operating Systems

This is a simple shell written in C.

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




====== External Commands ======

If the command entered is not recognized as an internal command the shell will try to run it as a program from your system, like ls, cat, or sleep.

---For example:---

ls -l
cat file.txt
sleep 5

These commands will be executed using fork() and exec(), which means the shell creates a new process to run the program without closing itself.



====== I/O Redirection ======

SimpleShell supports redirecting input and output for commands.

Input redirection (<)
Use < to read input from a file instead of the keyboard.

----Example:----

sort < unsorted.txt




Output redirection (> and >>)
Use > to write output to a file (overwriting it).

----Example:----

ls > files.txt

This saves the output of ls into files.txt.

Use >> to append output to a file instead of overwriting it.

----Example:----

echo "Hello" >> log.txt

Notes:

Redirection only affects the command it is attached to, not the shell itself.
You can combine input and output redirection:
sort < unsorted.txt > sorted.txt
Background Execution

You can run commands in the background by adding & at the end of the command.

----Example:----

sleep 10 &

This starts the sleep program but does not block the shell — you can continue typing other commands while it runs.

If & is not used, the shell waits for the program to finish before showing the prompt again.

