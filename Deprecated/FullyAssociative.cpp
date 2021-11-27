#include "FullyAssociative.h"

pair<long, long> FullyAssociative::IsolateFields(string& address) const {
    string binAdd;
    long set = 0;
    long tag;

    binAdd = bitset<32>(stol(address, 0, 16)).to_string();
    tag = stol(binAdd.substr(0, binAdd.length() - log2(bytesPerBlock)), 0, 2);

    return make_pair(set, tag);
}

FullyAssociative::FullyAssociative(int sets, int blocks, int bytesPerBlock, int repPolicy) : Cache(sets, blocks, bytesPerBlock, repPolicy) {}


