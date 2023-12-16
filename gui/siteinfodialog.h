# pragma once

#include <QFrame>

class Site;
class QLabel;
class QPushButton;
class QGroupBox;

class SiteInfoDialog : public QFrame
{
    Q_OBJECT
public:
    SiteInfoDialog(QWidget* parent = 0);

    Site* getSite() const;

public slots:
    void showSiteInfo(Site* site);

private:
    void fillPopulationInfo();
    void fillBuildingInfo();
    void fillSiteInfo();

    void setupLayout();

    Site* mSite;

    QLabel* mHappinessLabel;

    QGroupBox* mPopulationInfo;
    QGroupBox* mBuildingInfo;
    QGroupBox* mSiteInfo;
};
