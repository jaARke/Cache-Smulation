#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <sstream>


using namespace std;

class FIRST_ATTEMPT {
private:
    // Member variables:
    int sets;
    int blocks;
    int bytes;
    int blockPositions;
    // Function objects to store replacement functionality:
    function<void(string&)> miss;
    function<void(string&)> hit;
    // Used for FIFO replacement:
    queue<string> repQ;
    // Used for LRU replacement:
    vector<string> repVec;
    // Data structure to simulate a cache:
    unordered_map<int, vector<string>*>* cache;

    // Private functions:
    pair<string, string> IsolateFields(string& address) const;

public:
    // Public functions:
    FIRST_ATTEMPT(int sets, int blocks, int bytes, int replacement);
    ~FIRST_ATTEMPT();
    void Simulate(string& traceDirectory);
};
