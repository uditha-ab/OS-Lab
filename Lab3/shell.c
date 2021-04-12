#include  <stdio.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>


#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

/* Splits the string by space and returns the array of tokens
*
*/
char **tokenize(char *line)
{
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;

  for(i =0; i < strlen(line); i++){

    char readChar = line[i];

    if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
      token[tokenIndex] = '\0';
      if (tokenIndex != 0){
	tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
	strcpy(tokens[tokenNo++], token);
	tokenIndex = 0;
      }
    } else {
      token[tokenIndex++] = readChar;
    }
  }

  free(token);
  tokens[tokenNo] = NULL ;
  return tokens;
}

void sigint_handler(int signo) {
    printf("Caught SIGINT\n");
}


int main(int argc, char* argv[]) {
	char  line[MAX_INPUT_SIZE];
	char  **tokens;
	int i;

	FILE* fp;
	if(argc == 2) {
		fp = fopen(argv[1],"r");
		if(fp < 0) {
			printf("File doesn't exists.");
			return -1;
		}
	}
    signal(2, sigint_handler);
	while(1) {
		/* BEGIN: TAKING INPUT */
		bzero(line, sizeof(line));
		if(argc == 2) { // batch mode
			if(fgets(line, sizeof(line), fp) == NULL) { // file reading finished
				break;
			}
			line[strlen(line) - 1] = '\0';
		} else { // interactive mode
			printf("$ ");
			scanf("%[^\n]", line);
			getchar();
		}
		//printf("Command entered: %s (remove this debug output later)\n", line);// line is the string variable having the command
		/* END: TAKING INPUT */

		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);

       //do whatever you want with the commands, here we just print them
       /*
       & ---> 1 process in background
       && --> 2 processes in foreground executing one after the other .i.e in serise
       &&&--> 2 processes in foreground executing at same time .i.e in parallel
       */

       int n=0,flag=0;
        for(i=0;tokens[i]!=NULL;i++){
            if(strcmp(tokens[i],"&")==0){
                    n =i;
                    flag=1;
                    }
            else if(strcmp(tokens[i],"&&")==0){
                    n =i;
                    flag =2;
                    }
            else if(strcmp(tokens[i],"&&&")==0){
                    n =i;
                    flag =3;
                    }
        }
        if(flag==1){
            printf("found token %s \n", tokens[n+1]);
///KHANSA CODE HERE
        }
        else if(flag==2){
                int j,k;
                char *argn[] = {0};
                for(j=0;j<n;j++){
                        argn[j] = tokens[j];
                        //printf("n= %d j= %d %s \n",n,j,argn[j]);
                        }
                argn[n] = NULL;
                /*for(i=0;argn[i]!=NULL;i++){
                    printf("found test %d %d %s \n",n,i,argn[i]);
                    }
                for(i=0;argn[i]!=NULL;i++){
                    printf("found token %d %d %s \n",n,i,argn[i]);
                    }*/

            int id2=fork();
            if(id2==0){
                execvp(argn[0], argn);
                }
            else{
                wait(NULL);
                char *bonm[] = {0};
                for(k=0;tokens[k]!=NULL;k++){
                        bonm[k] = tokens[n+k+1];
                        }
                int id3= fork();
                if(id3==0){
                    execvp(bonm[0],bonm);
                    }else{wait(NULL);
                        }
                }
            puts("Finished executing 2");
        }
        else if(flag==3){
                int j,k;
                char *a1[] = {0};
                for(j=0;j<n;j++){
                        a1[j] = tokens[j];
                        }
                a1[n] = NULL;

            int id2=fork();
            if(id2==0){
                execvp(a1[0], a1);
                }
            else{
                //wait(NULL);
                char *a2[] = {0};
                for(k=0;tokens[k]!=NULL;k++){
                        a2[k] = tokens[n+k+1];
                        }
                int id3= fork();
                if(id3==0){
                    execvp(a2[0],a2);
                    }else{wait(NULL);
                        }
                }
            puts("Finished executing 3");
        }else{
            if(strcmp(tokens[0],"cd")==0){
                    chdir(tokens[1]);
                    }else if(strcmp(tokens[0], "exit") == 0) {
               exit(0);
                    }else{
            int id=fork();
            if(id==0){
                execvp(tokens[0], tokens);
                }
            else{
                wait(NULL);
                puts("Finished executing else");
                    }
                }
            }
/*		for(i=0;tokens[i]!=NULL;i++){
			printf("found token %s (remove this debug output later)\n", tokens[i]);
		}
*/

		// Freeing the allocated memory
		for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		free(tokens);

	}
	return 0;
}
