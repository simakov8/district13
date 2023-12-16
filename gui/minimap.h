#pragma once

#include <QFrame>
#include <QPixmap>
#include <QPushButton>

#include "../core/sites/building.h"
#include "../core/sites/site.h"

class QLabel;

class District;
class DistrictMinimapItem;
class Site;
class Event;

class DistrictMinimap : public QFrame
{
    Q_OBJECT

public:
    using BuildingTypeToImageMap = std::map<Building::Type, QString>;
    using BuildingTypeToTitleMap = std::map<Building::Type, QString>;

    static QPixmap getBuildingImage(Building::Type type);
    static QPixmap getBuildingImage(const Building* building);
    static QString getBuildingTitle(Building::Type type);

public:
    using MinimapRow = std::vector<DistrictMinimapItem*>;
    using Minimap = std::vector<MinimapRow>;

    DistrictMinimap(District* district, QWidget *parent = 0);
    virtual ~DistrictMinimap();

    void setDistrict(District* district);
    DistrictMinimapItem* getSelectedItem() const;

    void updateMinimap();
    void highlightArea(int centerX, int centerY, int area = 0, bool on = true);
    void highlightArea(Site::Address address, int area = 0, bool on = true);

    void selectMinimapItem();

public slots:
    void onClickMinimapItem();

signals:
    void siteSelected(Site* site);

private:
    void setupLayout();

    static const BuildingTypeToImageMap mImageMap;
    static const BuildingTypeToTitleMap mTitleMap;

    size_t mMapSize;
    Minimap mMinimap;
    DistrictMinimapItem* mSelectedItem;

    QVector<QLabel*> mMapIndices;

private:
    District* mDistrict;
};



class DistrictMinimapItem : public QPushButton
{
    Q_OBJECT
public:
    DistrictMinimapItem(Site* site, DistrictMinimap* minimap);

    static QSize getSize() { return QSize(48, 48); }

    Site* getSite() const;

    void highlight(bool on = true);
    void updatePicture();

private:
    Site* mSite;
};
