#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv){
	bool in = false;
	FILE *f;
	if(strcmp(argc[argv-1], "-append") == 0){
		if(argc > 1){
                	for(int i = 1; i < argc; i++){
                        	char *comm = strtok(argv[i], "=");
                        	char *file = strtok(NULL, "");
                        	if(strcmp(comm, "in") == 0){
                                	in = true;
                                	FILE *f = fopen(file, "r");
                                	if (f == NULL){
                                        	printf("ERROR: file not found\n");
                                	}
                                	dup2(fileno(f),0);
                        	}
                        	if(strcmp(comm, "err") == 0){
                                	FILE *f = fopen(file, "a");
                                	if (f == NULL){
                                        	printf("ERROR: file not found\n");
                                	}
                                	dup2(fileno(f),2);
                        	}
                        	if(strcmp(comm, "out") == 0){
                                	FILE *f = fopen(file, "a");
                                	if (f == NULL){
                                        	printf("ERROR: file not found\n");
                                	}
                                	dup2(fileno(f),1);
                        	}
                	}	
		}
	} else {
		if(argc > 1){
			for(int i = 1; i < argc; i++){
				char *comm = strtok(argv[i], "=");
				char *file = strtok(NULL, "");
				if(strcmp(comm, "in") == 0){
					in = true;
					FILE *f = fopen(file, "r");
					if (f == NULL){
						printf("ERROR: file not found\n");
					}
					dup2(fileno(f),0);
				}
				if(strcmp(comm, "err") == 0){
					FILE *f = fopen(file, "w");
					if (f == NULL){
                                        printf("ERROR: file not found\n");
                                }
				dup2(fileno(f),2);
				}
				if(strcmp(comm, "out") == 0){
                                	FILE *f = fopen(file, "w");
					if (f == NULL){
                                        	printf("ERROR: file not found\n");
                                	}
					dup2(fileno(f),1);
                        	}
			}
		}
	}
	if (in == false){
		char str[1024];
		fgets(str, 1024, stdin);
		fprintf(stdout, "STDOUT: %s", str);
		fprintf(stderr, "STDERR: %s", str);
	} else {
		char str[1024];
		while(fgets(str, 1024, stdin)){
                	fprintf(stdout, "STDOUT: %s\n", strtok(str,"\n"));
                	fprintf(stderr, "STDERR: %s\n", strtok(str,"\n"));
		}
	}
}
