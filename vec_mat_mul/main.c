#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <stdbool.h>

#include <sys/time.h>
#include <sys/resource.h>

uint64_t get_time()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return 1000 * t.tv_sec + (t.tv_usec / 1000);
}

#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>

sem_t *sem;
void intHandler(int dummy) {
    sem_post(sem);
    exit(-1);
}

void init(void);

void transferToDPUs(void);
void run(void);
void transferFromDPUs(void);

void end(void);

int main(void)
{
    bool use_sem = true;
    sem_t *sem = sem_open("UPMEM-SEM", 0);
    if(sem == SEM_FAILED) {
        //perror("Opening sem failed");
        use_sem = false;
    }

    if(use_sem)
    {
        struct sigaction sa = {
            .sa_handler = intHandler,
            .sa_flags = 0
        };
        sigemptyset(&sa.sa_mask);
        sigaction(SIGSEGV, &sa, NULL);
        sigaction(SIGINT, &sa, NULL);

        printf("Waiting for execution on server...\n");
        if(sem_wait(sem)) {
            perror("sem wait failed");
            return -1;
        }
        printf("=> go\n");
    }

    init();

    // copy data to DPUs
    uint64_t begin_cpy_to_dpu = get_time();
    transferToDPUs();
    uint64_t end_cpy_to_dpu = get_time();
    uint64_t time_cpy_to_dpu = (uint64_t)(end_cpy_to_dpu - begin_cpy_to_dpu);

    // launch program
    uint64_t begin_exec = get_time();
    run();
    uint64_t end_exec = get_time();
    uint64_t time_exec = (uint64_t)(end_exec - begin_exec);

    // copy result back
    uint64_t begin_cpy_back = get_time();
    transferFromDPUs();
    uint64_t end_cpy_back = get_time();
    uint64_t time_cpy_back = (uint64_t)(end_cpy_back - begin_cpy_back);

    end();

    // print timings
    printf("Cpy to: \t%ld ms\n", time_cpy_to_dpu);
    printf("Exec: \t\t%ld ms\n", time_exec);
    printf("Cpy back: \t%ld ms\n", time_cpy_back);

    if(use_sem)
    {
        sem_post(sem);
    }

    return 0;
}
