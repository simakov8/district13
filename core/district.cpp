#include <stdexcept>
#include "district.h"
#include "sites/site.h"
#include "sites/publicbuilding.h"

using namespace std;

District::District(size_t districtSize, float populatedPart)
    : mSize(districtSize),
      mPopulatedPart(populatedPart)
{
    mMap = DistrictMap(mSize, SiteRow(mSize, nullptr));

    for (size_t i = 0; i < mSize; ++i)
        for (size_t j = 0; j < mSize; ++j)
            mMap.at(i).at(j) = new Site(i, j);

    generateDistrictMap();
}

District::~District()
{
    for (SiteRow& row : mMap)
        for (Site* site : row)
            delete site;

    mMap.clear();
}

size_t District::getSize() const
{
    return mSize;
}

void District::setAdministration(Administration* admin)
{
    mAdmin = admin;
}

Administration* District::getAdministration() const
{
    return mAdmin;
}

Site* District::getSiteAt(size_t x, size_t y) const
{
    return mMap.at(x).at(y);
}

Site* District::getSiteAt(Site::Address address) const
{
    return getSiteAt(address.first, address.second);
}

Building* District::getBuildingAt(size_t x, size_t y) const
{
    return getSiteAt(x, y)->getBuilding();
}

Building* District::getBuildingAt(Site::Address address) const
{
    return getSiteAt(address)->getBuilding();
}

District::SiteRow District::getAdjacentSites(Site* site, int distance) const
{
    SiteRow sites;
    const Site::Address addr = site->getAddress();

    for (int i = addr.first - distance; i <= addr.first + distance; ++i)
    {
        for (int j = addr.second - distance; j <= addr.second + distance; ++j)
        {
            if (i == addr.first && j == addr.second)
                continue;

            try
            {
                sites.push_back(getSiteAt(i, j));
            }
            catch (std::out_of_range o)
            {
                continue;
            }
        }
    }
    return sites;
}

void District::nextTurn()
{
    updateBuildingsInfluenceAreas();

    for (size_t i = 0; i < getSize(); ++i)
        for (size_t j = 0; j < getSize(); ++j)
            getSiteAt(i, j)->nextTurn();
}

void District::updateBuildingsInfluenceAreas()
{
    for (size_t i = 0; i < getSize(); ++i)
        for (size_t j = 0; j < getSize(); ++j)
        {
            Building* building = getBuildingAt(i, j);

            if (!building || !building->affectsNeighbours())
                continue;

            SiteRow adjacentSites = getAdjacentSites(getSiteAt(i, j), building->getInfluenceArea());
            for (Site* site : adjacentSites)
                if (site->getBuilding())
                    site->getBuilding()->addNeighbour(building);
        }
}

void District::generateDistrictMap()
{
    if (mPopulatedPart < 0)
        mPopulatedPart = 0;
    if (mPopulatedPart > 1)
        mPopulatedPart = 1;

    SiteRow tempVector;
    for (const SiteRow& siteRow : mMap)
        tempVector.insert(tempVector.end(), siteRow.begin(), siteRow.end());

    size_t initialSize = tempVector.size();
    uint32_t populatedNeeded = initialSize * mPopulatedPart;

    while ((initialSize - tempVector.size()) < populatedNeeded)
    {
        size_t index = rand() % tempVector.size();
        tempVector[index]->constructBuilding(Building::Type::HOUSE);
        tempVector.erase(tempVector.begin() + index);
    }
}

