#pragma once

#include "building.h"

class PublicBuilding : public Building
{
public:
    PublicBuilding(Site* site, Building::Type type, int influenceArea);

    virtual bool isHouse() const { return false; }
    virtual bool isPublic() const { return true; }
    virtual bool affectsNeighbours() const { return true; }

    HappinessFactor getFactor() const;

    virtual void nextTurn();

protected:
    HappinessFactor mFactor;
};



class Shop : public PublicBuilding
{
public:
    Shop(Site* site);
};



class School : public PublicBuilding
{
public:
    School(Site* site);
};



class Park : public PublicBuilding
{
public:
    Park(Site* site);
};



class Factory : public PublicBuilding
{
public:
    static int getBaseIncome() { return 5; }

    Factory(Site* site);

    int calcIncome() const;

    virtual void addNeighbour(Building *neighbour);

private:
    bool mIsNearShop;
};
