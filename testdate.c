#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

int date(int argc,char **args)
{
    time_t t = time(NULL);
    struct tm tm;
    if(argc==1){
        tm = *localtime(&t);
    }
    else if(strcmp(*(args+1),"-R")==0){
        tm = *localtime(&t);
    }
    else if(strcmp(*(args+1),"-u")==0){
            tm = *gmtime(&t);
    }
    else{
        printf("Invalid Command, argument required\n");
        exit(0);
    }


    char Mon[5];
    char WDay[5];
    if(tm.tm_mon+1==1){
        strcpy(Mon,"Jan");
    }
    if(tm.tm_mon+1==2){
        strcpy(Mon,"Feb");
    }
    if(tm.tm_mon+1==3){
        strcpy(Mon,"Mar");
    }
    if(tm.tm_mon+1==4){
        strcpy(Mon,"Apr");
    }
    if(tm.tm_mon+1==5){
        strcpy(Mon,"May");
    }
    if(tm.tm_mon+1==6){
        strcpy(Mon,"Jun");
    }
    if(tm.tm_mon+1==7){
        strcpy(Mon,"Jul");
    }
    if(tm.tm_mon+1==8){
        strcpy(Mon,"Aug");
    }
    if(tm.tm_mon+1==9){
        strcpy(Mon,"Sep");
    }
    if(tm.tm_mon+1==10){
        strcpy(Mon,"Oct");
    }
    if(tm.tm_mon+1==11){
        strcpy(Mon,"Nov");
    }
    if(tm.tm_mon+1==12){
        strcpy(Mon,"Dec");
    }
    if(tm.tm_wday+1==2){
        strcpy(WDay,"Mon");
    }
    if(tm.tm_wday+1==3){
        strcpy(WDay,"Tue");
    }
    if(tm.tm_wday+1==4){
        strcpy(WDay,"Wed");
    }
    if(tm.tm_wday+1==5){
        strcpy(WDay,"Thu");
    }
    if(tm.tm_wday+1==6){
        strcpy(WDay,"Fri");
    }
    if(tm.tm_wday+1==7){
        strcpy(WDay,"Sat");
    }
    if(tm.tm_wday+1==1){
        strcpy(WDay,"Sun");
    }

    if(argc==1 || (argc==2 && (strcmp(*(args+1),"-R")==0))) {
        if(argc==1){
            printf("%s %s %d %02d:%02d:%02d IST %d\n", WDay, Mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,
               tm.tm_year + 1900);
        }
        else{
            printf("%s %d %s %d %02d:%02d:%02d +0530 \n", WDay, tm.tm_mday, Mon, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
        }
    }
    else{
        printf("%s %s %d %02d:%02d:%02d UTC %d\n", WDay, Mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,
               tm.tm_year + 1900);
    }
    exit(0);
}
