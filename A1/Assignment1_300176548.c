#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#define BUFFER_SIZE 50
#define READ_END 0
#define WRITE_END 1

int main(void) {
    
    char write_msg[BUFFER_SIZE];
    char read_msg[BUFFER_SIZE];
    pid_t pid;
    
    int parent_to_child[2], child_to_parent[2];  /* 2 pipes */

    /* create the pipe */
    if (pipe(parent_to_child) == -1 || pipe(child_to_parent) == -1) {
        fprintf(stderr, "Pipe failed\n");
        return 1;
    }

    /* fork a child process */
    pid = fork();

    if (pid < 0) {  /* error occurred */
        fprintf(stderr, "Fork failed\n");
        return 1;
    }

    if (pid > 0) {  /* parent process */
        close(parent_to_child[READ_END]); // Close unused read end
        close(child_to_parent[WRITE_END]); // Close unused write end

        // 1st Message (Parent → Child)
        strcpy(write_msg, "Greetings son");
        write(parent_to_child[WRITE_END], write_msg, strlen(write_msg) + 1);
        sleep(2); // Delay

        // 2nd Message (Child → Parent)
        read(child_to_parent[READ_END], read_msg, BUFFER_SIZE);
        printf("%s\n", read_msg);
        sleep(2); // Delay

        // 3rd Message (Parent → Child)
        strcpy(write_msg, "How are you son");
        write(parent_to_child[WRITE_END], write_msg, strlen(write_msg) + 1);
        sleep(2); // Delay

        // 4th Message (Child → Parent)
        read(child_to_parent[READ_END], read_msg, BUFFER_SIZE);
        printf("%s\n", read_msg);

        // Close pipes
        close(parent_to_child[WRITE_END]);
        close(child_to_parent[READ_END]);

    } else {  // Child Process
        close(parent_to_child[WRITE_END]); // Close unused write end
        close(child_to_parent[READ_END]); // Close unused read end

        // 1st Message (Parent → Child)
        read(parent_to_child[READ_END], read_msg, BUFFER_SIZE);
        printf("%s\n", read_msg);
        sleep(2); // Delay

        // 2nd Message (Child → Parent)
        strcpy(write_msg, "Hello dad");
        write(child_to_parent[WRITE_END], write_msg, strlen(write_msg) + 1);
        sleep(2); // Delay

        // 3rd Message (Parent → Child)
        read(parent_to_child[READ_END], read_msg, BUFFER_SIZE);
        printf("%s\n", read_msg);
        sleep(2); // Delay

        // 4th Message (Child → Parent)
        strcpy(write_msg, "Fine dad thanks");
        write(child_to_parent[WRITE_END], write_msg, strlen(write_msg) + 1);

        // Close pipes
        close(parent_to_child[READ_END]);
        close(child_to_parent[WRITE_END]);
    }

    return 0;
}
