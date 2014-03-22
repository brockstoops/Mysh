//Brock Stoops
//b3068055
//Operating Systems HomeWork 1





#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#define MAX_BUFFER 1024                        // max line buffer
#define MAX_ARGS 64                            // max # args
#define SEPARATORS " \t\n"                     // token sparators
   
int main (int argc, char ** argv)
{
    char buf[MAX_BUFFER];                      // line buffer
    char * args[MAX_ARGS];                     // pointers to arg strings
    char ** arg;                               // working pointer thru args
    char * prompt = "# " ;                    // shell prompt
    int murderall[100000] = {0};
    

/* keep reading input until "quit" command or eof of redirected input */
     
    while (!feof(stdin)) { 
    
/* get command line from input */
  
        fputs (prompt, stdout);                // write prompt
        if (fgets (buf, MAX_BUFFER, stdin )) { // read a line
        
/* tokenize the input into args array */

            arg = args;
            *arg++ = strtok(buf,SEPARATORS);   // tokenize input
            while ((*arg++ = strtok(NULL,SEPARATORS)));
                                               // last entry will be NULL 
 
            if (args[0]) {                     // if there's anything there
            
/* check for internal/external command */

                if (!strcmp(args[0],"run")) { // run command
		   pid_t pid = fork();
		   int status;
		   if(pid == 0){
                    execvp(args[1], &args[1]);  
		   }
		   else{
			if(waitpid(pid, &status, 0) == 0)
                         continue;
			}
                }
            
		 if (!strcmp(args[0],"background")) { // background command
		    pid_t pid = fork();
		    if(pid == 0){
		 	pid = getpid();
			arg = args;
			execvp(args[1], &args[1]);
		    }
		    printf("%d\n", (int) pid);
		    murderall[(int)pid] = 1;
                    continue;
                }

		 if (!strcmp(args[0],"murder")) { // run command
		    int k = kill(atoi(args[1]), SIGKILL);
			if(k == -1)
			printf("Failed to murder\n");
			else
			printf("Successfully murdered pid %d\n", atoi(args[1]));
		    murderall[atoi(args[1])] = 0;
                    continue;
                }		

		 if (!strcmp(args[0],"murderall")) { // murderall command
		    int i = 0;
		    int cnt = 0;
		    for(i = 0; i<10000; i++){
			if(murderall[i] == 1){
			kill(i, SIGKILL);
			cnt++;
			printf("%d ", i);
			}
		    }
		    printf("\nMurdered %d processes\n", cnt);
                    continue;
                }

		if (!strcmp(args[0],"repeat")) { // repeat command
		    int n = atoi(args[1]);
		    int i;
	    	    printf("PIDs: ");
		    for(i=0; i<n; i++){
		       pid_t pid = fork();
		       if(pid == 0){
		 	pid = getpid();
			arg = args;
			execvp(args[2], &args[2]);
		       }
		    printf("%d ", (int) pid);
		    murderall[(int)pid] = 1;
		    }
		    printf("\n");
                    continue;
                }

                if (!strcmp(args[0],"quit"))   // "quit" command
                    break;                     // break out of 'while' loop

/* else pass command onto OS (or in this instance, print them out) */

                //arg = args;
               // while (*arg) fprintf(stdout,"%s ",*arg++);
             //   fputs ("\n", stdout);
            }
        }
    }
    return 0; 
}
