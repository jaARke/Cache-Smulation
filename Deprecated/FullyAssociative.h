#pragma once
#include "../Cache.h"


class FullyAssociative : public Cache {
private:
    pair<long, long> IsolateFields(string& address) const override;

public:
    FullyAssociative(int sets, int blocks, int bytesPerBlock, int repPolicy);
};

