#pragma once

#include "sites/site.h"
#include "events/event.h"

#include <QList>

class District;
class ConstructionEvent;
class RepairingEvent;
class CleaningEvent;

class Administration
{
public:
    enum class CheckResult
    {
        SUCCESS,
        NOT_ENOUGH_MONEY,
        MAX_VALUE,
        ALREADY_STARTED
    };

    explicit Administration(District* district, int startMoney, int maxIncome);
    ~Administration();

    District* getDistrict() const { return mDistrict; }

    int calcAverageHappiness() const;
    int calcIncome() const;

    int getCurrentMoney() const { return mMoney; }
    void changeMoney(int amount);

    void nextTurn();

    QList<Event::Type> getAvailableActions(const Site* targetSite) const;

    CheckResult canConstructBuilding(Site::Address address, Building::Type type) const;
    ConstructionEvent* constructBuilding(Site::Address address, Building::Type type);

    CheckResult canRepairBuilding(Site::Address address) const;
    RepairingEvent* repairBuilding(Site::Address address);

    CheckResult canCleanSite(Site::Address address) const;
    CleaningEvent* cleanSite(Site::Address address);

private:
    District* mDistrict;

    int mMoney;
    const int mMaxIncome;
};
