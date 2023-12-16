#include "house.h"
#include "site.h"
#include "publicbuilding.h"
#include <algorithm>


House::House(Site* site)
    : Building(site, Building::Type::HOUSE),
      mHappiness(getBaseHappiness())
{

}

int House::getHappiness() const
{
    return mHappiness;
}

std::vector<HappinessFactor> House::getFactors() const
{
    std::vector<HappinessFactor> factors;
    factors.push_back(mSite->getCleanliness());
    factors.push_back(getCondition());

    for (PublicBuilding* building : mNearestBuildings)
        factors.push_back(building->getFactor());

    return factors;
}

void House::calculateHappiness()
{
    std::vector<HappinessFactor> factors = getFactors();

    mHappiness = getBaseHappiness();
    for (auto it = factors.cbegin(); it != factors.cend(); ++it)
        mHappiness += it->getHappinessImpact();

    if (mHappiness < getMinHappiness())
        mHappiness = getMinHappiness();
    if (mHappiness > getMaxHappiness())
        mHappiness = getMaxHappiness();
}

void House::addNeighbour(Building* neighbour)
{
    Building::addNeighbour(neighbour);

    if (neighbour->isPublic())
    {
        auto publicBuilding = dynamic_cast<PublicBuilding*>(neighbour);

        if (std::find(mNearestBuildings.cbegin(), mNearestBuildings.cend(), publicBuilding)
                 == mNearestBuildings.cend())
            mNearestBuildings.push_back(publicBuilding);
    }
}

void House::removeNeighbour(Building* neighbour)
{
    if (neighbour->isPublic())
    {
        auto publicBuilding = dynamic_cast<PublicBuilding*>(neighbour);
        mNearestBuildings.erase(std::find(mNearestBuildings.begin(), mNearestBuildings.end(), publicBuilding));
    }
}

void House::nextTurn()
{
    Building::nextTurn();

    calculateHappiness();
}

