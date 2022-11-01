#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>


int ls(int argc, char **args) {
    if(argc == 2) {
        chdir("/");
        char CurrPath[1024];
        chdir(*(args+1));
        struct dirent *Read;
        DIR *Directory = opendir(".");

        if (Directory == NULL) {
            printf("Directory doesn't exists\n");
        }

        Read = readdir(Directory);
        //printf("2\n\n");
        while (Read != NULL) {
            printf("%s\n", Read->d_name);
            Read = readdir(Directory);
        }
        closedir(Directory);
    }
    else if(argc == 3){
        chdir("/");
        char CurrPath[1024];
        chdir(*(args+1));
        if(strcmp(*(args+2),"-A")==0){
            //chdir("/");
            char CurrPath[1024];
            chdir(getcwd(CurrPath, sizeof(CurrPath)));
            chdir(*(args+2));
            struct dirent *Read;
            DIR *Directory = opendir(".");

            if (Directory == NULL) {
                printf("Directory doesn't exists\n");
            }

            Read = readdir(Directory);
            int i = 0;
            while (Read != NULL) {
                if(i>=2) {
                    printf("%s\n", Read->d_name);
                }
                Read = readdir(Directory);
                i++;
            }
            closedir(Directory);
        }
        else if(strcmp(*(args+2),"-m")==0){
            //chdir("/");
            char CurrPath[1024];
            chdir(getcwd(CurrPath, sizeof(CurrPath)));
            chdir(*(args+2));
            struct dirent *Read;
            DIR *Directory = opendir(".");

            if (Directory == NULL) {
                printf("Directory doesn't exists\n");
            }

            Read = readdir(Directory);
            while (Read != NULL) {
                printf("%s, ", Read->d_name);
                Read = readdir(Directory);
            }
            closedir(Directory);
            printf("\n");
        }
        else{
            printf("Invalid Command, argument required\n");
        }
    }
    exit(0);
}
