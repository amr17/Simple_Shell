#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

//functions protyping
void token(char* argc, char** args);
void execute(char** args);
void handler(int s);
int counter=0;
int flag =0;
FILE *logfile;
pid_t pid;

void main()
{
    signal(SIGCHLD, handler);
    char * user = getenv("USER"); // a pointer to the value of the environment variable passed as argument
    char   argc[70]; // input command
    char * argp[10]; // input parameter

    while (1)
    {
        counter=0;
        flag=0;
        printf("\n%s@shell> ", user);
        // Read input string
        if(fgets(argc, sizeof(argc), stdin) == NULL)
            break;
        // check & remove newline character from string
        if(argc[strlen(argc)-1] == '\n')
        {
            argc[strlen(argc)-1] = '\0';
        }
        // Split on space cmd into array of parameters
        token(argc, argp);
        // check Exit
        if(strcmp(argp[0],"exit") == 0)
        exit(1);
        // check if (&) character exist
        if (strcmp(argp[counter-1],"&")==0)
        {
            flag=1;
            argp[counter-1]=NULL;
        }
        // Execute
        execute(argp);
    }
}
//an interrupt handler that handles SIGCHLD, It is called when a child process is terminated.
void handler(int s)
{
    logfile = fopen("/home/saleh/Desktop/Simple_Shell/logfile.txt","a");
    pid_t pid;

    if(pid==0)
    {
        fprintf(logfile,"Parent id %d \n", getppid());
    }
    else
    {
        // wait(NULL);
        fprintf(logfile,"child process terminate id %d\n", getpid());
    }
    fclose(logfile);
}
//split on space
void token(char* argc, char** args)
{
    for(int i = 0; i < sizeof(args); i++)
    {
        args[i] = strsep(&argc, " ");
        if(args[i] == NULL)
            break;
        counter++;
    }
}
//execution
void execute(char** args)
{
    pid = fork();
    // fork successed
    if(pid >= 0)
    {
        // child process
        if (pid == 0)
        {
            // Execute command
            execvp(args[0],args);
            //if get error
            printf("\n %s: command could not execute \n",*args);
        }
        // Parent process
        else
        {
            if(!flag)
            {
                wait(NULL); // waiting until child become zero.

            }
        }
    }
    else
    {
        printf("\n ERROR: Fork Failed \n");
        exit(1);
    }
}
