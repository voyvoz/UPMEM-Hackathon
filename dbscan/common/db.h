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

void chunk_add_record(student_chunk *sc, student_record *sr) {
    sc->records[sc->count++] = *sr;
}

struct student_chunk *alloc_chunk() {
    return new student_chunk;
}

struct database {
    char name[32];
    struct S_Chunk *data;
};

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