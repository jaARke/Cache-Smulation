#include "FIRST_ATTEMPT.h"

using namespace std;

pair<string, string> FIRST_ATTEMPT::IsolateFields(string& address) const {
    string binAdd;
    string e1;
    string e2;
    // Convert to address to binary:
    binAdd = bitset<32>(stoi(address, 0, 16)).to_string();
    // Create the substrings out of the binary address:
    e1 = binAdd.substr(0, log2((double)blocks));
    e2 = binAdd.substr(log2((double)blocks), log2((double)blockPositions));
    return pair<string, string>(e1, e2);
}

FIRST_ATTEMPT::FIRST_ATTEMPT(int sets, int blocks, int bytes, int replacement) {
    // Initialize member variables:
    this->sets = sets;
    this->blocks = blocks;
    this->bytes = bytes;
    this->blockPositions = floor(bytes / 4);


    // Initialize replacement functions:
    if (replacement == 1) {
        miss = [this](string& newAddress) {
            string oldAddress = repVec[0];
            auto oldFields = IsolateFields(oldAddress);
            auto iter = cache->find(stoi(oldFields.first, 0, 2));
            iter->second->at(stoi(oldFields.second, 0, 2)) = newAddress;
            repVec.front().erase();
            repVec.push_back(newAddress);
        };
        hit = [this](string& newAddress) {
            auto iter = repVec.begin();
            while (*iter != newAddress) {
                iter++;
            }
            repVec.erase(iter);
            repVec.push_back(newAddress);
        };
    }
    else {
        miss = [this](string& newAddress) {
            repQ.pop();
            repQ.push(newAddress);
        };
        // Hit doesn't need to do anything in this case.
    }

    // Initialize the HashMap used to represent the cache:
    cache = new unordered_map<int, vector<string>*>;
    for (int i = 0; i < blocks; i++) {
        cache->insert(make_pair(i, new vector<string>(blockPositions, "")));
    }
}

void FIRST_ATTEMPT::Simulate(string& traceDirectory) {
    int hits = 0;
    int misses = 0;
    ifstream reader(traceDirectory);
    if (reader.is_open()) {
        string line;
        while(getline(reader, line)) {
            line = line.substr(4, 8);  // Isolate the memory address
            auto fields = IsolateFields(line);
            auto iter = cache->find(stoi(fields.first, 0, 2));
            if (iter != cache->end() && iter->second->at(stoi(fields.second, 0, 2)) == line) {
                hits++;
                hit(line);
            }
            else {
                misses++;
                miss(line);
            }
        }
        double result = (double) hits / misses;
        cout << "Hit / Miss Ratio: " << result << endl;
    }
    else {
        throw filesystem::filesystem_error("Cannot find specified trace file!", io_errc::stream);
    }
}

FIRST_ATTEMPT::~FIRST_ATTEMPT() {
    for (int i = 0; i < sets; i++) {
        delete cache->at(i);
    }
    delete cache;
}
