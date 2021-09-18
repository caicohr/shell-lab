#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>


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