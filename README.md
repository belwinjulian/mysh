Belwin Julian (bj273)
Soma Parvathini (svp98)


# Project III: My Shell (mysh)
This command-line spell checker helps ensure the accuracy of your text files by cross-referencing them with a word dictionary. It can process single files or scan entire directories (including subdirectories). When it finds a misspelling, it reports the file, line number, column number, and the incorrect word.

#Test Cases

Test Case 1: Interactive and Batch Mode

Input: We inputted a text file as an arguments and nothing to make sure that the interactive and batch mode worked properly.
Expected Output: When the text file was inputted, there should be no "mysh>" or prompt the user for an input whereas the opposite would be true if no file was inputted.

Test Case 2: Built-in functions

Input: We tested cd, pwd, and which to make sure they worked properly.
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
- **Interactive and Batch Modes**: Detects mode using `isatty()` and adjusts behavior for command input accordingly. <br>
- **Command Execution**: Executes external programs, handles built-in commands (`cd`, `pwd`, `which`, `exit`), and processes wildcards for file names. <br>
- **Redirection and Pipelines**: Implements `<` and `>` for I/O redirection and `|` for connecting two processes. <br>
- **Wildcards**: Handles single asterisk patterns in file names for expansion. <br>
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
