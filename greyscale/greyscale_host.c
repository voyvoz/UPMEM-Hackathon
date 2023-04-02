#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <dpu.h>
#include <dpu_log.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

// TODO: declare DPU set
int width, height, n;
unsigned char *img_in;
unsigned char *img_out;
unsigned char *img_out_cpu;

void init(void)
{
    // read image
    img_in = stbi_load("img.jpg", &width, &height, &n, 0);
    if(img_in == NULL) {
        printf("Could not load image\n");
        exit(-1);
    }

    // alloc image output
    img_out = malloc(width * height * 3 * sizeof(unsigned char));
    img_out_cpu = malloc(width * height * 3 * sizeof(unsigned char));

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

    // save DPU image
    stbi_write_jpg("img_grey_dpu.jpg", width, height, 3, img_out, 100);

    // check result
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            unsigned char greyScale = img_in[i * width * 3 + j * 3 + 0] * 0.22f + img_in[i * width * 3 + j * 3 + 1] * 0.11 + img_in[i * width * 3 + j * 3 + 2] * 0.58;
            img_out_cpu[i * width * 3 + j * 3 + 0] = greyScale; // B
            img_out_cpu[i * width * 3 + j * 3 + 1] = greyScale; // G
            img_out_cpu[i * width * 3 + j * 3 + 2] = greyScale; // R
        }
    }
    for(size_t i = 0; i < (width * height); i++)
    {
        if(img_out[i] != img_out_cpu[i])
        {
            printf("Error at pixel[%ld]: %d != %d\n", i, img_out[i], img_out_cpu[i]);
        }
    }

    // save CPU image
    stbi_write_jpg("img_grey_cpu.jpg", width, height, 3, img_out_cpu, 100);

    // free data
    free(img_in);
    free(img_out);
    free(img_out_cpu);
}
