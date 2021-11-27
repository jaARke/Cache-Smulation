#pragma once
#include <string>
#include <queue>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <fstream>
#include <iostream>

using namespace std;

class Cache {

private:
    const int sets;
    const int bytesPerBlock;
    const int blocks;
    const int repPolicy;

    function<void(pair<long, long>)> miss;
    function<void(pair<long, long>)> hit;

    unordered_map<long, queue<long>*>* repQ;
    unordered_map<long, vector<long>*>* repVec;

    unordered_map<long, unordered_set<long>*>* cache;

    pair<long, long> IsolateFields(string& address) const;

public:
    Cache(int sets, int blocks, int bytesPerBlock, int repPolicy);
    ~Cache();
    void Simulate(string& traceDirectory);
};


