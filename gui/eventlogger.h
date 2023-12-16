#pragma once

#include "../core/events/event.h"
#include "../core/sites/site.h"

#include <QObject>
#include <QStringList>

// TODO: do not inherit from QObject
class EventLogger : public QObject
{
    Q_OBJECT
public:
    using EventSummary = QStringList;

    EventLogger();

    EventSummary getEventLog(const Event& event, bool preview = false) const;

private:
    QString getAddressString(Site::Address address) const;

    EventSummary logConstructionEvent(const ConstructionEvent& event,
                                      bool preview = false) const;
    EventSummary logRepairingEvent(const RepairingEvent& event,
                                   bool preview = false) const;
    EventSummary logCleaningEvent(const CleaningEvent& event,
                                  bool preview = false) const;
};
