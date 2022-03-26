#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>

int main(){
	while(1){
	char str[100];
	char **args = NULL;
	char *temp;
	int size = 0;
	printf("MY_UNIX> ");
	gets(str);
	if(strcmp(str, "") == 0){
		continue;
	}
	temp = strtok(str, " ");
	while(temp != NULL){
		size++;
		if(size == 1){
			args = (char**) malloc(sizeof(char*)*size);	
		}
		else {
			args = (char**) realloc(args, sizeof(char*)*size);
		}
		if((strcmp(temp, "EOF") == 0) || (strcmp(temp, "exit") == 0)){
				exit(1);
		}
		args[size-1] = temp;
		temp = strtok(NULL, " ");
		//printf("comands: %s\n", args[size-1]);
		}
		args[size] = NULL;
		pid_t retval = fork();
		if (retval == 0){
			int exec = execvp(args[0], args);
			if(exec == -1){
				printf("ERROR: execvp didn't accept command\n");
				exit(1);
			}
		}
		else if (retval == -1) {
			printf("ERROR: fork error\n");
		}
		else {
			int stat;
			wait(&stat);
		}
	}
}
