#include <iostream>
#include <cmath>
#include "Cache.h"

using namespace std;

// Driver program:
int main() {
    // Parameter definitions:
    int sets;
    int blocks;
    int bytes;
    int repPolicy;
    string traceName;

    // Parameter collection and checking:
    cout << "Number of sets in the cache: ";
    cin >> sets;
    if (log2(sets) != floor(log2(sets))) {
        throw invalid_argument("Number of sets must be a positive power of 2!");
    }

    cout << "Number of blocks in each set: ";
    cin >> blocks;
    if (log2(blocks) != floor(log2(blocks))) {
        throw invalid_argument("Number of blocks must be a positive power of 2!");
    }

    cout << "Number of bytes per block: ";
    cin >> bytes;
    if (log2(bytes) != floor(log2(bytes)) || bytes < 4) {
        throw invalid_argument("Number of bytes must be a positive power of 2 and be greater than 4!");
    }

    cout << "Replacement policy:" << endl;
    cout << "1 - LRU" << endl;
    cout << "2 - FIFO" << endl;
    cin >> repPolicy;
    if (repPolicy != 1 && repPolicy != 2) {
        throw invalid_argument("Please enter either 1 or 2 for the cache's replacement policy!");
    }

    cout << "Trace file name: (in ./Traces directory, including extension): ";
    cin >> traceName;
    traceName = "Traces/" + traceName;
    cout << endl;

    // Run the simulation:
    Cache cache(sets, blocks, bytes, repPolicy);
    cache.Simulate(traceName);
    
    return 0;
}
