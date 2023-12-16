#include "builddialog.h"
#include "minimap.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>

const QMap<Building::Type, QString> BuildDialog::mBuildDescription =
{
    { Building::Type::SHOP,
      tr("Значительно увеличивает счастье домов в небольшом радиусе.") },
    { Building::Type::SCHOOL,
      tr("Значительно увеличивает счастье домов в большом радиусе.") },
    { Building::Type::FACTORY,
      tr("Каждый ход приносит доход, зависящий от состояния завода. Уменьшает счастье соседних домов.") },
    { Building::Type::PARK,
      tr("В два раза замедляет загрязнение участков и незначительно увеличивает счастье домов в большом радиусе.") },
};


BuildDialog::BuildDialog()
    : mChosenType(Building::Type::NONE)
{
    setWindowTitle(tr("Построить"));

    const Building::TypeList types = Building::getAllTypes();

    auto layout = new QVBoxLayout();

    for (const Building::Type type : types)
    {
        // TODO: remove if
        if (type != Building::Type::NONE && type != Building::Type::HOUSE)
        {
            auto buildButton = new QPushButton();
            buildButton->setIcon(DistrictMinimap::getBuildingImage(type));
            buildButton->setIconSize(QSize(40, 40));
            buildButton->setText(DistrictMinimap::getBuildingTitle(type));

            connect(buildButton, SIGNAL(clicked()), this, SLOT(onChooseBuilding()));

            layout->addWidget(buildButton);
            mBuildButtons.insert(buildButton, type);
        }
    }

    auto cancelButton = new QPushButton(tr("Назад"));
    connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(reject()));

    layout->addWidget(cancelButton, 0, Qt::AlignRight);
    setLayout(layout);
}

void BuildDialog::onChooseBuilding()
{
    QPushButton* pushedButton = qobject_cast<QPushButton*>(QObject::sender());
    mChosenType = mBuildButtons[pushedButton];

    QString description = mBuildDescription[mChosenType];

    QMessageBox info(this);
    info.setText(description + tr("\nСтоимость: %1").arg(Building::getBuildCost(mChosenType)));
    info.setWindowTitle(DistrictMinimap::getBuildingTitle(mChosenType));
    info.setIconPixmap(DistrictMinimap::getBuildingImage(mChosenType));
    info.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    if (info.exec() == QMessageBox::Ok)
        accept();
}

Building::Type BuildDialog::getChosenType() const
{
    return mChosenType;
}
