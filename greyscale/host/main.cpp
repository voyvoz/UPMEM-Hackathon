#include <dpu>
#include <dpu_management.h>

#include <stdexcept>
#include <sstream>
#include <chrono> 
#include <string.h>
#include <fstream>
#include <iostream>
#include <thread>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


void transfer_to_dpu(dpu_set_t &set) {

}

void run_dpu(dpu_set_t &set) {

}

void transfer_from_dpu(dpu_set_t &set) {

}

void run_cpu(unsigned char* data, int w, int h, int cn) {
    auto pixelPtr = data;

    for(int i = 0; i < h; i++)
    {
        for(int j = 0; j < w; j++)
        {
            auto greyScale = pixelPtr[i*w*cn + j*cn + 0] * 0.22f + pixelPtr[i*w*cn + j*cn + 1] * 0.11 + pixelPtr[i*w*cn + j*cn + 2] * 0.58;
            pixelPtr[i*w*cn + j*cn + 0] = greyScale; // B
            pixelPtr[i*w*cn + j*cn + 1] = greyScale; // G
            pixelPtr[i*w*cn + j*cn + 2] = greyScale; // R
        }
    }    

}

void run_mt(unsigned char* data, int w, int h, int cn) { 

}

int main(int argc, char** argv) {
    dpu_set_t set, dpu;
    
    // TODO:
    // 1. Load csv into DB.

    // 2. Initialize DPU environment.

    // 3. Push data to MRAM of DPUs.
    auto start_transfer = std::chrono::high_resolution_clock::now();
    transfer_to_dpu(set);
    auto end_transfer = std::chrono::high_resolution_clock::now();

    // 4. Run DPUs.
    auto start_launch = std::chrono::high_resolution_clock::now();
    run_dpu(set);
    auto end_launch = std::chrono::high_resolution_clock::now();

    // 5. Obtain results from DPUs.
    auto start_transfer2 = std::chrono::high_resolution_clock::now();
    transfer_from_dpu(set);
    auto end_transfer2 = std::chrono::high_resolution_clock::now();

    int w,h,n;
    unsigned char *data;
    
    data = stbi_load(argv[1], &w, &h, &n, 0);

    if(data == NULL) {
        std::cout << "Could not load image" << std::endl;
        return 1;
    }

    auto start_cpu = std::chrono::high_resolution_clock::now();
    run_cpu(data, w, h, 3);
    auto end_cpu = std::chrono::high_resolution_clock::now();
    stbi_write_png(argv[2], w, h, 3, data, w* 3);


    auto start_mt = std::chrono::high_resolution_clock::now();
    run_mt(data, w, h, 3);
    auto end_mt = std::chrono::high_resolution_clock::now();

    std::cout << "Transfer to:      " << std::chrono::duration_cast<std::chrono::milliseconds>(end_transfer-start_transfer).count() << " ms" << std::endl;
    std::cout << "DPU time:         " << std::chrono::duration_cast<std::chrono::milliseconds>(end_launch-start_launch).count() << " ms" << std::endl;
    std::cout << "Transfer from:    " << std::chrono::duration_cast<std::chrono::milliseconds>(end_transfer2-start_transfer2).count() << " ms" << std::endl;
    std::cout << "Baseline:         " << std::chrono::duration_cast<std::chrono::milliseconds>(end_cpu-start_cpu).count() << " ms" << std::endl;  
    std::cout << "Baseline MT:      " << std::chrono::duration_cast<std::chrono::milliseconds>(end_mt-start_mt).count() << " ms" << std::endl;      

    return 0;

}
