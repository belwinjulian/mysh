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
void external_command(char **args);
void redirection_and_piping(char **args);
int redirection(char **args);

int main(int argc, char *argv[]) {
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
    }

    //figure out how to not close terminal to avoid bugs

    return 0;
}
//foo bar < baz
//foo bar<baz
//should both be both same
//figure out how to account for redirection and piping having no spaces
void tokenize(char *line, int mode) {

    char *split_line[MAX_ARGS]; //line split up into a bunch of tokens
    int num = 0; 


//need to make code to check and add spaces before redirections and pipes



    char *token = strtok(line, " \n"); //first token

    while (token != NULL && num < MAX_ARGS - 1) {

        split_line[num++] = token;

        //token = strtok(line, " \n"); does not work
        

        token = strtok(NULL, " \n"); //this line should be correct

        //printf("token test: %s", token);

    }
    split_line[num] = NULL; // Null-terminate the array (important for exec() functions)

   
    
     if (split_line[0] != NULL) {
       
    command(split_line, mode);
}



  //  if (split_line[0] != NULL) {
    //   command(split_line, mode);
    //}

}

//cd    pwd     exit   which
int command(char **args, int mode) {
  /* if (args[0] == NULL) {
         return 0; 
    } */
    //i changed this to be in the process line function 
    
    int i = 0;
    int redirectOrPipe = 0;
    while (args[i] != NULL) {
        if (strcmp(args[i], "<") == 0 || strcmp(args[i], ">") == 0 || strcmp(args[i], "|") == 0) {
            redirectOrPipe = 1;
            break; // Found redirection or pipe, no need to check further
        }
        i++;
    }


    if (redirectOrPipe) {
        // Redirection/piping should take precedence
        external_command(args);
        return 0; 
    }

    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            perror("cd: no filename");
        }
    
    int s = chdir(args[1]);

        if (s != 0) {
            perror("cd: such file or directory");
        }

        return 0;

    } else if (strcmp(args[0], "pwd") == 0) {
        char *dir = getcwd(NULL, 0); //gets the directory which we are in

        if (dir != NULL) {

            printf("%s\n", dir);

        } else {
            perror("pwd: could not get directory path");
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

        } else {

            printf("which: missing argument\n"); 

        }

        return 0;

    } else {

        external_command(args);

    }

    return 0; 
}


void external_command(char **args) {

    pid_t pid; //id variable for forking
    

    int status = 0; //status when child ends or exits

    pid = fork(); //child pid should be 0 and parent should be whatever is not 0
    
    if (pid == 0) {

         //redirection_and_piping(args); this is where the redirection should go
        if (redirection(args) != 0) {
            exit(EXIT_FAILURE); // Exit if redirection failed
        }

        if (execvp(args[0], args) == -1) {
            perror("execvp: erorr during execvp functino");
             exit(EXIT_FAILURE); 
        }
       
    } else if (pid < 0) { // less than zero when child process was failed to get created
        perror("forking error");
    } else {
        // Parent process waits for the child to complete
        do {

            waitpid(pid, &status, WUNTRACED);

        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
}

void redirection_and_piping(char **args) {
   
}



int redirection(char **args) {
    int i = 0;
    while (args[i] != NULL) {
        if (strcmp(args[i], "<") == 0) {
            // Input redirection
            if (args[i + 1] == NULL) {
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
            if (args[i + 1] == NULL) {
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


//gcc -o myshell myshell.c
