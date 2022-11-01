//mkdir command
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int makdir(int argc, char **args){

    if(argc==3){
        chdir("/");
        chdir(*(args+2));
        char *Directory = *(args+1);
        mkdir(Directory,0777);
    }
    else if(argc==4){
        if(strcmp(*(args+1),"-v")==0){
            chdir("/");
            chdir(*(args+3));
            int check;
            char *Directory = *(args+2);
            check = mkdir(Directory,0777);
            if (check==0) {
                printf("Directory created\n");
            }
            else {
                printf("Unable to create Directory\n");
            }
        }
        else if(strcmp(*(args+1),"-p")==0){
            chdir("/");
            chdir(*(args+3));
            char *DirPath = *(args+2);
            char *curDir = strtok(DirPath,"/");
            while(curDir!=NULL){
                mkdir(curDir,0777);
                chdir(curDir);
                curDir = strtok(NULL,"/");
            }
        }
        else{
            printf("Invalid Command, argument required\n");
        }
    }
    exit(0);
}
