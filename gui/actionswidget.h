#pragma once

#include <QFrame>

class Site;
class Administration;
class Event;

class QGroupBox;
class QPushButton;

class ActionsWidget : public QFrame
{
    Q_OBJECT
public:
    ActionsWidget(Administration* admin, QWidget* parent = 0);

    void setAdministration(Administration* admin);

public slots:
    void chooseActions(Site* targetSite);

signals:
    void initEvent(Event* event);

private slots:
    void onShowBuildDialog();
    void onRepairBuilding();
    void onCleanSite();

private:
    QPushButton* addAction(QIcon icon, QString title);

    Site* mTargetSite;
    Administration* mAdmin;

    QGroupBox* mActionsGroupbox;

    QList<QPushButton*> mActions;
};
