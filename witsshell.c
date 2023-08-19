#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

/* To Do
- Bash
- redirection >
- parallel &
*/


char* exit_statements[] = {"Fine leave me, like everyone else does\n", "Everyone eventually leaves me. You're no different\n", 
"One day you'll realise how much of a mistake it was to leave me\n", "Fine Leave me, I never liked you anyway\n", 
"Hope you learn to cherish the time we spent together after you leave me\n", "It's fine if you leave, I'm too good for you anyway\n", 
"When you realise leaving me was a mistake, it will be too late and I'll be long gone\n", "I hope you know whenever you leave me, it hurts me a lot\n",
"Leave me, one day I'll hurt you just as much as you hurt me\n", "Please don't leave me :(\n", "Why are you leaving me? Are you trying to hurt me? \n"};

char error_message[30] = "An error has occurred\n";

//I call this data structure a linked-array. It's an array with a pointer to the next array
struct StringHolder
{
    char *string;
    struct StringHolder *next_pointer;
    int numStrings;
};

// function to initialize the array-list and set the length of the string
struct StringHolder *initStingHolder(const char *string)
{
    struct StringHolder *holder = (struct StringHolder *)malloc(sizeof(struct StringHolder));

    size_t string_size = strlen(string);

    if (holder != NULL)
    {
        holder->string = (char *)malloc(string_size + 1); 
        if (holder->string != NULL)     
        {
            snprintf(holder->string, string_size + 1, "%s",string);
        }  
    }

    return holder;
}

// fucntion that adds a pointer to the next array-list
void addPointer(struct StringHolder *holder, struct StringHolder *pointer){
    holder->next_pointer = pointer;
}

// function that frees up the space in memory used for the array-list
void freeStringHolder(struct StringHolder *holder)
{
    struct StringHolder *pointer = holder;
    struct StringHolder *temp;
    while (pointer != NULL)
    {
        temp = pointer->next_pointer;
        free(pointer);
        pointer = temp;
    }
    
}

void updatePath(struct StringHolder *head, struct StringHolder *path){
    
    int pid = fork();

    if (pid == 0)
    {
        free(path);

        path = initStingHolder(head->next_pointer->string);

        struct StringHolder *pathPtr = path;

        struct StringHolder *headPtr = head->next_pointer;

        while (headPtr != NULL)         
        {
            addPointer(pathPtr, initStingHolder(headPtr->string));
            
            pathPtr = pathPtr->next_pointer;
            headPtr = headPtr->next_pointer;
        }
    }
    else
    {
        wait(NULL);
    }
    
}

char** getArg(struct StringHolder *pointer){
    char* arg[pointer->numStrings - 1];
    
    if (pointer->numStrings > 1)
    {

        for (size_t i = 1; i < pointer->numStrings; i++){
            pointer = pointer->next_pointer;

            arg[i] = &pointer->string;
        }
        
    }
    
    return arg;
}

int countStrings(char **array) {
    int count = 0;

    while (array[count] != NULL) {
        count++;
    }

    return count;
}

void cd(struct StringHolder *head){

    int pid = fork();
    if (pid == 0){
        if (head->numStrings != 2 || chdir(head->next_pointer->string) != 0)
        {
            printError();
        }   
    }
    else{
        wait(NULL);
    }
}

void exitShell(struct StringHolder *head){

    srand(time(NULL));

    if (head->numStrings == 1){
        printf("%s", exit_statements[rand() % countStrings(exit_statements)]);
        exit(0);
    }
    else{
        printf("You can't leave me just yet ;)\n");
        printError();
    }
}

void printError(){
    write(STDERR_FILENO, error_message, strlen(error_message));
}

int main(int argc, char const *argv[])
{

    struct StringHolder *path = initStingHolder("/bin/");

    char *token, *rest;
    const char *delim = " ";

    char *inputString = NULL;  // Pointer to store the input String
    size_t len = 0;     // Initial size of buffer
    ssize_t read;

    struct StringHolder *head;
    struct StringHolder *pointer;

    if (argc == 1)
    {
        while (true)
        {
            printf("Hello! How can I help you Today :)");
            printf("witsshell> ");

            read = getline(&inputString, &len, stdin);

            if (read != -1) {
                //remove \n from the end of the input string
                inputString[strcspn(inputString, "\n")] = '\0';

                token = inputString;
                rest = inputString;

                token = strsep(&rest, delim);

                head = initStingHolder(token);

                head->numStrings = 1;

                pointer = head;

                while ((token = strsep(&rest, delim)) != NULL)  
                {
                    addPointer(pointer, initStingHolder(token));

                    head->numStrings++;
                    
                    pointer = pointer->next_pointer;
                }

                //print array-list
                // while (pointer != NULL)
                // {
                //     printf("%s \n", pointer->string);
                //
                //     pointer = pointer->next_pointer;
                // }


                if (strcmp(head->string, "exit") == 0)
                {
                    exitShell(head);

                }
                else if (strcmp(head->string, "path") == 0)
                {
                    updatePath(head, path);
                }
                else if (strcmp(head->string, "cd") == 0)
                {

                    cd(head);
                }
                else
                {
                    pointer = path;
                    while (pointer != NULL)
                    {
                        char *str_search = strdup(pointer->string);
                        strcat(str_search, head->string);

                        if (access(str_search, X_OK) == 0)
                        {

                            int pid = fork();
                            if (pid == 0){
                                char** arg = getArg(head);
                                execv(str_search, arg);
                            }
                            else {
                                wait(NULL);
                                break;
                            }
                            
                        }

                        pointer = pointer->next_pointer;  
                    }   
                }
                //free(pointer);

            } else {
                srand(time(NULL));

                printf("%s", exit_statements[rand() % countStrings(exit_statements)]);
                exit(0);
            }

            
        }
    }
    else if (argc > 2)
    {
        printError();
        exit(1);
    }
    else // This is if an arg is passed to ./witsshell
    {
        FILE *file = fopen(argv[1], "r"); // Open for reading ("r" mode)
        if (file == NULL){
            printError();
            exit(1);
        }
        
        while (fgets(inputString, sizeof(inputString), file) != NULL) {
                inputString[strcspn(inputString, "\n")] = '\0';

                token = inputString;
                rest = inputString;

                token = strsep(&rest, delim);

                head = initStingHolder(token);

                head->numStrings = 1;

                pointer = head;

                // while ((token = strsep(&rest, delim)) != NULL)  
                // {
                //     addPointer(pointer, initStingHolder(token));

                //     head->numStrings++;
                    
                //     pointer = pointer->next_pointer;
                // }

                // //print array-list
                // // while (pointer != NULL)
                // // {
                // //     printf("%s \n", pointer->string);
                // //
                // //     pointer = pointer->next_pointer;
                // // }


                // if (strcmp(head->string, "exit") == 0)
                // {
                //     exitShell(head);

                // }
                // else if (strcmp(head->string, "path") == 0)
                // {
                //     updatePath(head, path);
                // }
                // else if (strcmp(head->string, "cd") == 0)
                // {

                //     cd(head);
                // }
                // else
                // {
                //     pointer = path;
                //     while (pointer != NULL)
                //     {
                //         char *str_search = strdup(pointer->string);
                //         strcat(str_search, head->string);

                //         if (access(str_search, X_OK) == 0)
                //         {

                //             int pid = fork();
                //             if (pid == 0){
                //                 char** arg = getArg(head);
                //                 execv(str_search, arg);
                //             }
                //             else {
                //                 wait(NULL);
                //                 break;
                //             }
                            
                //         }

                //         pointer = pointer->next_pointer;  
                //     }   
                // }
        }
        freeStringHolder(head);
    }

    free(inputString);  // Remember to free the dynamically allocated memory 
    
    return 0;
}