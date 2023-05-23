#ifndef _MESSAGE_H
#define __MESSAGE_H

#define MAX_MSG_LEN 255
#define MSG_TYPE 114514

typedef struct prototype_message
{
    long type;
    char text[MAX_MSG_LEN];
} message;

#endif