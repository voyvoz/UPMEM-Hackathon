#include <stdint.h>

#define CHUNK_SIZE 65536
#define RECORDS 817

struct student_record {
    uint64_t id;    
    char firstname[32];
    char lastname[32];
    uint32_t age;  
} S_Record;

struct student_chunk {
    struct student_record records[RECORDS];
    struct student_chunk *next;
    uint64_t count;
} S_Chunk;

struct database {
    char name[32];
    struct student_chunk *data;
};