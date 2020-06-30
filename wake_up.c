#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

// make your own handler for SIGALRM here

void catch() {
  for(int i=0;i<3;i++){
    printf("wake up\n");
    sleep(1);
  }
  //printf("wake up\n");
}


int main(void)
{
	int pid, status, signo;
	time_t t;

	// your codes here
  
  signal(SIGALRM, catch);

	
	// do not modify
	if((pid = fork()) < 0)
	{ // error
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if(pid == 0)
	{ // child
		// alarm() doesn't work properly on repl.it
		// It is replaced with kill(,SIGALRM) in parent part
		// alarm(1);
		pause();
	}
	else
	{ // parent
		kill(pid, SIGALRM);	
		sleep(3);
		kill(pid, SIGINT);	

		// wait for child
		if(waitpid(pid, &status, 0) < 0)
		{ 
			perror("waitpid");
			exit(EXIT_FAILURE);
		}
		else
		{
			if(WIFSIGNALED(status))
			{ // if child terminated by signal
				signo = WTERMSIG(status);
				if(signo != SIGINT)
				{ // if signal is not SIGINT
					perror("termsig");
					printf("child process killed by %s\n", strsignal(signo));					
					exit(EXIT_FAILURE);
				}
				else
				{ // child killed by SIGINT 
					printf("Success\n");
				}
			}
			else
			{ // child terminated by other method
				perror("ifsignaled");
				exit(EXIT_FAILURE);
			}
		}
	}
  return 0;
}