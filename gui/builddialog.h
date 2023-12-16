#pragma once

#include <QDialog>
#include <QMap>
#include "../core/sites/building.h"

class BuildDialog : public QDialog
{
    Q_OBJECT
public:
    explicit BuildDialog();

    Building::Type getChosenType() const;

public slots:
    void onChooseBuilding();

private:
    static const QMap<Building::Type, QString> mBuildDescription;

    Building::Type mChosenType;

    QMap<QPushButton*, Building::Type> mBuildButtons;
};
