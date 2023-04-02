#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <dpu.h>
#include <dpu_log.h>
#include "common.h"

// TODO: declare DPU set
float* vec;
float* mat;
float* res;
float* res_cpu;

void init(void)
{
    // alloc data
    vec = (float*)malloc(SIZE_VEC * sizeof(float));
    mat = (float*)malloc(SIZE_MAT * sizeof(float));
    res = (float*)malloc(SIZE_VEC * sizeof(float));
    res_cpu = (float*)malloc(SIZE_VEC * sizeof(float));

    // init data
    srand(time(NULL));
    float a = 50.0f;
    for(size_t i = 0; i < SIZE_VEC; i++)
    {
        vec[i] = ((float)rand() / (float)(RAND_MAX)) * a;
        for(size_t j = 0; j < SIZE_VEC; j++)
        {
            mat[i * DIM + j] = ((float)rand() / (float)(RAND_MAX)) * a;
        }
    }
    memset(res, 0, SIZE_VEC * sizeof(float));
    memset(res_cpu, 0, SIZE_VEC * sizeof(float));

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
    for(size_t i = 0; i < DIM; i++)
    {
        for(size_t j = 0; j < DIM; j++)
        {
            res_cpu[j] += mat[j * DIM + i] * vec[i];
        }
    }
    for(size_t i = 0; i < DIM; i++)
    {
        if(fabs(res[i] - res_cpu[i]) > 0.001f)
        {
            printf("Error at vec[%ld]: %f != %f\n", i, res[i], res_cpu[i]);
        }
    }

    // free data
    free(vec);
    free(mat);
    free(res);
    free(res_cpu);
}
