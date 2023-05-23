#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <semaphore.h>
#include <sys/mman.h>
int main(int argc, char const *argv[])
{
    sem_t *sem_1;
    sem_1 = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_init(sem_1, 1, 0);
    int pipeFd[2];
    pid_t child_1, child_2;

    if (pipe(pipeFd) == -1)
    {
        perror("error occured while creating pipe");
        exit(1);
    }

    child_1 = fork();
    if (child_1 < 0)
    {
        perror("error occured while executing fork");
        exit(1);
    }

    if (child_1 == 0)
    {

        close(pipeFd[0]);
        char message[128];
        memset(message, '\0', sizeof(message));

        sprintf(message, "Child process #1 (pid: %d) is sending a message\n", getpid());
        printf("%s", message);
        write(pipeFd[1], message, strlen(message));
        close(pipeFd[1]);
        sem_post(sem_1);
        exit(0);
    }

    sem_wait(sem_1);
    child_2 = fork();

    if (child_2 < 0)
    {
        perror("error occured while executing fork");
        exit(1);
    }

    if (child_2 == 0)
    {
        close(pipeFd[0]);
        char message[128];
        memset(message, '\0', sizeof(message));

        sprintf(message, "Child process #2 (pid: %d) is sending a message\n", getpid());
        printf("%s", message);
        write(pipeFd[1], message, strlen(message));
        close(pipeFd[1]);
        sem_post(sem_1);
        exit(0);
    }

    sem_wait(sem_1);
    if (child_1 > 0 && child_2 > 0)
    {

        close(pipeFd[1]);
        printf("\n=========================\n");
        char buffer[128];
        int len;
        while ((len = read(pipeFd[0], buffer, sizeof(buffer))) > 0)
        {
            buffer[len] = '\0';
            printf("father process (pid: %d):start fetching messages from pipe\n%s", getpid(), buffer);
        }
        printf("father process (pid: %d): task finished\n", getpid());
        close(pipeFd[0]);
        sem_destroy(sem_1);
        return 0;
    }
    return 1;
}
