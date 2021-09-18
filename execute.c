#include<stdlib.h>
#include<stdio.h>=
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<fcntl.h>

void execute_normal(char **argv);
void execute_redirect(char **argv);

int main(int argc, char** argv) {
    char *cmd[] = { "/bin/ls", "-l", (char*)0};
    execute_normal(cmd);
    execute_redirect(cmd);
}

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
        fd=open("/home/cslade/cs415/lsout.txt",O_RDWR|O_CREAT,0644);
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