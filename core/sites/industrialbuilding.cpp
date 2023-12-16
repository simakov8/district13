#include "industrialbuilding.h"

IndustrialBuilding::IndustrialBuilding(Site* site)
    : PublicBuilding(site, PublicBuilding::Type::FACTORY)
{
    mFactor = HappinessFactor(1, 1, 0, 0, 10);
    mFactor.setValue(mFactor.getMaxValue());
}

int IndustrialBuilding::calcIncome() const
{
    HappinessFactor condition = getCondition();

    return getBaseIncome() * ((double)condition.getValue() / condition.getMaxValue());
}

