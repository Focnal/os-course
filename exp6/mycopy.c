#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <libgen.h>
int debug_flag = 0;

#define MAX_BUFFER_SIZE 4096
#define MAX_PATH_LEN 4096
void copyDir(const char *src, const char *dest);
void copyFile(const char *src, const char *dest);
void printSpace(int count)
{
    for (int i = count; i > 0; i--)
    {
        printf(" ");
    }
}

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("用法: %s <源路径> <目的路径>\n", argv[0]);
        return 1;
    }

    struct stat srcSt;
    if (stat(argv[1], &srcSt) != 0)
    {
        printf("%s: 对 '%s' 调用 stat 失败：没有那个文件或目录\n", argv[0], argv[1]);
        return 1;
    }

    if (S_ISDIR(srcSt.st_mode))
    {
        // 这是任务提示
        int srclen = strlen(argv[1]), destlen = strlen(argv[2]);
        int tab = (srclen >= destlen ? srclen : destlen) / 2;
        printf("复制任务开始: \n");
        if (srclen <= destlen)
        {
            printSpace((destlen - srclen) / 2);
        }
        printf("%s\n", argv[1]);
        printSpace(tab);
        printf("|\n");
        printSpace(tab);
        printf("v\n");
        if (srclen > destlen)
        {
            printSpace((srclen - destlen) / 2);
        }
        printf("%s\n", argv[2]);
        // 任务提示结束

        copyDir(argv[1], argv[2]);
    }
    else
    {
        // 判断目的路径是否为已存在和是否为目录
        struct stat destSt;
        if (stat(argv[2], &destSt) == 0 && S_ISDIR(destSt.st_mode))
        {
            char _dest[MAX_PATH_LEN], tmp[MAX_PATH_LEN];
            strncpy(tmp, argv[1], MAX_PATH_LEN);
            snprintf(_dest, MAX_PATH_LEN, "%s/%s", argv[2], basename(tmp));

            // 这是任务提示
            int srclen = strlen(argv[1]), destlen = strlen(_dest);
            int tab = (srclen >= destlen ? srclen : destlen) / 2;
            printf("复制任务开始: \n");
            if (srclen <= destlen)
            {
                printSpace((destlen - srclen) / 2);
            }
            printf("%s\n", argv[1]);
            printSpace(tab);
            printf("|\n");
            printSpace(tab);
            printf("v\n");
            if (srclen > destlen)
            {
                printSpace((srclen - destlen) / 2);
            }
            printf("%s\n", _dest);
            // 任务提示结束

            copyFile(argv[1], _dest);
        }
        else
        {
            // 这是任务提示
            int srclen = strlen(argv[1]), destlen = strlen(argv[2]);
            int tab = (srclen >= destlen ? srclen : destlen) / 2;
            printf("复制任务开始: \n");
            if (srclen <= destlen)
            {
                printSpace((destlen - srclen) / 2);
            }
            printf("%s\n", argv[1]);
            printSpace(tab);
            printf("|\n");
            printSpace(tab);
            printf("v\n");
            if (srclen > destlen)
            {
                printSpace((srclen - destlen) / 2);
            }
            printf("%s\n", argv[2]);
            // 任务提示结束

            copyFile(argv[1], argv[2]);
        }
    }
    return 0;
}

void copyDir(const char *src, const char *dest)
{
    DIR *dir;

    char subSrc[MAX_PATH_LEN], subDest[MAX_PATH_LEN];
    mkdir(dest, 0755);

    dir = opendir(src);

    if (dir == NULL)
    {
        perror("打不开");
        exit(EXIT_FAILURE);
    }

    for (struct dirent *entry; (entry = readdir(dir)) != NULL;)
    {
        if (!strcmp(".", entry->d_name) || !strcmp("..", entry->d_name))
        {
            continue;
        }

        snprintf(subSrc, sizeof(subSrc), "%s/%s", src, entry->d_name);
        snprintf(subDest, sizeof(subDest), "%s/%s", dest, entry->d_name);
        if (debug_flag)
            printf("copy: %s => %s\n", subSrc, subDest);
        if (entry->d_type == DT_DIR)
        {

            copyDir(subSrc, subDest);
        }
        else
        {
            copyFile(subSrc, subDest);
        }
    }
    closedir(dir);
}

void copyFile(const char *src, const char *dest)
{
    int destFD, srcFD;
    char buffer[MAX_BUFFER_SIZE];

    destFD = open(dest, O_CREAT | O_WRONLY, 0755);
    srcFD = open(src, O_RDONLY);
    if (debug_flag)
        printf("copy: %s => %s\n", src, dest);

    for (int size = 0; (size = read(srcFD, buffer, MAX_BUFFER_SIZE * sizeof(char))) != 0; write(destFD, buffer, size))
        ;

    close(destFD);
    close(srcFD);
}
