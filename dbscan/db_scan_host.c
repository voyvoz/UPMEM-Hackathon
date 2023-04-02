#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <dpu.h>
#include <dpu_log.h>
#include "db.h"


// TODO: declare DPU set
extern struct database* db;

void init(void)
{
    // TODO: alloc DPUs and load program
}

void transferToDPUs(void)
{
    // TODO: copy data to DPUs
}

void run(void)
{
    // TODO: start DPU program
}

void transferFromDPUs(void)
{
    // TODO: copy data back
}

void end(void)
{
    // TODO: free DPUs

    // check result

    // free data
}
