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
    
    key = ftok(".", 123); //fetch message queue
    msgid = msgget(key, 0666);
    if (msgid == -1) {
        perror("error occurs in fucntion msgget\n");
        exit(1);
    }
    

    while (1) {
        msgrcv(msgid, &msg, sizeof(msg.text), MSG_TYPE, 0);  //receive message
        printf("Consumer: Message received: %s", msg.text);
    }

}
