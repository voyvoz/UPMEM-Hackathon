#include <dpu>
#include <dpu_management.h>

#include <stdexcept>
#include <sstream>
#include <chrono> 

#include "../common/db.h"

typedef std::vector<struct student_records> result_set;

std::size_t load_csv(const std::string &path, struct database *db) {
    std::size_t count = 0;
    std::ifstream file(path);

    if(!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    std::string line, data;

    while(std::getline(file, line)) {
        std::stringstream ss(line);

        int colType = 0;
        
        student_record sr;

        while(ss >> data) {
            switch(colType) {
                case 0: {
                    sr.id = std::stoi(data);
                    break;
                }
                case 1: {
                    sr.firstname = data;
                }
                case 2: {
                    sr.lastname = data;
                }
                case 3: {
                    sr.age = std::stoi(data);
                }
            }
            colType++;
        }
        db_add_record(db, sr);
        count++;
    }
    return count;
}

void transfer_to_dpu(struct database *db, dpu_set_t &set) {

}

void run_dpu(dpu_set_t &set) {

}

void transfer_from_dpu(dpu_set_t &set, result_set &rs) {

}

void run_cpu(result_set &rs) {

}

int main() {
    auto db = new_database();

    dpu_set_t set, dpu;

    // TODO:
    // 1. Load csv into DB.
    load_csv("student_data.csv", db);

    // 2. Initialize DPU environment.

    // 3. Push data to MRAM of DPUs.
    auto start_transfer = std::chrono::high_resolution_clock::now();
    transfer_to_dpu(db, set);
    auto end_transfer = std::chrono::high_resolution_clock::now();

    // 4. Run DPUs.
    auto start_launch = std::chrono::high_resolution_clock::now();
    run_dpu(set);
    auto end_launch = std::chrono::high_resolution_clock::now();

    // 5. Obtain results from DPUs.
    result_set rs;
    auto start_transfer2 = std::chrono::high_resolution_clock::now();
    transfer_from_dpu(set, rs);
    auto end_transfer2 = std::chrono::high_resolution_clock::now();

    result_set rs2;
    auto start_cpu = std::chrono::high_resolution_clock::now();
    run_cpu(rs);
    auto end_cpu = std::chrono::high_resolution_clock::now();

    std::cout << "Transfer to:      " << std::chrono::duration_cast<std::chrono::milliseconds>(end_transfer-start_transfer).count() << " ms\n"
              << "DPU time:         " << std::chrono::duration_cast<std::chrono::milliseconds>(end_launch-start_launch).count() << " ms\n"
              << "Transfer from:    " << std::chrono::duration_cast<std::chrono::milliseconds>(end_transfer2-start_transfer2).count() << " ms\n" 
              << "Baseline:         " << std::chrono::duration_cast<std::chrono::milliseconds>(end_cpu-start_cpu).count() << " ms\n"      

    return 0;
}