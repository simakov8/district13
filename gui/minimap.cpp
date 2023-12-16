#include <stdexcept>

#include "minimap.h"
#include "siteinfodialog.h"

#include "../core/administration.h"
#include "../core/district.h"
#include "../core/sites/site.h"
#include "../core/sites/house.h"
#include "../core/events/event.h"

#include <QIcon>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>

using namespace std;

const DistrictMinimap::BuildingTypeToImageMap DistrictMinimap::mImageMap = {
    { Building::Type::NONE, ":/res/icons/empty" },
    { Building::Type::HOUSE, ":/res/icons/house" },
    { Building::Type::SHOP, ":/res/icons/shop" },
    { Building::Type::SCHOOL, ":/res/icons/school" },
    { Building::Type::PARK, ":/res/icons/park" },
    { Building::Type::FACTORY, ":/res/icons/factory" }
};

const DistrictMinimap::BuildingTypeToTitleMap DistrictMinimap::mTitleMap = {
    { Building::Type::NONE, tr("Пустырь") },
    { Building::Type::HOUSE, tr("Жилой дом") },
    { Building::Type::SHOP, tr("Магазин") },
    { Building::Type::SCHOOL, tr("Школа") },
    { Building::Type::PARK, tr("Парк") },
    { Building::Type::FACTORY, tr("Завод") }
};

DistrictMinimap::DistrictMinimap(District* district, QWidget *parent)
    : QFrame(parent),
      mDistrict(nullptr),
      mSelectedItem(nullptr)
{
    setDistrict(district);
}

DistrictMinimap::~DistrictMinimap()
{
    for (MinimapRow& row : mMinimap)
        for (DistrictMinimapItem* mapItem : row)
            delete mapItem;
    mMinimap.clear();

    mMapIndices.clear();
}

void DistrictMinimap::setDistrict(District* district)
{
    Q_ASSERT(district);
    mDistrict = district;
    mSelectedItem = nullptr;

    for (MinimapRow& row : mMinimap)
        for (DistrictMinimapItem* mapItem : row)
            delete mapItem;

    mMapSize = mDistrict->getSize();

    // TODO: check if equal size
    mMinimap = Minimap(mMapSize, MinimapRow(mMapSize, nullptr));

    for (size_t i = 0; i < mMapSize; ++i)
        for (size_t j = 0; j < mMapSize; ++j)
            mMinimap.at(i).at(j) = new DistrictMinimapItem(mDistrict->getSiteAt(i, j), this);

    setupLayout();
}

DistrictMinimapItem* DistrictMinimap::getSelectedItem() const
{
    return mSelectedItem;
}

void DistrictMinimap::onClickMinimapItem()
{
    // remove selection from previously selected site
    if (mSelectedItem)
    {
        Site* previous = mSelectedItem->getSite();
        int area = previous->getBuilding() ? previous->getBuilding()->getInfluenceArea() : 0;
        highlightArea(previous->getAddress(), area, false);
    }

    DistrictMinimapItem* mapItem = qobject_cast<DistrictMinimapItem*>(QObject::sender());
    mSelectedItem = mapItem;

    Site* site = mapItem->getSite();

    // highlight area around selected site
    int area = site->getBuilding() ? site->getBuilding()->getInfluenceArea() : 0;
    highlightArea(site->getAddress(), area, true);

    siteSelected(site);
}

void DistrictMinimap::setupLayout()
{
    if (layout())
    {
        delete layout();
        qDeleteAll(mMapIndices);
        mMapIndices.clear();
    }

    QGridLayout* layout = new QGridLayout(this);
    layout->setSpacing(0);
    layout->setSizeConstraint(QLayout::SetMaximumSize);

    QLabel* indexLabel;
    for (size_t i = 0; i < mMapSize; ++i)
    {
        indexLabel = new QLabel(QString::number(i));
        indexLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(indexLabel, i + 1, 0, 1, 1);
        mMapIndices.append(indexLabel);

        indexLabel = new QLabel(QString::number(i));
        indexLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(indexLabel, 0, i + 1, 1, 1);
        mMapIndices.append(indexLabel);
    }

    for (size_t i = 0; i < mMapSize; ++i)
        for (size_t j = 0; j < mMapSize; ++j)
            layout->addWidget(mMinimap.at(i).at(j), i + 1, j + 1, 1, 1);
}

void DistrictMinimap::updateMinimap()
{
    //onClickMinimapItem(nullptr);

    for (size_t i = 0; i < mMapSize; ++i)
        for (size_t j = 0; j < mMapSize; ++j)
            mMinimap.at(i).at(j)->updatePicture();
}

void DistrictMinimap::highlightArea(int centerX, int centerY, int area, bool on)
{
    for (int i = centerX - area; i <= centerX + area; ++i)
        for (int j = centerY - area; j <= centerY + area; ++j)
        {
            try
            {
                mMinimap.at(i).at(j)->highlight(on);
            }
            catch (std::out_of_range o)
            {
                continue;
            }
        }
}

void DistrictMinimap::highlightArea(Site::Address address, int area, bool on)
{
    highlightArea(address.first, address.second, area, on);
}

void DistrictMinimap::selectMinimapItem()
{

}

QPixmap DistrictMinimap::getBuildingImage(Building::Type type)
{
    BuildingTypeToImageMap::const_iterator it;
    it = mImageMap.find(type);
    if (it != mImageMap.cend())
        return QPixmap(it->second);
    else
        return QPixmap(mImageMap.find(Building::Type::NONE)->second);
}

QPixmap DistrictMinimap::getBuildingImage(const Building* building)
{
    return getBuildingImage(building ? building->getType() : Building::Type::NONE);
}

QString DistrictMinimap::getBuildingTitle(Building::Type type)
{
    BuildingTypeToTitleMap::const_iterator it;
    it = mTitleMap.find(type);
    if (it != mTitleMap.cend())
        return it->second;
    else
        return QString("");
}

DistrictMinimapItem::DistrictMinimapItem(Site* site, DistrictMinimap* minimap)
    : QPushButton(minimap), mSite(site)
{
    setContentsMargins(0, 0, 0, 0);
    setMinimumSize(getSize());
    setMaximumSize(getSize());
    setIconSize(getSize());

    updatePicture();

    setFlat(true);
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    connect(this, SIGNAL(clicked(bool)), minimap, SLOT(onClickMinimapItem()));
}

Site* DistrictMinimapItem::getSite() const
{
    return mSite;
}

void DistrictMinimapItem::highlight(bool on)
{
    if (on)
        setStyleSheet("border: 0px solid black; background-color: #A9F16C;");
    else
        setStyleSheet("");
}

void DistrictMinimapItem::updatePicture()
{
    Building* building = mSite->getBuilding();
    QIcon icon = QIcon(DistrictMinimap::getBuildingImage(building));
    setIcon(icon);
}
