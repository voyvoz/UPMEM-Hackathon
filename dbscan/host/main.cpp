#include <dpu>
#include <dpu_management.h>

#include <stdexcept>
#include <sstream>
#include <chrono> 
#include <string.h>
#include <fstream>
#include <iostream>

#include "../common/db.h"

typedef std::vector<struct student_records*> result_set;

void chunk_add_record(student_chunk *sc, student_record *sr);
struct student_chunk *alloc_chunk();
void db_add_record(struct database* db, struct student_record* sr);
struct database* new_database();

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
        
        student_record *sr = new student_record();

        while(ss.good()) {
            std::getline(ss, data, ',');
            if(colType == 0) sr->id = std::stoi(data);
            else if(colType == 1) strcpy(sr->firstname, data.c_str());
            else if(colType == 2) strcpy(sr->lastname, data.c_str());
            else if(colType == 3) sr->age = std::stoi(data);
            if(ss.peek() == ',') ss.ignore();
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
    auto count = load_csv("./student_data.csv", db);
    std::cout << "Loaded " << count << " items into DB" << std::endl;

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

    std::cout << "Transfer to:      " << std::chrono::duration_cast<std::chrono::milliseconds>(end_transfer-start_transfer).count() << " ms" << std::endl;
    std::cout << "DPU time:         " << std::chrono::duration_cast<std::chrono::milliseconds>(end_launch-start_launch).count() << " ms" << std::endl;
    std::cout << "Transfer from:    " << std::chrono::duration_cast<std::chrono::milliseconds>(end_transfer2-start_transfer2).count() << " ms" << std::endl;
    std::cout << "Baseline:         " << std::chrono::duration_cast<std::chrono::milliseconds>(end_cpu-start_cpu).count() << " ms" << std::endl;      

    return 0;
}

void chunk_add_record(student_chunk *sc, student_record *sr) {
    sc->records[sc->count++] = *sr;
}

struct student_chunk *alloc_chunk() {
    return new student_chunk;
}

struct database* new_database() {
    auto db = new database();
    db->data = new student_chunk();
    return db;
}

void db_add_record(struct database* db, struct student_record* sr) {
    struct student_chunk* prev = nullptr;
    auto cur = db->data;
    
    while(cur != nullptr) {
        if(cur->count == RECORDS) {
            prev = cur;
            cur = cur->next;
        } else {
            chunk_add_record(cur, sr);
            return;
        }
    }

    prev->next = alloc_chunk();
    chunk_add_record(prev->next, sr);
}