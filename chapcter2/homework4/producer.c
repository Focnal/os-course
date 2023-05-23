#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "message.h"

int main() {
    int msgid;
    key_t key;
    message msg;
    
    key = ftok(".", 123); //create message queue
    msgid = msgget(key, IPC_CREAT | 0666);
    if (msgid == -1) {
        perror("error occurs in function msgget\n");
        exit(1);
    }
    

    while (1) {
        printf("Producer: Input message> ");
        fgets(msg.text, MAX_MSG_LEN, stdin); //get input
        msg.type = MSG_TYPE;
        msgsnd(msgid, &msg, sizeof(msg.text), 0); //send message
        printf("Producer: Message sent: %s\n", msg.text);
    }

}
