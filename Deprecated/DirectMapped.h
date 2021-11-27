#pragma once
#include "../Cache.h"

class DirectMapped : public Cache {
private:
    pair<long, long> IsolateFields(string& address) const override;

public:
    DirectMapped(int sets, int blocks, int bytesPerBlock, int repPolicy);
};
