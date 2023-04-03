#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <sstream>
#include <string.h>
#include <fstream>
#include <iostream>
#include <vector>

#include "db.h"

#include <sys/time.h>
#include <sys/resource.h>

uint64_t get_time()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return 1000 * t.tv_sec + (t.tv_usec / 1000);
}

extern "C"
{
void init(void);

void transferToDPUs(void);
void run(void);
void transferFromDPUs(void);

void end(void);
}

typedef std::vector<struct student_records*> result_set;

void chunk_add_record(student_chunk *sc, student_record *sr);
struct student_chunk *alloc_chunk();
void db_add_record(struct database* db, struct student_record* sr);
struct database* new_database();

struct database* db;

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

int main(void)
{
    db = new_database();
    size_t count = load_csv("./student_data.csv", db);
    std::cout << "Loaded " << count << " items into DB" << std::endl;

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
        close(fd);
    }

    return 0;
}
