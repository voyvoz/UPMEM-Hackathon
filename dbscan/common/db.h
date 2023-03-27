#include <stdint.h>

#define CHUNK_SIZE 65536

struct student_record {
    uint64_t id;    
    char firstname[32];
    char lastname[32];
    uint32_t age;  
} S_Record;

struct student_chunk {
    struct student_record records[817];
    struct student_chunk *next;
    uint64_t count;
} S_Chunk;

void add_record(student_chunk *sc, student_record *sr) {
    sc->records[sc->count++] = *sr;
}

struct student_chunk *alloc_chunk() {
    return new student_chunk;
}

struct database {
    char name[32];
    struct S_Chunk *data;
};