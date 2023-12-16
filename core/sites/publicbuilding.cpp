#include "publicbuilding.h"

PublicBuilding::PublicBuilding(Site* site, Building::Type type, int influenceArea)
    : Building(site, type, influenceArea)
{
}

HappinessFactor PublicBuilding::getFactor() const
{
    return mFactor;
}

void PublicBuilding::nextTurn()
{
    Building::nextTurn();
}



Shop::Shop(Site* site)
    : PublicBuilding(site, Building::Type::SHOP, 2)
{
    mFactor = HappinessFactor(1, 1, 0, 0, 15);
    mFactor.setValue(mFactor.getMaxValue());
}

School::School(Site* site)
    : PublicBuilding(site, Building::Type::SCHOOL, 3)
{
    mFactor = HappinessFactor(1, 1, 0, 0, 10);
    mFactor.setValue(mFactor.getMaxValue());
}

Park::Park(Site* site)
    : PublicBuilding(site, Building::Type::PARK, 3)
{
    mFactor = HappinessFactor(1, 1, 0, 0, 5);
    mFactor.setValue(mFactor.getMaxValue());
}

Factory::Factory(Site* site)
    : PublicBuilding(site, Building::Type::FACTORY, 1),
      mIsNearShop(false)
{
    mFactor = HappinessFactor(0, 1, 0, 1, 15);
    mFactor.setValue(mFactor.getMinValue());
}

void Factory::addNeighbour(Building* neighbour)
{
    PublicBuilding::addNeighbour(neighbour);

    if (neighbour->getType() == Building::Type::SHOP)
        mIsNearShop = true;
}

int Factory::calcIncome() const
{
    HappinessFactor condition = getCondition();

    int income = getBaseIncome() * ((double)condition.getValue() / condition.getMaxValue());
    if (mIsNearShop)
        income = (double)income * 1.5;

    return income;
}

