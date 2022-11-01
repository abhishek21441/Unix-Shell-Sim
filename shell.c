#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include<sys/wait.h>
#include <unistd.h>
#include <dirent.h>
#include "testls.c"
#include "testdate.c"
#include "testcat.c"
#include "testrm.c"
#include "testmkdir.c"

int main(int argc, char **args) {
    //start location of program execution
    char ExecPath[200];
    getcwd(ExecPath,sizeof(ExecPath));
    char UserPath[200]; // users relative location
    while(1){

    	getcwd(UserPath,sizeof(UserPath)); //start of where the shell is
        printf("Shell: %s$ ",UserPath);

        //acccept user input into Comm
        char Comm[100];
        fgets(Comm,100,stdin);

        //handle empty instruction case
        if(Comm[0]=='\0'|| Comm[0]=='\n'){
            continue;
        }

        //obtain single line Comm
        char* CommFin = strtok(Comm,"\n");

        //divide Comm into words to be read as arguments
        char *CommArray[100];//Stores the divided strings of entered Comm
        int CommNo = 0;//Stores the number of strings in CommArray
        char *CommTok = strtok(CommFin," "); //divides based on " " delimiter
        while(CommTok!=NULL){
            CommArray[CommNo] = CommTok;
            CommTok = strtok(NULL," ");
            ++CommNo;
        }


		if(strcmp(CommArray[CommNo-1],"&t")==0){

			/*
	        We have now obtained an array of strings and its length.
	        Can be treated as double pointer char etc.

	        We can now begin to test the different user input cases.
	        */


	        //_____________________________________________________________________

	        //1. Echo 
	        if(strcmp(CommArray[0], "echo") == 0){
	            if(CommNo==3) {  //length
	                printf("%s\n", CommArray[1]);
	            }
	            else if(CommNo>3){
	                //-n without the end newline character
	                if(strcmp(CommArray[1],"-n")==0){
	                    for(int i=2;i<CommNo-1;i++){
	                        printf("%s ",CommArray[i]);
	                    }
	                }

	                //potential * with echo calling ls.c???

	                //normal with an end newline character
	                else{
	                    for(int i=1;i<CommNo-1;i++){
	                        printf("%s ",CommArray[i]);
	                    }
	                    printf("\n");
	                }
	            }
	        }

	        //_____________________________________________________________________

	        //2. Pwd
	        else if(strcmp(CommArray[0], "pwd") == 0){

	            //No other Comm simply get cwd
	            if(CommNo==2) {
	                char CurrPath[500];
	                getcwd(CurrPath, sizeof(CurrPath));
	                printf("%s\n", CurrPath);
	            }
	            else{
	                //Symlinks????
	                if(strcmp(CommArray[1], "-L")==0){
	                    printf("%s\n", getenv("PWD"));
	                }

	                // -P same as normal case
	                else if(strcmp(CommArray[1], "-P")==0){ 
	                    char CurrPath[200];
	                    getcwd(CurrPath, sizeof(CurrPath));
	                    printf("%s\n", CurrPath);
	                }
	                else{
	                    printf("Invalid Comm, argument required\n");
	                }
	            }
	        }

	        //_____________________________________________________________________

	        //3. Cd

	        else if(strcmp(CommArray[0], "cd") == 0){

	            //Actual directory
	            if(CommNo==3 && (strcmp(CommArray[1],"..")!=0 || strcmp(CommArray[1],"/")!=0 || strcmp(CommArray[1],"-P")!=0 || strcmp(CommArray[1],"~")!=0)){
	                char CurrPath[200];
	                getcwd(CurrPath, sizeof(CurrPath));
	                strcat(CurrPath,"/");
	                strcat(CurrPath,CommArray[1]);
	                int PathNo = chdir(CurrPath);
	                if(PathNo!=0){
	                    printf("No such directory found\n");
	                }
	                // if(strcmp(CommArray[1],"~")==0){
	                //     chdir(getenv("HOME"));
	                // }
	            }

	            // .. runs directly through chdir
	            else if(CommNo==3 && strcmp(CommArray[1],"..")==0){
	                char CurrPath[200];
	                getcwd(CurrPath, sizeof(CurrPath));
	                chdir("..");
	            }
	            // / runs directly through chdir
	            else if(CommNo==3 && strcmp(CommArray[1],"/")==0){
	                chdir("/");
	            }
	            // ~ takes to home 
	            else if(CommNo==3 && strcmp(CommArray[1],"~")==0){
	                chdir(getenv("HOME"));
	            }
	            else if(CommNo==4 && strcmp(CommArray[1],"-P")==0){
	                char CurrPath[200];
	                getcwd(CurrPath, sizeof(CurrPath));
	                strcat(CurrPath,"/");
	                strcat(CurrPath,CommArray[2]);
	                int PathNo = chdir(CurrPath);
	                if(PathNo!=0){
	                    printf("No such directory found\n");
	                }
	            }
	            else{
	                printf("Invalid Comm, Arg required\n");
	            }

	        }

	        //_____________________________________________________________________
	        //_____________________________________________________________________

	        //External Comms : Each calls an external file using exec

	        //1. Ls

	        //ls Comm call -A flag, -m flag taken care of in testls2.c

	        else if(strcmp(CommArray[0],"ls")==0){
	            char CurrPath[200];
	            getcwd(CurrPath,sizeof(CurrPath));
	            chdir("/");
	            chdir(ExecPath);
	            if(CommNo==2){ //Simple ls : prints conents of the current directory
	                int pid_id = fork();
	                if(pid_id==0){
	                    char *args[] = {"./testls",NULL};
	                    //execv(args[0],args);
	                    char *argv[] = {"./a.out",UserPath,NULL}; //passes the current location of user into ls
	                    ls(2,argv);
	                    // execv("./ls",args);
	                    // exit(5);
	                }
	                wait(NULL);
	                // char *argv[] = {"./testls2",ExecPath};
	                
	            }
	            else if(CommNo==3){
	                int pid_id = fork();
	                if(pid_id==0){ // the extra argument passed is the flag, taken care of in the ls.c program
	                    char *args[] = {"./testls",CommArray[1],CurrPath,NULL};
	                    //execv(args[0],args);
	                    //passes the current location of user and the flag into ls
	                    char *argv[] = {"./a.out",UserPath,CommArray[1]};
	                    ls(3,argv);
	                }
	                wait(NULL);

	                // char *argv[] = {"./testls2",ExecPath,CommArray[1]};
	            }
	            chdir("/");
	            chdir(CurrPath);
	        }

	        //_____________________________________________________________________

	        //2. Date

	        else if(strcmp(CommArray[0],"date")==0){
	            char CurrPath[200];
	            getcwd(CurrPath,sizeof(CurrPath));
	            chdir("/");
	            chdir(ExecPath);
	            if(CommNo==2){
	                int pid_id = fork();
	                if(pid_id==0){
	                    char *args[] = {"./testdate",NULL};
	                    //execvp(args[0],args);
	                    date(1,args);
	                }
	            }
	            else if(CommNo==3){
	                int pid_id = fork();
	                if(pid_id==0){
	                    char *args[] = {"./testdate",CommArray[1],NULL};
	                    //execvp(args[0],args);
	                    date(2,args);
	                }
	            }
	            wait(NULL);
	            chdir("/");
	            chdir(CurrPath);
	        }

	        //_____________________________________________________________________

	        //3. Cat

	        else if(strcmp(CommArray[0],"cat")==0){
	            char CurrPath[200];
	            getcwd(CurrPath,sizeof(CurrPath));
	            chdir("/");
	            chdir(ExecPath);

	            //printf("\n%s\n",UserPath );


	            if(CommNo==3){
	                //Strings to adjust the complete path of the file assuming it will exist in the directory where the user is in the shell
	                char PreFilePath[200];
	                char FilePath[200] = "";

	                strcat(PreFilePath,UserPath);
	                strcat(PreFilePath,"/");
	                strcat(FilePath,CommArray[1]);
	                strcat(PreFilePath,FilePath);
	                
	                int pid_id = fork();
	                if(pid_id==0){
	                    char *args[] = {"./cat",PreFilePath,NULL};
	                    //execvp(args[0],args);
	                    cat(3,args);

	                    //readjusting the strings so that they can work multiple times
	                    strcpy(PreFilePath, "");
	                    strcpy(FilePath, "");
	                }
	                wait(NULL);
	            }
	            else if(CommNo>3){
	                char PreFilePath[200];
	                char FilePath[200] = "";

	                strcat(PreFilePath,UserPath);
	                strcat(PreFilePath,"/");
	                strcat(FilePath,CommArray[2]);
	                strcat(PreFilePath,FilePath);
	                //printf("%s\n",PreFilePath);

	                int pid_id = fork();
	                if(pid_id==0){
	                    //passing the flag as well the full file address
	                    char *args[] = {"./cat",CommArray[1],PreFilePath,NULL};
	                    //execvp(args[0],args);
	                    cat(4,args);
	                    strcpy(PreFilePath, "");
	                    strcpy(FilePath, "");
	                }
	                wait(NULL);
	            }

	            chdir("/");
	            chdir(CurrPath);
	        }

	        //_____________________________________________________________________

	        //4. Rm

	        else if(strcmp(CommArray[0],"rm")==0){
	            char CurrPath[200];
	            getcwd(CurrPath,sizeof(CurrPath));
	            chdir("/");
	            chdir(ExecPath);
	            if(CommNo==3){
	                int pid_id = fork();
	                if(pid_id==0){
	                    char *args[] = {"./rm",CommArray[1],CurrPath,NULL};
	                    //execvp(args[0],args);
	                    rm(3,args);
	                }
	                wait(NULL);
	            }
	            else if(CommNo>3){
	                int pid_id = fork();
	                if(pid_id==0){
	                    char *args[] = {"./rm",CommArray[1],CommArray[2],CurrPath,NULL};
	                    //execvp(args[0],args);
	                    rm(4,args);
	                }
	                wait(NULL);
	            }
	            chdir("/");
	            chdir(CurrPath);
	        }

	        //_____________________________________________________________________

	        //5. Mkdir

	        else if(strcmp(CommArray[0],"mkdir")==0){
	            char CurrPath[200];
	            getcwd(CurrPath,sizeof(CurrPath));
	            chdir("/");
	            chdir(ExecPath);
	            if(CommNo==3){
	                int pid_id = fork();
	                if(pid_id==0){
	                    char *args[] = {"./mkdir",CommArray[1],CurrPath,NULL};
	                    //execvp(args[0],args);
	                    makdir(3,args);

	                }
	                wait(NULL);
	            }
	            else if(CommNo>3){
	                int pid_id = fork();
	                if(pid_id==0){
	                    char *args[] = {"./mkdir",CommArray[1],CommArray[2],CurrPath,NULL};
	                    //execvp(args[0],args);
	                    makdir(4,args);

	                }
	                wait(NULL);
	            }
	            chdir("/");
	            chdir(CurrPath);

	        }

	        //_____________________________________________________________________

	        // Exit Comm


	        if(strcmp(CommFin,"exit")==0){
	            //break;
	            exit(0);
	        }

		}
		
		else{
			

			/*
	        We have now obtained an array of strings and its length.
	        Can be treated as double pointer char etc.

	        We can now begin to test the different user input cases.
	        */


	        //_____________________________________________________________________

	        //1. Echo 
	        if(strcmp(CommArray[0], "echo") == 0){
	            if(CommNo==2) {  //length
	                printf("%s\n", CommArray[1]);
	            }
	            else if(CommNo>2){
	                //-n without the end newline character
	                if(strcmp(CommArray[1],"-n")==0){
	                    for(int i=2;i<CommNo;i++){
	                        printf("%s ",CommArray[i]);
	                    }
	                }

	                //potential * with echo calling ls.c???

	                //normal with an end newline character
	                else{
	                    for(int i=1;i<CommNo;i++){
	                        printf("%s ",CommArray[i]);
	                    }
	                    printf("\n");
	                }
	            }
	        }

	        //_____________________________________________________________________

	        //2. Pwd
	        else if(strcmp(CommArray[0], "pwd") == 0){

	            //No other Comm simply get cwd
	            if(CommNo==1) {
	                char CurrPath[500];
	                getcwd(CurrPath, sizeof(CurrPath));
	                printf("%s\n", CurrPath);
	            }
	            else{
	                //Symlinks????
	                if(strcmp(CommArray[1], "-L")==0){
	                    printf("%s\n", getenv("PWD"));
	                }

	                // -P same as normal case
	                else if(strcmp(CommArray[1], "-P")==0){ 
	                    char CurrPath[200];
	                    getcwd(CurrPath, sizeof(CurrPath));
	                    printf("%s\n", CurrPath);
	                }
	                else{
	                    printf("Invalid Comm, argument required\n");
	                }
	            }
	        }

	        //_____________________________________________________________________

	        //3. Cd

	        else if(strcmp(CommArray[0], "cd") == 0){

	            //Actual directory
	            if(CommNo==2 && (strcmp(CommArray[1],"..")!=0 || strcmp(CommArray[1],"/")!=0 || strcmp(CommArray[1],"-P")!=0 || strcmp(CommArray[1],"~")!=0)){
	                char CurrPath[200];
	                getcwd(CurrPath, sizeof(CurrPath));
	                strcat(CurrPath,"/");
	                strcat(CurrPath,CommArray[1]);
	                int PathNo = chdir(CurrPath);
	                if(PathNo!=0){
	                    printf("No such directory found\n");
	                }
	                // if(strcmp(CommArray[1],"~")==0){
	                //     chdir(getenv("HOME"));
	                // }
	            }

	            // .. runs directly through chdir
	            else if(CommNo==2 && strcmp(CommArray[1],"..")==0){
	                char CurrPath[200];
	                getcwd(CurrPath, sizeof(CurrPath));
	                chdir("..");
	            }
	            // / runs directly through chdir
	            else if(CommNo==2 && strcmp(CommArray[1],"/")==0){
	                chdir("/");
	            }
	            // ~ takes to home 
	            else if(CommNo==2 && strcmp(CommArray[1],"~")==0){
	                chdir(getenv("HOME"));
	            }
	            else if(CommNo==3 && strcmp(CommArray[1],"-P")==0){
	                char CurrPath[200];
	                getcwd(CurrPath, sizeof(CurrPath));
	                strcat(CurrPath,"/");
	                strcat(CurrPath,CommArray[2]);
	                int PathNo = chdir(CurrPath);
	                if(PathNo!=0){
	                    printf("No such directory found\n");
	                }
	            }
	            else{
	                printf("Invalid Comm, Arg required\n");
	            }

	        }

	        //_____________________________________________________________________
	        //_____________________________________________________________________

	        //External Comms : Each calls an external file using exec

	        //1. Ls

	        //ls Comm call -A flag, -m flag taken care of in testls2.c

	        else if(strcmp(CommArray[0],"ls")==0){
	            char CurrPath[200];
	            getcwd(CurrPath,sizeof(CurrPath));
	            chdir("/");
	            chdir(ExecPath);
	            if(CommNo==1){ //Simple ls : prints conents of the current directory


	                // char *CommFinArr[100];//Stores the divided strings of entered Comm
	                // int CommFinN = 0;//Stores the number of strings in Comm arr
	                // char *token2Comm = strtok(ExecPath," "); //divides based on " " delimiter
	                // while(token2Comm!=NULL){
	                //     CommFinArr[CommFinN] = token2Comm;
	                //     token2Comm = strtok(NULL," ");
	                //     ++CommFinN;
	                // }


	                int pid_id = fork();
	                if(pid_id==0){
	                    char *args[] = {"./testls",NULL};
	                    //execv(args[0],args);
	                    char *argv[] = {"./a.out",UserPath,NULL}; //passes the current location of user into ls
	                    ls(2,argv);
	                    // execv("./ls",args);
	                    // exit(5);
	                }
	                wait(NULL);
	                // char *argv[] = {"./testls2",ExecPath};
	                
	            }
	            else if(CommNo==2){
	                int pid_id = fork();
	                if(pid_id==0){ // the extra argument passed is the flag, taken care of in the ls.c program
	                    char *args[] = {"./testls",CommArray[1],CurrPath,NULL};
	                    //execv(args[0],args);
	                    //passes the current location of user and the flag into ls
	                    char *argv[] = {"./a.out",UserPath,CommArray[1]};
	                    ls(3,argv);
	                }
	                wait(NULL);

	                // char *argv[] = {"./testls2",ExecPath,CommArray[1]};
	            }
	            chdir("/");
	            chdir(CurrPath);
	        }

	        //_____________________________________________________________________

	        //2. Date

	        else if(strcmp(CommArray[0],"date")==0){
	            char CurrPath[200];
	            getcwd(CurrPath,sizeof(CurrPath));
	            chdir("/");
	            chdir(ExecPath);
	            if(CommNo==1){
	                int pid_id = fork();
	                if(pid_id==0){
	                    char *args[] = {"./testdate",NULL};
	                    //execvp(args[0],args);
	                    date(1,args);
	                }
	            }
	            else if(CommNo==2){
	                int pid_id = fork();
	                if(pid_id==0){
	                    char *args[] = {"./testdate",CommArray[1],NULL};
	                    //execvp(args[0],args);
	                    date(2,args);
	                }
	            }
	            wait(NULL);
	            chdir("/");
	            chdir(CurrPath);
	        }

	        //_____________________________________________________________________

	        //3. Cat

	        else if(strcmp(CommArray[0],"cat")==0){
	            char CurrPath[200];
	            getcwd(CurrPath,sizeof(CurrPath));
	            chdir("/");
	            chdir(ExecPath);

	            //printf("\n%s\n",UserPath );


	            if(CommNo==2){
	                //Strings to adjust the complete path of the file assuming it will exist in the directory where the user is in the shell
	                char PreFilePath[200];
	                char FilePath[200] = "";

	                strcat(PreFilePath,UserPath);
	                strcat(PreFilePath,"/");
	                strcat(FilePath,CommArray[1]);
	                strcat(PreFilePath,FilePath);
	                
	                int pid_id = fork();
	                if(pid_id==0){
	                    char *args[] = {"./cat",PreFilePath,NULL};
	                    //execvp(args[0],args);
	                    cat(3,args);

	                    //readjusting the strings so that they can work multiple times
	                    strcpy(PreFilePath, "");
	                    strcpy(FilePath, "");
	                }
	                wait(NULL);
	            }
	            else if(CommNo>2){
	                char PreFilePath[200];
	                char FilePath[200] = "";

	                strcat(PreFilePath,UserPath);
	                strcat(PreFilePath,"/");
	                strcat(FilePath,CommArray[2]);
	                strcat(PreFilePath,FilePath);
	                //printf("%s\n",PreFilePath);

	                int pid_id = fork();
	                if(pid_id==0){
	                    //passing the flag as well the full file address
	                    char *args[] = {"./cat",CommArray[1],PreFilePath,NULL};
	                    //execvp(args[0],args);
	                    cat(4,args);
	                    strcpy(PreFilePath, "");
	                    strcpy(FilePath, "");
	                }
	                wait(NULL);
	            }

	            chdir("/");
	            chdir(CurrPath);
	        }

	        //_____________________________________________________________________

	        //4. Rm

	        else if(strcmp(CommArray[0],"rm")==0){
	            char CurrPath[200];
	            getcwd(CurrPath,sizeof(CurrPath));
	            chdir("/");
	            chdir(ExecPath);
	            if(CommNo==2){
	                int pid_id = fork();
	                if(pid_id==0){
	                    char *args[] = {"./rm",CommArray[1],CurrPath,NULL};
	                    //execvp(args[0],args);
	                    rm(3,args);
	                }
	                wait(NULL);
	            }
	            else if(CommNo>2){
	                int pid_id = fork();
	                if(pid_id==0){
	                    char *args[] = {"./rm",CommArray[1],CommArray[2],CurrPath,NULL};
	                    //execvp(args[0],args);
	                    rm(4,args);
	                }
	                wait(NULL);
	            }
	            chdir("/");
	            chdir(CurrPath);
	        }

	        //_____________________________________________________________________

	        //5. Mkdir

	        else if(strcmp(CommArray[0],"mkdir")==0){
	            char CurrPath[200];
	            getcwd(CurrPath,sizeof(CurrPath));
	            chdir("/");
	            chdir(ExecPath);
	            if(CommNo==2){
	                int pid_id = fork();
	                if(pid_id==0){
	                    char *args[] = {"./mkdir",CommArray[1],CurrPath,NULL};
	                    //execvp(args[0],args);
	                    makdir(3,args);

	                }
	                wait(NULL);
	            }
	            else if(CommNo>2){
	                int pid_id = fork();
	                if(pid_id==0){
	                    char *args[] = {"./mkdir",CommArray[1],CommArray[2],CurrPath,NULL};
	                    //execvp(args[0],args);
	                    makdir(4,args);

	                }
	                wait(NULL);
	            }
	            chdir("/");
	            chdir(CurrPath);

	        }

	        //_____________________________________________________________________

	        // Exit Comm


	        if(strcmp(CommFin,"exit")==0){
	            //break;
	            exit(0);
	        }

		}
	}
	
}