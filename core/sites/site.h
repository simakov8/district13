#pragma once

#include <string>
#include <map>

#include "building.h"

class ConstructionEvent;
class CleaningEvent;

class Site
{
    friend class ConstructionEvent;
    friend class CleaningEvent;

public:
    using Address = std::pair<int, int>;

    Site(int posX, int posY);

    Address getAddress() const;

    bool isOccupied() const;
    bool isPendingConstruction() const;
    bool isPendingCleaning() const;
    Building* getBuilding() const;

    bool constructBuilding(Building::Type type);

    void nextTurn();

    HappinessFactor& getCleanliness();
    int calcCleaningCost() const;

private:
    Address mAddress;
    Building* mBuilding;

    bool mPendingConstruction;
    bool mPendingCleaning;

    HappinessFactor mCleanliness;
};
