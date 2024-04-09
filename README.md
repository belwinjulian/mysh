Belwin Julian (bj273)
Soma Parvathini (svp98)


# Project III: My Shell (mysh)
This command-line spell checker helps ensure the accuracy of your text files by cross-referencing them with a word dictionary. It can process single files or scan entire directories (including subdirectories). When it finds a misspelling, it reports the file, line number, column number, and the incorrect word.


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
