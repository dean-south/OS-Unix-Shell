#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>

//I call this data structure a linked-array. It's an array with a pointer to the next array
struct stringHolder
{
    char *string;
    struct stringHolder *next_pointer;
};

// function to initialize the array-list and set the length of the string
struct stringHolder *initStingHolder(const char *string, size_t string_size)
{
    struct stringHolder *holder = (struct stringHolder *)malloc(sizeof(struct stringHolder));

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
void addPointer(struct stringHolder *holder, struct stringHolder *pointer){
    holder->next_pointer = pointer;
}

// function that frees up the space in memory used for the array-list
void freeStringHolder(struct stringHolder *holder)
{
    struct stringHolder *pointer = holder;
    struct stringHolder *temp;
    while (pointer != NULL)
    {
        temp = pointer->next_pointer;
        free(pointer);
        pointer = temp;
    }
    
}


int main(int argc, char const *argv[])
{
    if (argc > 1){
        printf("%s", &argv);

        return 0;

    }


    char *inputString = NULL;  // Pointer to store the input String
    size_t len = 0;     // Initial size of buffer
    ssize_t read;

    char *token, *rest;
    const char *delim = " ";

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

            struct stringHolder *head = initStingHolder(token, strlen(token));

            struct stringHolder *pointer = head;

            while ((token = strsep(&rest, delim)) != NULL)  
            {
                
                addPointer(pointer, initStingHolder(token, strlen(token)));
                
                pointer = pointer->next_pointer;
            }

            pointer = head;

            //print array-list
            // while (pointer != NULL)
            // {
            //     printf("%s \n", pointer->string);
            //
            //     pointer = pointer->next_pointer;
            // }

            if (strcmp(head->string, "exit") == 0 || 1==1)
            {
                exit(0);
            } 

            freeStringHolder(head);
            free(pointer);

        } else {
            printf("\n");
            exit(0);
        }

        
    }

    free(inputString);  // Remember to free the dynamically allocated memory 
    
    return 0;
}