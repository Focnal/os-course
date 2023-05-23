#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char const *argv[])
{
    if (argc == 3)
    {

        char cmd[4096];
        memset(cmd, '\0', sizeof(cmd));
        snprintf(cmd, 4095, "cp %s %s\n", argv[1], argv[2]);
        system(cmd);
        return 0;
    }
    if (argc == 4)
    {
        char cmd[4096];
        memset(cmd, '\0', sizeof(cmd));
        snprintf(cmd, 4095, "cp %s %s %s\n", argv[1], argv[2], argv[3]);
        system(cmd);
        return 0;
    }

    system("cp --help");
    return 1;
}
