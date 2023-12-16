#include "eventlogger.h"
#include "minimap.h"
#include "../core/sites/site.h"

EventLogger::EventLogger()
{

}

EventLogger::EventSummary EventLogger::getEventLog(const Event& event, bool preview) const
{
    switch (event.getType())
    {
    case Event::Type::Undefined:
        return EventSummary();

    case Event::Type::Construction:
        return logConstructionEvent(dynamic_cast<const ConstructionEvent&>(event), preview);

    case Event::Type::Repairing:
        return logRepairingEvent(dynamic_cast<const RepairingEvent&>(event), preview);

    case Event::Type::Cleaning:
        return logCleaningEvent(dynamic_cast<const CleaningEvent&>(event), preview);

    default:
        return EventSummary();
    }
}

QString EventLogger::getAddressString(Site::Address address) const
{
    return QString::number(address.first) + "-" + QString::number(address.second);
}

EventLogger::EventSummary EventLogger::logConstructionEvent(const ConstructionEvent& event,
                                                            bool preview) const
{
    EventSummary summary;

    Site* site = event.getSite();
    Site::Address address = site->getAddress();

    QString title = DistrictMinimap::getBuildingTitle(event.getBuildingType());

    if (preview)
        summary.append(tr("На участке ") +
                       getAddressString(address) +
                       tr(" началось строительство ") +
                       title);
    else
        summary.append(
                    tr("На участке ") +
                    getAddressString(address) +
                    tr(" завершилось строительство ") +
                    title);
    return summary;
}

EventLogger::EventSummary EventLogger::logRepairingEvent(const RepairingEvent& event,
                                                         bool preview) const
{
    EventSummary summary;

    Building* building = event.getBuilding();
    Site::Address address = building->getSite()->getAddress();
    QString title = DistrictMinimap::getBuildingTitle(building->getType());

    if (preview)
        summary.append(tr("На участке ") +
                       getAddressString(address) +
                       tr(" начался ремонт ") +
                       title +
                       tr(" стоимостью ") +
                       QString::number(building->calcRepairCost()));
    else
        summary.append(tr("На участке ") +
                       getAddressString(address) +
                       tr(" завершился ремонт ") +
                       title);
    return summary;
}

EventLogger::EventSummary EventLogger::logCleaningEvent(const CleaningEvent& event,
                                                        bool preview) const
{
    EventSummary summary;

    Site* site = event.getSite();
    Site::Address address = site->getAddress();

    if (preview)
        summary.append(tr("На участке ") +
                       getAddressString(address) +
                       tr(" началась уборка территории. Стоимость работ: ") +
                       QString::number(site->calcCleaningCost()));
    else
        summary.append(tr("На участке ") +
                       getAddressString(address) +
                       tr(" завершилась уборка."));

    return summary;
}

