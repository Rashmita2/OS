

#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#define WHITESPACE " \t\n"      // We want to split our command line up into tokens
// so we need to define what delimits our tokens.
// In this case  white space
// will separate the tokens on our command line

#define MAX_COMMAND_SIZE 255    // The maximum command-line size

#define MAX_NUM_ARGUMENTS 5     // Mav shell only supports five arguments
void historyList(char history_List[15][255] , int count ); // Function declearation

int main()
{
    
    char history_List[15][255] = {{ 0 }}; //2D array store the each command line
    char * cmd_str = (char*) malloc( MAX_COMMAND_SIZE ); // Assign memeory in OS for execuation
    int count =0; // Variable counts input by user maximum of 15
    int showpid[15]; // Array stores showpid data
    int incr;   //  Variable counts pids
    
    while( 1 )
    {
        // Print out the msh prompt
        printf ("msh> ");
        // Read the command from the commandline.  The
        // maximum command that will be read is MAX_COMMAND_SIZE
        // This while command will wait here until the user
        // inputs something since fgets returns NULL when there
        // is no input
        while( !fgets (cmd_str, MAX_COMMAND_SIZE, stdin) );
        if(cmd_str[0] == '\n') continue;
        strcpy(history_List[count], cmd_str); // Copy command from user to an array
        count++;
        /* Parse input */
        char *token[MAX_NUM_ARGUMENTS];
        
        int token_count = 0;
        // Pointer to point to the token
        // parsed by strsep
        char *argument_ptr;
        
        char *working_str  = strdup( cmd_str );
        // we are going to move the working_str pointer so
        // keep track of its original value so we can deallocate
        // the correct amount at the end
        char *working_root = working_str;
        
        // Tokenize the input stringswith whitespace used as the delimiter
        while ( ( (argument_ptr = strsep(&working_str, WHITESPACE ) ) != NULL) &&
               (token_count<MAX_NUM_ARGUMENTS))
        {
            token[token_count] = strndup( argument_ptr, MAX_COMMAND_SIZE );
            if( strlen( token[token_count] ) == 0 )
            {
                token[token_count] = NULL;
            }
            token_count++;
        }
        
        
        if(count==15) // Check and assign counter to 0 equals to maxmimum command of 15
        {
            count = 0;
        }
        else if(strcmp(token[0],"exit")==0) // Compares and exit
        {
            exit(0);
        }
        else if (strcmp(token[0],"quit")==0) // Compare and exit
        {
            exit(0);
        }
        else if(strcmp(token[0], "cd")==0)  // Compare and help execution
        {
            chdir(token[1]);                 // Execution of cd takes place
        }
        else if (strcmp(token[0],"history")==0) // Compare command inputed by user
        {
            historyList(history_List,count);      // List the all 15 user input value
        }
        else if(strcmp(token[0], "showpids")==0)  // Compare user
        {
            for(int i =0; i<incr; i++)           // Display all the pids
            {
                printf("%d\n", showpid[i]);
            }
        }
        else
        {
            pid_t pid = fork();           // Execution and formation of pids
            
            if(pid == 0)
            {
                int pid_first = execvp(token[0], &token[0]);  // Execution of execvp function
                if(pid_first == -1 )
                {
                    printf("%s: command not found\n", token[0]);
                    
                }
                
            }
            else if (pid <0 )
            {
                printf("process not created");
            }
            else
            {
                showpid[incr] = pid;
                incr++;
                if(incr >14)
                {
                    incr = 0;
                }
                int status;
                wait(&status);       // Waits if function is not ready
            }
        }
        
        
        free( working_root );
        
    }
    return 0;
}
// Displays all the command entered by user max command it can diaply is 15
void historyList(char history_List[15][255] , int count )
{
    int i = 0;
    for(i = 0;i<count; i++)
    {
        printf("%s", history_List[i]);   // Prints all the command stored in array
    }
}
