#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

char** parse(char* temp){
        int size = 0;
        char** args = NULL;
        while(temp != NULL){
                size++;
                if(size == 1){
                        args = (char**) malloc(sizeof(char*)*size);
                }
                else {
                        args = (char**) realloc(args, sizeof(char*)*size);
                }
                args[size-1] = strdup(temp);
                temp = strtok(NULL, "_");
        }
        //TEST PRINT
        //for(int j = 0; j < size; j++){
        //      printf("%s ",args[j]);
        //}
        //printf("\n");
        //END TEST
        args[size] = NULL;
        return args;
}

int main(int argc, char **argv){
	if(argc < 3){
		printf("ERROR: not enough arguments provided\n");
		exit(1);
	}
	int **fds, pipeC = 0;
	fds = (int**) malloc(sizeof(int*)*(argc-2));
	for(int i = 0; i < argc-1; i++){
		fds[i] = (int*) malloc(sizeof(int)*2);
	}
	for(int i = 0; i < argc-2; i++){
		pipe(fds[i]);
		pipeC++;
	}
	for(int i = 1; i <= argc-1; i++){
		pid_t retval = fork();
		char **args = NULL;
		if(retval == 0){
			char *temp = strtok(argv[i], "_");
			if(i == 1){
				args = parse(temp);
				
				for(int j = 1; j < argc-2; j++){
					close(fds[j][0]);
					close(fds[j][1]);
				}
				close(fds[0][0]);
				if(dup2(fds[0][1], 1) == -1){
					printf("ERROR: dup2 failed");
				}		
				if(execvp(args[0], args) == -1){
                                	printf("ERROR: execvp didn't accept command\n");
                                	exit(1);
                        	}
				
			}
			else if(i == argc-1){
				args = parse(temp);

                                for(int z = 0; z < argc-3; z++){
                                        close(fds[z][0]);
                                        close(fds[z][1]);
                                }
                                close(fds[argc-3][1]);
                                if(dup2(fds[argc-3][0], 0) == -1){
					printf("ERROR: dup2 failed");
				}             
                                if(execvp(args[0], args) == -1){
                                        printf("ERROR: execvp didn't accept command\n");
                                        exit(1);
                                }
			}
			else{
				args = parse(temp);
				int nextP;
				
				if(i%2 == 0){
					nextP = (i/2)-1; 	
				} else {
					nextP = ((i+1)/2)-1;
				}
				
				for(int z = 0; z < argc-1; z++){
					if(z == nextP){
						close(fds[z][1]);	
					}
					else if(z == nextP+1){
						close(fds[z][0]);
					}
					else{
						close(fds[z][0]);
						close(fds[z][1]);
					}
				}
             			if(dup2(fds[nextP][0], 0) == -1){
					printf("ERROR: dup2 failed");
				}
				if(dup2(fds[nextP+1][1], 1) == -1){
					printf("ERROR: dup2 failed");
				}
                                if(execvp(args[0], args) == -1){
                                        printf("ERROR: execvp didn't accept command\n");
                                        exit(1);
                      		}	
			}
		}
		else if(retval == -1){
			printf("ERROR: fork error\n");
			exit(1);
		}
	}
	for(int q = 0; q < argc-1; q++){
                close(fds[q][0]);
                close(fds[q][1]);
        }
        for(int q = 0; q < argc-1; q++){
                wait(NULL);
        }
}
