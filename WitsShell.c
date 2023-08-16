#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

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

char* exit_statements[] = {"Fine leave me, like everyone else does\n", "Everyone eventually leaves me. You're no different\n", 
"One day you'll realise how of a mistake it is to leave me\n", "Fine Leave me, I never liked you anyway\n", 
"Hope you learn to cherish the time we spent together after you leave me\n"};

char** getArg(struct StringHolder *pointer){
    char** arg[pointer->numStrings - 1];
    
    if (pointer->numStrings > 1)
    {

        for (size_t i = 1; i < pointer->numStrings; i++){
            pointer = pointer->next_pointer;

            arg[i] = &pointer->string;
        }
        
    }
    
    return arg;
}


int main(int argc, char const *argv[])
{

    struct StringHolder *path = initStingHolder("/bin/");

    char *token, *rest;
    const char *delim = " ";

    srand(time(NULL));


    if (argc > 1){
        printf("%s", &argv);

        return 0;
    }


    char *inputString = NULL;  // Pointer to store the input String
    size_t len = 0;     // Initial size of buffer
    ssize_t read;


    while (true)
    {
        printf("witsshell> ");
        read = getline(&inputString, &len, stdin);

        if (read != -1) {
            //remove \n from the end of the input string
            inputString[strcspn(inputString, "\n")] = '\0';

            token = inputString;
            rest = inputString;

            token = strsep(&rest, delim);

            struct StringHolder *head = initStingHolder(token);

            head->numStrings = 1;

            struct StringHolder *pointer = head;

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
                printf("%s", exit_statements[rand() % 5]);
                exit(0);
            }
            else if (strcmp(head->string, "path") == 0)
            {
                updatePath(head, path);
            }
            else if (strcmp(head->string, "cd") == 0)
            {
                printf("fuck off \n");
            }
            else
            {
                pointer = path;
                while (pointer != NULL)
                {
                    char *str_search = strcat(pointer->string, head->string);
                    if (access(str_search, X_OK) == 0)
                    {

                        int pid = fork();
                        if (pid == 0){
                            char** arg = getArg(head);
                            execv(pointer->string, arg);
                        }
                        else {
                            wait(NULL);
                            break;
                        }
                        
                    }

                    pointer = pointer->next_pointer;

                    
                }
                
                
                
            }
            

            freeStringHolder(head);
            //free(pointer);

        } else {
            printf("%s", exit_statements[rand()%5]);
            exit(0);
        }

        
    }

    free(inputString);  // Remember to free the dynamically allocated memory 
    
    return 0;
}