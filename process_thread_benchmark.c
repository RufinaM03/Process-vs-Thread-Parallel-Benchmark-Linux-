#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <time.h>

#define BILLION 1000000000L


// CPU-bound workload
long long compute_task(long long n) {
    long long sum = 0;
    for (long long i = 0; i < n; i++) {
        sum += i % 7;
    }
    return sum;
}


// THREAD SECTION
typedef struct {
    long long workload;
    long long result;
} thread_data;

void* thread_function(void* arg) {
    thread_data* data = (thread_data*)arg;
    data->result = compute_task(data->workload);
    return NULL;
}

double time_diff(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec) / (double)BILLION;
}

int main(int argc, char* argv[]) {

    if (argc < 3) {
        printf("Usage: %s <num_workers> <workload>\n", argv[0]);
        return 1;
    }

    int num_workers = atoi(argv[1]);
    long long workload = atoll(argv[2]);

    printf("Workers: %d | Workload per worker: %lld\n\n",
           num_workers, workload);

    // PROCESS VERSION
    printf("=== Multi-Process Execution ===\n");

    struct timespec start_p, end_p;
    clock_gettime(CLOCK_MONOTONIC, &start_p);

    int pipes[num_workers][2];
    pid_t pid;

    for (int i = 0; i < num_workers; i++) {
        pipe(pipes[i]);
        pid = fork();

        if (pid == 0) {
            close(pipes[i][0]);

            long long result = compute_task(workload);
            write(pipes[i][1], &result, sizeof(result));

            close(pipes[i][1]);
            exit(0);
        }
    }

    long long total_process = 0;

    for (int i = 0; i < num_workers; i++) {
        close(pipes[i][1]);

        long long child_result;
        read(pipes[i][0], &child_result, sizeof(child_result));
        total_process += child_result;

        close(pipes[i][0]);
        wait(NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end_p);
    double process_time = time_diff(start_p, end_p);

    printf("Total (Process): %lld\n", total_process);
    printf("Execution Time (Process): %.6f sec\n\n", process_time);

    // THREAD VERSION
    printf("=== Multi-Thread Execution ===\n");

    struct timespec start_t, end_t;
    clock_gettime(CLOCK_MONOTONIC, &start_t);

    pthread_t threads[num_workers];
    thread_data tdata[num_workers];

    for (int i = 0; i < num_workers; i++) {
        tdata[i].workload = workload;
        pthread_create(&threads[i], NULL, thread_function, &tdata[i]);
    }

    long long total_thread = 0;

    for (int i = 0; i < num_workers; i++) {
        pthread_join(threads[i], NULL);
        total_thread += tdata[i].result;
    }

    clock_gettime(CLOCK_MONOTONIC, &end_t);
    double thread_time = time_diff(start_t, end_t);

    printf("Total (Thread): %lld\n", total_thread);
    printf("Execution Time (Thread): %.6f sec\n\n", thread_time);

    // Comparison
    printf("=== Performance Comparison ===\n");
    printf("Process Time: %.6f sec\n", process_time);
    printf("Thread Time : %.6f sec\n", thread_time);

    if (process_time > thread_time)
        printf("Threads were faster.\n");
    else
        printf("Processes were faster.\n");

    return 0;
}
