#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_COMMAND_LEN 4096
void mycopy(int argc, const char **argv);
int main(int argc, char const *argv[])
{
    mycopy(argc, argv);
    return 0;
}

void mycopy(int argc, const char **argv)
{
    char *command = (char *)malloc(sizeof(char) * MAX_COMMAND_LEN);
    memset(command, '\0', MAX_COMMAND_LEN);
    strcpy(command, "cp");
    for (int i = 1; i < argc; i++)
    {
        int index = 0;
        strcat(command, " ");
        strcat(command, argv[i]);
    }
    system(command);
    free(command);
}