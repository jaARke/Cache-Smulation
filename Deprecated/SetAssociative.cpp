#include "SetAssociative.h"

pair<long, long> SetAssociative::IsolateFields(string &address) const {
    string binAdd;
    long set;
    long tag;

    binAdd = bitset<32>(stol(address, 0, 16)).to_string();
    set = stol(binAdd.substr(0, log2(sets)), 0, 2);
    tag = stol(binAdd.substr(0, binAdd.length() - log2(bytesPerBlock)), 0, 2);

    return make_pair(set, tag);
}

SetAssociative::SetAssociative(int sets, int blocks, int bytesPerBlock, int repPolicy) : Cache(sets, blocks, bytesPerBlock, repPolicy) {}
