Belwin Julian (bj273)
Soma Parvathini (svp98)


# Project III: My Shell (mysh)
A simple command-line shell we designed for the CS 214 course project which should simulate the capability to interpret and execute commands like bash or zsh. The shell we created should be capable of supporting both interactive and batch modes.
#Test Cases

Test Case 1: Interactive and Batch Mode

Input: We inputted a text file as an arguments and nothing to make sure that the interactive and batch mode worked properly.
Expected Output: When the text file was inputted, there should be no "mysh>" or prompt the user for an input whereas the opposite would be true if no file was inputted.

Test Case 2: Built-in functions

Input: We tested cd, pwd, exit and which to make sure they worked properly. 
Expected Output: depending on which built-in function we used, it should have outputted the correct result. cd changed directory. pwd shows the directory you are in. which finds the path of an executable.

Test Case 3: External Functions

Input: Made sure that most external functions worked such as ls, echo, and cat.
Expected Output: The external functions should what they are expected of with execvp().

Test Case 4: Tokenization

Input: We had to make sure that foo bar > baz and foo bar>baz would tokenize properly. This was a problem because the strtok() method we used only separated based on the delimiters " " and "\n". We had to make a new function for this.
Expected Output: They both should tokenize to the same thing which should be "foo", "bar", ">", and "baz"

Test Case 5: Redirection

Input: We need to make sure both redirection ">" and "<" worked.
Expected Output: Successful redirection should result in data flowing according to the direction indicated by the arrows. 

Test Case 6: Pipes

Input: We need to make sure "|" (pipe) worked.
Expected Output: The output should have been the input of the second part of the pipe.

Test Case 7: Wildcard

Input: We need to make sure the wildcard '*' worked as intended.
Expected Output: All the possible values which have the characters beginning and after the '*'.


# Features
- **Interactive and Batch Modes**: Detects mode using `isatty()` function. Interactive mode allows user to interact with the interface by typing. Batch mode takes input from text files. <br>
- **Command Execution**: Executes external programs and commands, should be able to handle built-in commands such as `cd`, `pwd`, `which`, `exit`, and processes wildcards to get every file with names that have letter before and after '*'. <br>
- **Redirection and Pipelines**: Using `<` and `>` for I/O redirection and `|` for connecting two processes (output to input). <br>
- **Wildcards**: Handles single asterisk patterns in file names for expansion before and after asterisk. <br>
- **Conditional Execution**: Allows for conditional command execution based on the success or failure of previous commands. <br>
  
## Usage
Launch in interactive mode by running without arguments, or in batch mode by specifying a script file or piping input.
# Compilation
 - To compile the spell checker, use the command as follows:  <br>
**gcc -o spellchecker spchk.c**

### Interactive Mode
- $ ./mysh
Welcome to my shell!
mysh> [Enter command here]

### Batch Mode
- $ ./mysh [script file]
or -
$ cat [script file] | ./mysh

## Built-in Commands
- `cd [directory]`: Changes the shell's current directory.
- `pwd`: Displays the shell's current directory.
- `which [program]`: Searches for a program in the system's PATH.
- `exit`: Exits the shell.

## Redirection and Pipelines
- Use `<` for redirecting input, `>` for redirecting output.
- Use `|` to connect the output of one command as the input to another.

## Wildcards
- Supports patterns with `*` in the filename, matching files in the same directory.

## Testing Strategy
Describe your approach to testing, including specific scenarios and methods used to verify functionality.

## Compiling and Running
- Compile: Use `make` or manually compile with the appropriate compiler command.
- Run: Launch `./mysh` for interactive mode or specify a script file for batch mode.
