#include <stdio.h>   // Standard Input/Output
#include <unistd.h>  // Required for fork()

int main() {
    fork(); // Create a child process
    fork(); // Create a child process
    printf("Hello, World!\n");
    return 0;
}
