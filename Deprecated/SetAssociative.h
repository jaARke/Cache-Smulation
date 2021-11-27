#pragma once
#include "../Cache.h"

class SetAssociative : public Cache {
private:
    pair<long, long> IsolateFields(string& address) const override;

public:
    SetAssociative(int sets, int blocks, int bytesPerBlock, int repPolicy);
};