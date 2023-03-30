#include <dpu>
#include <dpu_management.h>

#include <stdexcept>
#include <sstream>
#include <chrono> 
#include <string.h>
#include <fstream>
#include <iostream>
#include <thread>

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

void transfer_to_dpu(dpu_set_t &set) {

}

void run_dpu(dpu_set_t &set) {

}

void transfer_from_dpu(dpu_set_t &set) {

}

void run_cpu(Mat image) {

    uint8_t* pixelPtr = (uint8_t*)image.data;
    int cn = image.channels(); // 3 BGR

    for(int i = 0; i < image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
        {
            auto greyScale = pixelPtr[i*image.cols*cn + j*cn + 0] * 0.22f + pixelPtr[i*image.cols*cn + j*cn + 1] * 0.11 + pixelPtr[i*image.cols*cn + j*cn + 2] * 0.58;
            pixelPtr[i*image.cols*cn + j*cn + 0] = greyScale; // B
            pixelPtr[i*image.cols*cn + j*cn + 1] = greyScale; // G
            pixelPtr[i*image.cols*cn + j*cn + 2] = greyScale; // R
        }
    }    
}

int main(int argc, char** argv) {
    dpu_set_t set, dpu;
    Mat img = imread("img.jpg");
    run_cpu(img);
    imwrite("grey.jpg", img);
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

    auto start_cpu = std::chrono::high_resolution_clock::now();
    run_cpu(img);
    auto end_cpu = std::chrono::high_resolution_clock::now();

    std::cout << "Transfer to:      " << std::chrono::duration_cast<std::chrono::milliseconds>(end_transfer-start_transfer).count() << " ms" << std::endl;
    std::cout << "DPU time:         " << std::chrono::duration_cast<std::chrono::milliseconds>(end_launch-start_launch).count() << " ms" << std::endl;
    std::cout << "Transfer from:    " << std::chrono::duration_cast<std::chrono::milliseconds>(end_transfer2-start_transfer2).count() << " ms" << std::endl;
    std::cout << "Baseline:         " << std::chrono::duration_cast<std::chrono::milliseconds>(end_cpu-start_cpu).count() << " ms" << std::endl;      

    return 0;

}
