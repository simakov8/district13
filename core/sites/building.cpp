#include "building.h"
#include "site.h"

const std::map<Building::Type, int> Building::buildCosts = {
    { Building::Type::NONE, 0},
    { Building::Type::HOUSE, 10},
    { Building::Type::SHOP, 15},
    { Building::Type::SCHOOL, 20},
    { Building::Type::PARK, 30},
    { Building::Type::FACTORY, 25}
};

int Building::getBuildCost(Building::Type type)
{
    return buildCosts.find(type)->second;
}

int Building::calcRepairCost(double repairRatio) const
{
    return Building::getBuildCost(mType) * 0.75 * repairRatio;
}

Building::Building(Site* site, Building::Type type, int influenceArea)
    : mSite(site),
      mType(type),
      mInfluenceArea(influenceArea),
      mPendingRepairing(false),
      mCondition(0, 100, 3, 75, 30)
{
}

Site* Building::getSite() const
{
    return mSite;
}

Building::Type Building::getType() const
{
    return mType;
}

Building::TypeList Building::getAllTypes()
{
    TypeList types = {Type::NONE,
                      Type::HOUSE,
                      Type::SHOP,
                      Type::SCHOOL,
                      Type::PARK,
                      Type::FACTORY};
    return types;
}

int Building::getInfluenceArea() const
{
    return mInfluenceArea;
}

HappinessFactor Building::getCondition() const
{
    return mCondition;
}

bool Building::isPendingRepairing() const
{
    return mPendingRepairing;
}

void Building::addNeighbour(Building* neighbour)
{
    if (neighbour->getType() == Type::PARK)
        mSite->getCleanliness().setStep(mSite->getCleanliness().getStep() * 0.5);
}

void Building::removeNeighbour(Building* neighbour)
{
    if (neighbour->getType() == Type::PARK)
        mCondition.setStep(mCondition.getStep() * 2);
}

void Building::nextTurn()
{
    --mCondition;
}

