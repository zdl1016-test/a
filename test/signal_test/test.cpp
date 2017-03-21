#include <stdio.h>
#include <signal.h>
#include <unistd.h>

static void sig_usr(int);

void RunWork();

int main(void)
{        
    RunWork();
#if 0    
	for(; ;)                
		pause();
#endif
}

void RunWork()
{
    pid_t pid = fork();
    if( pid == 0 )
    {
        printf( "enter child process ... \n" );
     	
        if(signal(SIGUSR1, sig_usr) == SIG_ERR)                
	    	printf("can't catch SIGUSR1\n");        
    	if(signal(SIGUSR2, sig_usr) == SIG_ERR)                
	    	printf("can't catch SIGUSR2\n");        

        for(;;)
        {
            sleep(1);
        }
        printf( "will leave pid==0\n" );
    }
    if( pid > 0 )
    {
        printf( "continue parent process ... \n" );
        printf( "will leave pid>0\n" );
    }   
    printf( "exit RunWork.\n" );
}

static void sig_usr(int signo){        
	if(signo == SIGUSR1)
		printf("received SIGUSR1\n");
	else if(signo == SIGUSR2)
		printf("received SIGUSR2\n");
	else
		printf("received signal %d\n", signo);
}

