#pragma once

#include <list>
#include <string>

#include "../sites/building.h"

class Site;

class Event
{
public:
    friend class Game;

    enum class Type {
        Undefined,
        Construction,
        Repairing,
        Cleaning
    };

    Event(Type type, bool calledByUser = false);

    Type getType() const;
    bool isCalledByUser() const;

    virtual bool canBeExecuted() const = 0;

private:
    virtual void execute() = 0;

    const Type mType;
    const bool mUser;
};

class ConstructionEvent : public Event
{
public:
    ConstructionEvent(Site* site, Building::Type newBuilding);

    virtual bool canBeExecuted() const;

    Site* getSite() const;
    Building::Type getBuildingType() const;

private:
    virtual void execute();

    Site* mSite;
    Building::Type mBuildingType;
};

class RepairingEvent : public Event
{
public:
    RepairingEvent(Building* building, double repairRatio = 0.5);

    virtual bool canBeExecuted() const;

    Building* getBuilding() const;

private:
    virtual void execute();

    double mRepairRatio;
    Building* mBuilding;
};

class CleaningEvent : public Event
{
public:
    CleaningEvent(Site* site, double cleanRatio = 1.0);

    virtual bool canBeExecuted() const;

    Site* getSite() const;

private:
    virtual void execute();

    double mCleanRatio;
    Site* mSite;
};
