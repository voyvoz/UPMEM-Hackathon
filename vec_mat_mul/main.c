#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/resource.h>

uint64_t get_time()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return 1000 * t.tv_sec + (t.tv_usec / 1000);
}

void init(void);

void transferToDPUs(void);
void run(void);
void transferFromDPUs(void);

void end(void);

int main(void)
{
    bool uselock = true;
    int fd = open("/tmp/UPMEM.lock", O_RDWR);
    if(fd == -1)
        uselock = false;
    
    if(uselock)
    {
        printf("Waiting for execution on server...\n");        
        lockf(fd, F_LOCK, 0);
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
    
    if(uselock)
    {
        lockf(fd, F_ULOCK, 0);
    }

    return 0;
}
