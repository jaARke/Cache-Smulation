#include "Cache.h"

pair<long, long> Cache::IsolateFields(string &address) const {
    long set;
    long tag;
    // Convert the address to binary:
    string binAdd = bitset<32>(stol(address, 0, 16)).to_string();

    // Determine the set number:
    if (sets == 1) {
        set = 0;
    }
    else {
        // Set number is the first log2(sets) characters of the binary address
        set = stol(binAdd.substr(0, log2(sets)), 0, 2);
    }
    // Determine the tag (the first n - log2(bytes/block) characters in the binary address)
    tag = stol(binAdd.substr(0, binAdd.length() - log2(bytesPerBlock)), 0, 2);

    return make_pair(set, tag);
}

Cache::Cache(int sets, int blocks, int bytesPerBlock, int repPolicy)
            : sets(sets), blocks(blocks), bytesPerBlock(bytesPerBlock), repPolicy(repPolicy) {
    // Initialize the structure used to imitate the cache:
    cache = new unordered_map<long, unordered_set<long>*>;
    for (long i = 0; i < sets; i++) {
        cache->insert(make_pair(i, new unordered_set<long>()));
    }

    // Initialize hit and miss functions depending on replacement policy:
    if (repPolicy == 1) {
        // Initialize the structure used to keep track of LRU replacement:
        repVec = new unordered_map<long, vector<long>*>;
        for (int i = 0; i < sets; i++) {
            repVec->insert(make_pair(i, new vector<long>()));
        }
        miss = [this](pair<long, long> newFields) {
            auto set = cache->find(newFields.first);    // Find the set
            auto replacementVector = repVec->find(newFields.first)->second; // Find the replacement vector associated with that set
            if (set->second->size() >= this->blocks) {    // The set is full
                auto oldTag = replacementVector->at(0);
                set->second->extract(oldTag);   // Take out the old tag
                set->second->insert(newFields.second);  // Put in the new
                replacementVector->erase(replacementVector->begin());   // Remove the old tag from the replacement vector
            }
            else {  // The set is not full
                set->second->insert(newFields.second);
            }
            // Place the new tag into the replacement vector:
            replacementVector->push_back(newFields.second);
        };
        hit = [this](pair<long, long> newFields) {
            // Find the tag in the replacementVector, erase it, and put it at the back:
            auto replacementVector = repVec->find(newFields.first)->second;
            auto iter = replacementVector->begin();
            while (*iter != newFields.second) {
                iter++;
            }
            replacementVector->erase(iter);
            replacementVector->push_back(newFields.second);
        };
    }
    else {
        // Initialize the structure used to keep track of FIFO replacement:
        repQ = new unordered_map<long, queue<long>*>;
        for (int i = 0; i < sets; i++) {
            repQ->insert(make_pair(i, new queue<long>()));
        }
        miss = [this](pair<long, long> newFields) {
            auto set = cache->find(newFields.first);    // Find the set
            auto replacementQueue = repQ->find(newFields.first)->second;    // Find the associated replacement queue
            if (set->second->size() >= this->blocks) {  // The set is full
                auto oldTag = replacementQueue->front();
                set->second->extract(oldTag);   // Take out the tag at the front of the replacement queue
                set->second->insert(newFields.second);  // Put in the new tag
                replacementQueue->pop();    // Remove the old tag from the replacement queue
            }
            else {  // The set is not full
                set->second->insert(newFields.second);
            }
            // Place the new tag into the replacement queue:
            replacementQueue->push(newFields.second);
        };
        // Hit doesn't need to do anything in this case
    }
}

Cache::~Cache() {
    auto iter1 = cache->begin();
    while (iter1 != cache->end()) {
        delete iter1->second;
        iter1++;
    }
    if (repPolicy == 1) {   // Deconstruct the repVec
        auto iter2 = repVec->begin();
        while (iter2 != repVec->end()) {
            delete iter2->second;
            iter2++;
        }
        delete repVec;
    }
    else {  // Deconstruct the repQ
        auto iter2 = repQ->begin();
        while(iter2 != repQ->end()) {
            delete iter2->second;
            iter2++;
        }
        delete repQ;
    }
    delete cache;
}

void Cache::Simulate(string& traceDirectory) {
    int hits = 0;
    int misses = 0;

    // Open the trace file and read line-by-line:
    ifstream reader(traceDirectory);
    if (reader.is_open()) {
        string line;
        while (getline(reader, line)) {
            line = line.substr(4, 8);   // Isolate the hex memory address
            auto fields = IsolateFields(line);  // Split the memory address into fields (set no., tag no.)
            auto iter = cache->find(fields.first);  // Find the set the memory address belongs in
            if (iter != cache->end()) {
                if (iter->second->contains(fields.second)) {    // The set contains the address' residing block
                    hits++;
                    if (repPolicy != 2){
                        hit(fields);
                    }
                }
                else {  // The set does not contain the address' residing block
                    misses++;
                    miss(fields);
                }
            }
            else {  // The set number should always be found in the cache if initialization is done correctly
                throw invalid_argument("Sanity Check: This error should never be thrown.");
            }
        }

        // Calculate the result and print it:
        double result = (double) hits / (misses + hits);
        cout << "Hit Rate: " << result << endl;
    }
    else {  // The trace file could not be opened
        throw filesystem::filesystem_error("Cannot find specified trace file!", io_errc::stream);
    }
}
