// C program to implement
// the above approach
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include<sys/wait.h>
#include <unistd.h>
#include <dirent.h>

// Driver code
int cat(int argc, char *args[])
{
	FILE* ptr;
	char ch;

	if(argc==3){
		ptr = fopen(*(args + 1),"r");
		char *line = NULL;
		size_t len = 0;

		if (NULL == ptr) {
			printf("file can't be opened \n");
			exit(1);
		}

		while (getline(&line, &len, ptr) != -1) {
            printf("%s", line);
        }

		// do {
		// 	ch = fgetc(ptr);
		// 	printf("%c", ch);

		// 	// Checking if character is not EOF.
		// 	// If it is EOF stop eading.
		// } while (ch != EOF);

		fclose(ptr);
		printf("\n");

	}
	else if(argc==4){
		// printf("arg1: %s\n",*(args +1) );
		// printf("arg2: %s\n",*(args +2) );

		if(strcmp(*(args + 1),"-n")==0){

			ptr = fopen(*(args + 2),"r");
			char *line = NULL;
			size_t len = 0;

			if (NULL == ptr) {
				printf("file can't be opened \n");
				exit(1);
			}

			// reads text until newline is encountered
			int i = 1;
            while (getline(&line, &len, ptr) != -1) {
                printf("%d %s", i, line);
                i++;
            }

			fclose(ptr);
			printf("\n");

		}
		else if(strcmp(*(args + 1),"-e")==0){

			ptr = fopen(*(args + 2),"r");
			char *line = NULL;
			size_t len = 0;

			if (NULL == ptr) {
				printf("file can't be opened \n");
				exit(1);
			}

			// reads text until newline is encountered
			while (getline(&line, &len, ptr) != -1) {
                printf("%s$", line);
            }

            fclose(ptr);
			printf("\n");

		}
	}

	
	return 0;
}