#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

#define CHUNK_SIZE 65536
#define RECORDS 817

struct student_record {
    uint64_t id;    
    char firstname[32];
    char lastname[32];
    uint32_t age;  
};

struct student_chunk {
    struct student_record records[RECORDS];
    struct student_chunk *next;
    uint64_t count;
};

struct database {
    char name[32];
    struct student_chunk *data;
};

#endif // COMMON_H
