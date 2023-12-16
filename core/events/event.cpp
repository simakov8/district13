#include "event.h"
#include "../sites/site.h"

/////////////////////////////////////////////////////////////////
/// Event
/////////////////////////////////////////////////////////////////

Event::Event(Type type, bool calledByUser)
    : mType(type), mUser(calledByUser)
{

}

Event::Type Event::getType() const
{
    return mType;
}

bool Event::isCalledByUser() const
{
    return mUser;
}

/////////////////////////////////////////////////////////////////
/// ConstructionEvent
/////////////////////////////////////////////////////////////////

ConstructionEvent::ConstructionEvent(Site* site, Building::Type newBuilding)
    : Event(Event::Type::Construction, true),
      mSite(site),
      mBuildingType(newBuilding)
{
    if (mSite)
        mSite->mPendingConstruction = true;
}

void ConstructionEvent::execute()
{
    mSite->constructBuilding(mBuildingType);
}

bool ConstructionEvent::canBeExecuted() const
{
    return mSite && !mSite->isOccupied();
}

Site* ConstructionEvent::getSite() const
{
    return mSite;
}

Building::Type ConstructionEvent::getBuildingType() const
{
    return mBuildingType;
}

/////////////////////////////////////////////////////////////////
/// RepairingEvent
/////////////////////////////////////////////////////////////////

RepairingEvent::RepairingEvent(Building *building, double repairRatio)
    : Event(Event::Type::Repairing, true),
      mBuilding(building),
      mRepairRatio(repairRatio)
{
    if (mBuilding)
        mBuilding->mPendingRepairing = true;
}

bool RepairingEvent::canBeExecuted() const
{
    return mBuilding;
}

Building* RepairingEvent::getBuilding() const
{
    return mBuilding;
}

void RepairingEvent::execute()
{
    HappinessFactor& condition = mBuilding->mCondition;
    condition.setValue(condition.getValue() + condition.getMaxValue() * mRepairRatio);

    mBuilding->mPendingRepairing = false;
}

/////////////////////////////////////////////////////////////////
/// CleaningEvent
/////////////////////////////////////////////////////////////////

CleaningEvent::CleaningEvent(Site* site, double cleanRatio)
    : Event(Event::Type::Cleaning, true),
      mSite(site),
      mCleanRatio(cleanRatio)
{
    if (mSite)
        mSite->mPendingCleaning = true;
}

bool CleaningEvent::canBeExecuted() const
{
    return mSite;
}

Site* CleaningEvent::getSite() const
{
    return mSite;
}

void CleaningEvent::execute()
{
    HappinessFactor& cleanliness = mSite->mCleanliness;
    cleanliness.setValue(cleanliness.getValue() + cleanliness.getMaxValue() * mCleanRatio);

    mSite->mPendingCleaning = true;
}
