#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <errno.h>

#define MSG_KEY_INFO 1234      // Message queue key for broadcasting
#define MSG_KEY_QUERY 5678     // Message queue key for queries
#define MSG_SIZE 100           // Maximum message size
#define NUM_FARMERS 20         // Number of farmers

// Message structure
struct msg_buffer {
    long msg_type;
    char msg_text[MSG_SIZE];
};

int main() {
    int msg_id_info, msg_id_query;
    struct msg_buffer message;

    // Create message queue for broadcasting information
    msg_id_info = msgget(MSG_KEY_INFO, 0666 | IPC_CREAT);
    if (msg_id_info == -1) {
        perror("msgget for info failed");
        exit(EXIT_FAILURE);
    }

    // Create message queue for receiving queries
    msg_id_query = msgget(MSG_KEY_QUERY, 0666 | IPC_CREAT);
    if (msg_id_query == -1) {
        perror("msgget for queries failed");
        exit(EXIT_FAILURE);
    }

    // Broadcast information for each farmer
    for (int i = 0; i < NUM_FARMERS; i++) {
        message.msg_type = 1;  // Arbitrary message type for broadcast
        strcpy(message.msg_text, "Pesticide info: Use organic pesticides for better yield.");

        if (msgsnd(msg_id_info, &message, sizeof(message.msg_text), 0) == -1) {
            perror("msgsnd failed");
            exit(EXIT_FAILURE);
        }
        printf("Broadcasting agricultural information to farmer %d...\n", i + 1);
    }

    // Handle incoming farmer queries
    for (int i = 0; i < NUM_FARMERS; i++) {  // Simulate handling queries from 3 farmers
        if (msgrcv(msg_id_query, &message, sizeof(message.msg_text), 2, 0) == -1) {  // msg_type = 2 for queries
            perror("msgrcv for query failed");
            exit(EXIT_FAILURE);
        }
        printf("Received query from farmer %d: %s", i + 1, message.msg_text);
    }

    // Cleanup message queues
    msgctl(msg_id_info, IPC_RMID, NULL);   // Remove broadcast queue
    msgctl(msg_id_query, IPC_RMID, NULL);  // Remove query queue

    return 0;
}
