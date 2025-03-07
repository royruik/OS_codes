// Roy Rui 300176548
// -pthread command is needed to compile the problem


#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

// Shared variables
int a, b, turn; 
pthread_mutex_t lock;  // Mutex for synchronization

// Thread function prototypes
void* thread_function(void* arg);

// Thread function
void* thread_function(void* arg) {
    int id = *(int*)arg;  // Thread ID

    for (int i = 0; i < a; i++) { // Repeat for 'a' cycles
        while (turn != id);  // Busy-wait for the turn
        
        pthread_mutex_lock(&lock); // Enter critical section
        
        // Update 'b' based on thread ID
        b += (id + 1);
        printf("Thr%d, (b+%d=%d)\n", id, id+1, b);

        // Set turn for next thread (Round Robin)
        turn = (turn + 1) % 4;

        pthread_mutex_unlock(&lock); // Exit critical section
    }
    return NULL;
}

// Function to compute Fibonacci sequence up to 'b'
void fibonacci(int n) {
    int t1 = 1, t2 = 1, nextTerm;
    printf("Fibonacci sequence up to %d: ", n);
    for (int i = 1; i <= n; i++) {
        printf("%d ", t1);
        nextTerm = t1 + t2;
        t1 = t2;
        t2 = nextTerm;
    }
    printf("\n");
}

int main() {
    // User input
    printf("Enter integer value of a (number of RR cycles): ");
    scanf("%d", &a);
    printf("Enter integer value of b (initial value): ");
    scanf("%d", &b);
    printf("Enter the Thread # to start first (0 to 3): ");
    scanf("%d", &turn);

    pthread_t threads[4];
    int thread_ids[4] = {0, 1, 2, 3};

    pthread_mutex_init(&lock, NULL); // Initialize mutex

    // Create 4 threads
    for (int i = 0; i < 4; i++) {
        pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Parent: Final value of b = %d\n", b);

    // Compute Fibonacci sequence
    fibonacci(b);

    // Cleanup
    pthread_mutex_destroy(&lock);
    return 0;
}
