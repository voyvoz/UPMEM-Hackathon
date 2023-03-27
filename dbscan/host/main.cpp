#include <dpu>
#include <dpu_management.h>

#include <stdexcept>
#include <sstream> 

#include "../common/db.h"

void load_csv(const std::string &path, struct database *db) {
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
    }
}



int main() {
    // TODO:
    // 1. Load csv into DB.

    // 2. Initialize DPU environment.

    // 3. Push data to MRAM of DPUs.

    // 4. Run DPUs.

    // 5. Obtain results from DPUs.
    return 0;
}