#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>//mkdir
#include <sys/types.h>//mkdir

#include<stdio.h>
#include<sys/wait.h>
#include<fcntl.h>

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

#define BUFFER_SIZE 3000
#define TOKENS_SIZE 50


int main(int argc, char **argv) {
    
    while (1) {
    
        char buffer[BUFFER_SIZE];
        
        char *tokens[TOKENS_SIZE];
        int num_tokens = 0;
        
        //piece of the String
        char *token_ptr;
        
        long size;
        char *buf;
        char *ptr;
        //for mkdir
        int status;
        
        
        size = pathconf(".", _PC_PATH_MAX);
        
        
        //buf = (char *)malloc((size_t)size);
        ptr = getcwd(buf, (size_t)size);
        
        //ls file here
        int listDir(char *dir) {        
            DIR             *dip;
            struct dirent   *dit;       
            int             i = 0;       
            /* check to see if user entered a directory name */        
            if (num_tokens < 2) {
                if ((dip = opendir(".")) == NULL) {                
                    //perror("opendir");
                    //prints out error                
                    //tests different errors.
                    if (errno==ENOTDIR) {
                        printf("Not Directory\n");
                    }
                    if (errno==EACCES) {
                        printf("Permission Denied\n");
                    }
                    if (errno==ENOENT) {
                        printf("File not Found\n");
                    }
                    return 0;
                }
                /*  struct dirent *readdir(DIR *dir);         
                *         
                * Read in the files from argv[1] and print */        
                while ((dit = readdir(dip)) != NULL) {                
                    i++;                
                    if (dit->d_type==DT_DIR)
                    printf("\nDirectory: %s", dit->d_name);
                    else
                    printf("\n%s", dit->d_name);      
                }        
                printf("\n\nreaddir() found a total of %i files\n", i);
                /* int closedir(DIR *dir);         
                *         
                * Close the stream to argv[1]. And check for errors. */
                if (closedir(dip) == -1) {
                    perror("closedir");
                    return 0;        
                }
                return 1;
                //ending here!!!
                
            }        
            /* DIR *opendir(const char *name);*
            * Open a directory stream to argv[1] and make sure         
            * it's a readable and valid (directory) */        
            if ((dip = opendir(tokens[1])) == NULL) {                
                //perror("opendir");//prints out error                
                //tests different errors.
                if (errno==ENOTDIR) {
                    printf("Not Directory\n");
                }
                if (errno==EACCES) {
                    printf("Permission Denied\n");
                }
                if (errno==ENOENT) {
                    printf("File not Found\n");
                }
                return 0;
            }
            /*  struct dirent *readdir(DIR *dir);         
            *         
            * Read in the files from argv[1] and print */        
            while ((dit = readdir(dip)) != NULL) {                
                i++;                
                if (dit->d_type==DT_DIR)
                printf("\nDirectory: %s", dit->d_name);
                else
                printf("\n%s", dit->d_name);        
            }        
            printf("\n\nreaddir() found a total of %i files\n", i);
            /* int closedir(DIR *dir);         
            *         
            * Close the stream to argv[1]. And check for errors. */
            if (closedir(dip) == -1) {
                perror("closedir");
                return 0;        
            }
            return 1;
        }
        
        //start of execute c here
        void execute_normal(char **argv) {
            pid_t pid;
            int status;
            pid = fork();
            if (pid < 0) {
                printf("ERROR fork failed\n");
                exit(1);
            }
            else if (pid == 0)//child process
            {
                if (execvp(*argv, argv) < 0) {
                    perror("execvp");
                    exit(1);
                }
            }
            else {
                while (wait(&status) != pid) {} //parent, waits for completion
                }
        }
        
        void execute_redirect(char **argv) {
            pid_t pid;
            int status;
            int defout;
            int fd;
            pid = fork();
            if (pid < 0) {
                printf("ERROR fork failed\n");
                exit(1);
            }
            else if (pid == 0) //child process
            {
                defout = dup(1);
                fd=open(ptr,O_RDWR|O_CREAT,0644);
                dup2(fd,1);
                if (execvp(*argv, argv) < 0)
                {
                    perror("execvp");
                    exit(1);
                }
                close(fd);
            }
            else {
                while (wait(&status) != pid) {} //parent, waits for completion
                }
        }
        //ending execute here
        
        printf("%s$$$ ", ptr);
        //free(buf);
        
        //Read in a line using fgets, don't forget to trim the new line at the end.
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        
        //While loop collecting tokens
        token_ptr = strtok(buffer, " ");
        while(token_ptr != NULL) {
            tokens[num_tokens] = token_ptr;
            num_tokens++;
            token_ptr = strtok(NULL, " ");
        }
        
        //Nulling the last array
        tokens[num_tokens] = 0;
        
        /*For Loop Printing out tokens and debugging
        for (int i = 0; i < num_tokens; i++) {
            printf("Token %i: %s\n", i, tokens[i]);
        }
        */
        
        //Exiting program
        int resultExit = strcmp("exit", tokens[0]);
        
        //echo
        int resultEcho = strcmp("echo", tokens[0]);
        
        //mkdir
        int resultMkdir = strcmp("mkdir", tokens[0]);
        
        //rmdir
        int resultRmdir = strcmp("rmdir", tokens[0]);
        
        //cd
        int resultCd = strcmp("cd", tokens[0]);
        
        //ls
        int resultLs = strcmp("ls", tokens[0]);
        
        //exit
        if (resultExit == 0) {
            return 0;
        }
        else if (resultEcho == 0 ) {//echo
            for(int i = 1; i < num_tokens; i++) {
                printf("%s ", tokens[i]);
            }
            printf("\n");
        }
        else if (resultMkdir == 0) {//mkdir
            status = mkdir(tokens[1], S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            if (status == -1){
                perror("Error");
            }
        }
        else if (resultRmdir == 0) {//rmdir
            status = rmdir(tokens[1]);
            if (status == -1){
                perror("Error");
            }
        }
        else if (resultCd == 0) {//cd
            char *add = strcat(ptr, "/");
            char *directory = strcat(ptr, tokens[1]);
            status = chdir(directory);
            if (status == -1) {
                perror("Error");
            }
        }
        else if (resultLs == 0) {//ls
            listDir(*tokens);
        }
        else {//Command not recognized
            int place = num_tokens - 2;
            int resultFind = strcmp(">", tokens[place]);
            int last = num_tokens -1;
            char *addnow = strcat(ptr, "/");
            char *out = strcat(addnow, tokens[last]);
            if (resultFind == 0) {//if tokens has ">"
                execute_redirect(tokens);
            }
            else {
                execute_normal(tokens);
            }
            
        }
        
    }
    
}