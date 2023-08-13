#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>

int main(int argc, char const *argv[])
{
    char input[] = "apple,banana,cherry,date";
    char *token, *rest = input;
    const char *delim = ",";

    while ((token = strsep(&rest, delim)) != NULL) {
        printf("%zu \n", strlen(token));
        printf("Token: %s\n", token);
    }


    return 0;
}
