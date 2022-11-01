# Unix-Shell-Sim
A Unix Shell Simulator with 3 internal and 5 external commands, each with 2 options.

Normal Commands are executed using functions from external command files. The &t at the end of the command executes using execv command.

Internal Commands : ‘cd’, ‘echo’ and ‘pwd’.
External Commands : ‘ls’, ‘cat’, ‘date’, ‘rm’ and ‘mkdir’.

After splitting the user input command into an array of strings, this array is sent as the argument to individual external command (file) functions that are called in the main shell program.

