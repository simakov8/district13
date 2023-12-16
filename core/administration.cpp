#include "administration.h"
#include "district.h"
#include "events/event.h"
#include "sites/site.h"
#include "sites/house.h"
#include "sites/industrialbuilding.h"

Administration::Administration(District* district, int startMoney, int maxIncome)
    : mDistrict(district),
      mMoney(startMoney),
      mMaxIncome(maxIncome)
{
    if (mDistrict)
        mDistrict->setAdministration(this);
}

Administration::~Administration()
{
    if (mDistrict)
        delete mDistrict;
}

int Administration::calcAverageHappiness() const
{
    int sumHappiness = 0;
    int totalHouses = 0;
    for (size_t i = 0; i < mDistrict->getSize(); ++i)
    {
        for (size_t j = 0; j < mDistrict->getSize(); ++j)
        {
            House* house = dynamic_cast<House*>(mDistrict->getBuildingAt(i, j));

            if (house)
            {
                sumHappiness += house->getHappiness();
                ++totalHouses;
            }
        }
    }

    return (double)sumHappiness / totalHouses;
}

int Administration::calcIncome() const
{
    const int happiness = calcAverageHappiness();
    const int maxHappiness = House::getMaxHappiness();

    const int baseIncome = (double)happiness / maxHappiness * mMaxIncome;

    int factoryIncome = 0;
    for (size_t i = 0; i < mDistrict->getSize(); ++i)
    {
        for (size_t j = 0; j < mDistrict->getSize(); ++j)
        {
            auto factory = dynamic_cast<Factory*>(mDistrict->getBuildingAt(i, j));
            if (factory)
                factoryIncome += factory->calcIncome();
        }
    }

    return baseIncome + factoryIncome;
}

void Administration::changeMoney(int amount)
{
    mMoney += amount;

    if (mMoney < 0)
        mMoney = 0;
}

void Administration::nextTurn()
{
    changeMoney(calcIncome());
}

QList<Event::Type> Administration::getAvailableActions(const Site* targetSite) const
{
    QList<Event::Type> availableActions;
    if (!targetSite)
        return availableActions;

    availableActions.append(Event::Type::Cleaning);

    if (targetSite->isOccupied())
        availableActions.append(Event::Type::Repairing);
    else
        availableActions.append(Event::Type::Construction);

    return availableActions;
}

Administration::CheckResult Administration::canConstructBuilding(Site::Address address, Building::Type type) const
{
    if (mMoney < Building::getBuildCost(type))
        return CheckResult::NOT_ENOUGH_MONEY;

    Site* site = mDistrict->getSiteAt(address);
    if (site->isPendingConstruction())
        return CheckResult::ALREADY_STARTED;

    return CheckResult::SUCCESS;
}

ConstructionEvent* Administration::constructBuilding(Site::Address address, Building::Type type)
{
    if (canConstructBuilding(address, type) != CheckResult::SUCCESS)
        return nullptr;

    Site* site = mDistrict->getSiteAt(address);
    auto event = new ConstructionEvent(site, type);

    changeMoney(-Building::getBuildCost(type));

    return event;
}

Administration::CheckResult Administration::canRepairBuilding(Site::Address address) const
{
    Building* building = mDistrict->getBuildingAt(address);
    if (mMoney < building->calcRepairCost())
        return CheckResult::NOT_ENOUGH_MONEY;

    if (building->isPendingRepairing())
        return CheckResult::ALREADY_STARTED;
    if (building->getCondition().getValue() == building->getCondition().getMaxValue())
        return CheckResult::MAX_VALUE;

    return CheckResult::SUCCESS;
}

RepairingEvent* Administration::repairBuilding(Site::Address address)
{
    if (canRepairBuilding(address) != CheckResult::SUCCESS)
        return nullptr;

    Building* building = mDistrict->getBuildingAt(address);

    auto event = new RepairingEvent(building);
    changeMoney(-building->calcRepairCost());

    return event;
}

Administration::CheckResult Administration::canCleanSite(Site::Address address) const
{
    Site* site = mDistrict->getSiteAt(address);
    if (mMoney < site->calcCleaningCost())
        return CheckResult::NOT_ENOUGH_MONEY;

    if (site->isPendingCleaning())
        return CheckResult::ALREADY_STARTED;
    if (site->getCleanliness().getValue() == site->getCleanliness().getMaxValue())
        return CheckResult::MAX_VALUE;

    return CheckResult::SUCCESS;
}

CleaningEvent* Administration::cleanSite(Site::Address address)
{
    if (canCleanSite(address) != CheckResult::SUCCESS)
        return nullptr;

    Site* site = mDistrict->getSiteAt(address);

    auto event = new CleaningEvent(site);
    changeMoney(-site->calcCleaningCost());

    return event;
}
