#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <glob.h>

#define MAX_ARGS 64
#define MAX_LINE 1024
//i dont know if can use maximum line and arguments but I put it because most websites say this should be the maximum of the shell

// Forward declarations because I wrote the main function at the beginning and am too lazy to rearrange the order of the functions
void tokenize(char *line, int mode);
int command(char **args, int mode);
void redirection_and_piping(char **args);
int redirection(char **args);
void handle_single_command(char **args);
void expand_wildcard(char **args);
char* add_spaces(const char *str);
int execute_conditional(char **args);

int stat = 0; 


int main(int argc, char *argv[]) {
    int saved_stdin = dup(STDIN_FILENO);
    int saved_stdout = dup(STDOUT_FILENO);
    char line[MAX_LINE + 1]; //explicitly need to make space for the null terminator so i added one more
    ssize_t num_read; //bytes read by read() later

    int fd = STDIN_FILENO; //sets file descriptor to 0 which is the standard input

    if (argc == 2) {  //if there are more than one arguments that means that input file was given
        fd = open(argv[1], O_RDONLY);

        if (fd == -1) { //error checking
            perror("open");
            return EXIT_FAILURE;
        }

        dup2(fd, STDIN_FILENO); //makes the input file as the standard input
        close(fd); //close file descriptor because we already made the input file the standard input which should be file descriptor 0

    }

     int mode = isatty(STDIN_FILENO); //outputs 1 if the standard input is the terminal if not outputs 0

    if(mode) {
    printf("Welcome to our shell!\n"); //he said we needed some greeting
    }

    while (1) {
        if (mode) {
            printf("mysh> ");
            fflush(stdout); // not 100% needed just prevents input bugs
        }

        num_read = read(fd, line, MAX_LINE); // Read up to MAX_LINE characters and puts it into line char array
        //num_read gets the number of bytes that were read and copied into line

        if (num_read <= 0) { // End-of-file encountered or error
            if (mode && num_read == 0) {
                printf("\nExiting from our shell.\n");
            }
            break;
        }

        line[num_read] = '\0'; // puts a null-terminator at the end of the string right after \n so it can be a proper string to pass through strchr functino

        char *newline = strchr(line, '\n'); //when enter is pressed \n is added at the end the line after the null terminator
        //newline has the pointer address of the \n element which should be at the end of the line

        if (newline != NULL) {
            *newline = '\0'; //only get the user input without the \n
        } else if (num_read == MAX_LINE) {

            char clear = getchar(); //to use to clear out rest of line after buffer

            while (clear != '\n' && clear != -1){ //as long as clear does not equal new line or end of file

                clear = getchar();

            } //clear it out one by one because getchar reads the next character in standard input
        }

        if (line[0] == '\0') {

            continue; // Skip lines that have nothing

        }

        tokenize(line, mode);
        dup2(saved_stdin, STDIN_FILENO);
        dup2(saved_stdout, STDOUT_FILENO);
    }

    //figure out how to not close terminal to avoid bugs

    return 0;
}
char* add_spaces(const char *str) {

    if(str == NULL) {
        return NULL;
    }

    int nSize = 1;
    int x = 0;

    while(str[x] != '\0') {

        if(str[x] == '>' || str[x] == '<' || str[x] == '|') {

            if(x > 0 && str[x-1] != ' ') {
                nSize++;
            }

            if(str[x+1] != '\0' && str[x+1] != ' ') {
                nSize++;
            }
        }
        nSize++;
        x++;
    }

    char* nStr = (char*)malloc(nSize);

    if(nStr == NULL) {
        return NULL;
    }

    
    int i = 0;
    int j = 0; 

    while(str[i] != '\0') {
        
        if ((str[i] == '>' || str[i] == '<' || str[i] == '|') && i > 0 && str[i-1] != ' ') {
            nStr[j] = ' ';
            j++;
        }
        
        nStr[j] = str[i];
        j++;

      
        if ((str[i] == '>' || str[i] == '<' || str[i] == '|') && str[i+1] != '\0' && str[i+1] != ' ') {
            nStr[j] = ' ';
            j++;
        }
        i++;
    }

    nStr[j] = '\0';

    return nStr;
}

void tokenize(char *line, int mode) {

    char *split_line[MAX_ARGS]; //line split up into a bunch of tokens
    int num = 0; 


//need to make code to check and add spaces before redirections and pipes
 line = add_spaces(line);



    char *token = strtok(line, " \n"); //first token

    while (token != NULL && num < MAX_ARGS - 1) {

        split_line[num++] = token;

        //token = strtok(line, " \n"); does not work
        

        token = strtok(NULL, " \n"); //this line should be correct

        //printf("token test: %s", token);

    }
    split_line[num] = NULL; // Null-terminate the array (important for exec() functions)

   
    int i = 0;
     if (split_line[0] != NULL) {
        expand_wildcard(split_line);

        for (i = 0; split_line[i] != NULL; i++) {
            if (strcmp(split_line[i], "|") == 0) {
                redirection_and_piping(split_line);
                return;//theres a pipe, so it was handled already
            }
    
        }
        redirection_and_piping(split_line);
        command(split_line, mode);//there was only a redirection, so we need to still run command


    }
    
            
}



  //  if (split_line[0] != NULL) {
    //   command(split_line, mode);
    //}


void expand_wildcard(char **args) {
    int i = 0;
    // Calculate the current size of args
    int argc;
    for (argc = 0; args[argc] != NULL; argc++);

    while (args[i] != NULL) {
        if (strchr(args[i], '*') != NULL) {
            glob_t globbuf;
            memset(&globbuf, 0, sizeof(globbuf));
            // Use GLOB_NOCHECK to return the pattern if no matches are found
            int result = glob(args[i], GLOB_NOCHECK, NULL, &globbuf);

            if (result == 0 && globbuf.gl_pathc > 0) {
                // Calculate the new size for args
                int new_argc = argc - 1 + globbuf.gl_pathc; // Replace one arg with glob matches

                // Reallocate args with the new size
                //*args = (char **)realloc(*args, (new_argc + 1) * sizeof(char *)); // +1 for NULL terminator

                // Shift the existing arguments to make space for the new glob results
                memmove(&args[i + globbuf.gl_pathc], &(args[i + 1]), (argc - i) * sizeof(char *));

                // Copy the glob results into args
                for (size_t j = 0; j < globbuf.gl_pathc; j++) {
                    args[i + j] = strdup(globbuf.gl_pathv[j]);
                }

                // Update counters according to the new size
                argc = new_argc;
                i += globbuf.gl_pathc; // Move past the newly added args
            } else {
                // If no matches found, just go to the next arg
                i++;
            }

            globfree(&globbuf);
        } else {
            // No wildcard in the current arg, move to the next one
            i++;
        }
    }

    // Ensure args is NULL-terminated
    args[argc] = NULL;
}


//cd    pwd     exit   which
int command(char **args, int mode) {

      if (args[0] != NULL && (strcmp(args[0], "then") == 0 || strcmp(args[0], "else") == 0)) {
        if (!execute_conditional(args)) {
            return stat; // Maintain the last command's status and skip execution
        }
        // If the conditional command is to be executed, shift the args array to remove the conditional token
        int i;
        for (i = 0; args[i+1] != NULL; i++) {
            args[i] = args[i+1];
        }
        args[i] = NULL; // Null-terminate the modified args array
    }
    

    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            perror("cd: no filename");
            stat = 1;
        }
    
    int s = chdir(args[1]);

        if (s != 0) {
            perror("cd: such file or directory");
            stat = 1;
        }

        return 0;

    } else if (strcmp(args[0], "pwd") == 0) {
        char *dir = getcwd(NULL, 0); //gets the directory which we are in

        if (dir != NULL) {

            printf("%s\n", dir);

        } else {
            perror("pwd: could not get directory path");
            stat = 1;
        }

        return 0;

    } else if (strcmp(args[0], "exit") == 0) {

        printf("Exiting from our shell.\n");

        exit(EXIT_SUCCESS); //should exit the shell with the message

    } else if (strcmp(args[0], "which") == 0) {

       if (args[1] != NULL) {

        char *path = getenv("PATH");
        //printf("\nenv path: %s", path);

        char *token = strtok(path, ":");
         //printf("\ntoken path: %s", path);

        if(token != NULL) {

            char fPath[MAX_LINE] = ""; //full path of executable
            
            strcpy(fPath, token);
            strcat(fPath, "/");
            strcat(fPath, args[1]);

            //should be the entire path of the executable including the executable

            printf("%s\n", fPath);

            return 0;
        }

            printf("Command not found\n"); //only comes here if the file is unaccessible or unexecutable
            stat = 1;

        } else {

            printf("which: missing argument\n"); 

        }

        return stat;

    } else {

        handle_single_command(args);

    }

    return 0; 
}

void handle_single_command(char **args) {
    pid_t pid = fork();

    if (pid == 0) { // Child Process

        if (execvp(args[0], args) == -1) {
            stat = 1;
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    } else if (pid < 0) {
        perror("fork");
    } else { // Parent process
        wait(NULL); 

        
    }
}

void redirection_and_piping(char **args) {
   
     int i, pipe_index = -1;

    // Find pipe and redirection symbols, handle redirection
    for (i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "|") == 0) {
            pipe_index = i;
            args[i] = NULL;  // Replace '|' with NULL
        } else if (strcmp(args[i], "<") == 0 || strcmp(args[i], ">") == 0) {
            if (redirection(args) != 0) {
                return; 
            }
            i--; // Adjust index for removed elements 
        }
    }

     if (pipe_index == -1) {
        return; 
    }


         // Create a single pipe
    int pipefd[2];
    if (pipe(pipefd) < 0) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }


    // Fork the first process
    pid_t pid1 = fork();
    if (pid1 == 0) {  // First child process
        close(pipefd[0]); // Close unused read end 
        dup2(pipefd[1], STDOUT_FILENO); 
        close(pipefd[1]);
        execvp(args[0], args);
        perror("execvp (first command)");
        exit(EXIT_FAILURE);
    }
    else if (pid1 < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }



     // Fork the second process
    pid_t pid2 = fork();
    if (pid2 == 0) { // Second child process
        close(pipefd[1]); // Close unused write end
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        execvp(args[pipe_index + 1], args + pipe_index + 1);
        perror("execvp (second command)");
        exit(EXIT_FAILURE);
    } else if (pid2 < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    close(pipefd[0]);
    close(pipefd[1]);

    wait(NULL);
    wait(NULL);
}



int redirection(char **args) {
    int i = 0;
    while (args[i] != NULL) {
        if (strcmp(args[i], "<") == 0) {
            // Input redirection
            if (args[i + 1] == NULL) {      //no argument after < 
                perror("mysh: redirection error: no input file\n");
                return 1;
            }
            int fd = open(args[i + 1], O_RDONLY);
            if (fd == -1) {
                perror("mysh: open (input)");
                return 1;
            }
            dup2(fd, STDIN_FILENO);
            close(fd);


       
            
        }
        else if (strcmp(args[i], ">") == 0) {
            // Output redirection
            if (args[i + 1] == NULL) {           //no argument after >
                perror("mysh: redirection error: no output file\n");
                return 1;
            }
            int fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0640);
            if (fd == -1) {
                perror("mysh: open (output)");
                return 1;
            }
            dup2(fd, STDOUT_FILENO);
            close(fd); 
        }
        i++;
    }

    //remove redirection symbols and filename after redirection is complete.
    i = 0;
    int j = i;
    while(args[i] != NULL)
    {
        if((strcmp(args[i], ">") == 0) || (strcmp(args[i], "<") == 0))
        {
            j++; // Skip the filename
            while (args[j + 1] != NULL) {
                args[j] = args[j + 1];
                j++;
            }
            args[j] = NULL;  // New null terminator

            // Additionally remove the redirection symbol itself
            j=i;         // Decrement to go back to the position of the redirection symbol
            while (args[j + 1] != NULL) {
                args[j] = args[j + 1];
                j++;
            }
            args[j] = NULL; // New null terminator 
            i--;
        }
        i++;
    }


    
    return 0; // Success
}

int execute_conditional(char **args) {
    if (strcmp(args[0], "then") == 0) {
        if (stat != 0) {
            return 0; // Skip this command because the last one failed
        }
    } else if (strcmp(args[0], "else") == 0) {
        if (stat == 0) {
            return 0; // Skip this command because the last one succeeded
        }
    }
    return 1; // Indicate the command should be executed
}


//gcc -o myshell myshell.c
