#include "publicbuilding.h"

class IndustrialBuilding : public PublicBuilding
{
public:
    static int getBaseIncome() { return 5; }

    IndustrialBuilding(Site* site);

    int calcIncome() const;
};
