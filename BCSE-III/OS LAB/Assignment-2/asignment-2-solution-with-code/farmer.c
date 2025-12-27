#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <errno.h>

#define MSG_KEY_INFO 1234      // Message queue key for broadcasting
#define MSG_KEY_QUERY 5678     // Message queue key for queries
#define MSG_SIZE 100           // Maximum message size

// Message structure
struct msg_buffer {
    long msg_type;
    char msg_text[MSG_SIZE];
};

int main() {
    int msg_id_info, msg_id_query;
    struct msg_buffer message;

    // Open the message queue for receiving the broadcast
    msg_id_info = msgget(MSG_KEY_INFO, 0666);
    if (msg_id_info == -1) {
        perror("msgget for info failed");
        exit(EXIT_FAILURE);
    }

    // Receive the broadcasted information
    if (msgrcv(msg_id_info, &message, sizeof(message.msg_text), 1, 0) == -1) {  // msg_type = 1 for broadcast
        perror("msgrcv failed");
        exit(EXIT_FAILURE);
    }
    printf("Farmer received broadcast: %s", message.msg_text);

    // Sending a query back to the broadcaster
    msg_id_query = msgget(MSG_KEY_QUERY, 0666);
    if (msg_id_query == -1) {
        perror("msgget for query failed");
        exit(EXIT_FAILURE);
    }

    message.msg_type = 2;  // Arbitrary message type for query
    strcpy(message.msg_text, "Justice for Farmers!!!\n");
    
    if (msgsnd(msg_id_query, &message, sizeof(message.msg_text), 0) == -1) {
        perror("msgsnd failed");
        exit(EXIT_FAILURE);
    }
    printf("Query sent by farmer.\n");

    return 0;
}
