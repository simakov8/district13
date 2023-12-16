#pragma once

#include "building.h"
#include <vector>
#include <list>

class PublicBuilding;

class House : public Building
{
public:
    House(Site* site);

    static inline int getMinHappiness() { return 0; }
    static inline int getMaxHappiness() { return 100; }
    static inline int getBaseHappiness() { return 50; }

    int getHappiness() const;

    virtual bool isHouse() const { return true; }
    virtual bool isPublic() const { return false; }
    virtual bool affectsNeighbours() const { return false; }

    std::vector<HappinessFactor> getFactors() const;
    void calculateHappiness();

    virtual void addNeighbour(Building* neighbour);
    virtual void removeNeighbour(Building* neighbour);

    virtual void nextTurn();

private:
    std::list<PublicBuilding*> mNearestBuildings;

    int mBaseHappiness;
    int mHappiness;
};
