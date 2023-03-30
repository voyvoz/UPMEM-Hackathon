#include <stdio.h>
#include <dpu.h>
#include <dpu_log.h>

struct dpu_set_t set;

void init(void)
{
    DPU_ASSERT(dpu_alloc(1, NULL, &set)); // for simulator: "backend=simulator" instead of NULL
    DPU_ASSERT(dpu_load(set, "./hello_world", NULL));
}

void transferToDPUs(void)
{

}

void run(void)
{
    DPU_ASSERT(dpu_launch(set, DPU_SYNCHRONOUS));
}

void transferFromDPUs(void)
{

}

void end(void)
{
    struct dpu_set_t dpu;
    DPU_FOREACH(set, dpu) {
        DPU_ASSERT(dpu_log_read(dpu, stdout));
    }

    DPU_ASSERT(dpu_free(set));
}
