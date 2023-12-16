#include "actionswidget.h"
#include "../core/administration.h"
#include "../core/sites/site.h"
#include "../core/sites/building.h"
#include "../core/events/event.h"
#include "builddialog.h"

#include <QGroupBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>

ActionsWidget::ActionsWidget(Administration* admin, QWidget* parent)
    : QFrame(parent),
      mTargetSite(nullptr),
      mAdmin(admin)
{
    mActionsGroupbox = new QGroupBox(tr("Действия"), this);

    new QVBoxLayout(mActionsGroupbox);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(mActionsGroupbox);

    chooseActions(mTargetSite);
}

void ActionsWidget::setAdministration(Administration* admin)
{
    mAdmin = admin;
}

void ActionsWidget::chooseActions(Site* targetSite)
{
    qDeleteAll(mActions);
    mActions.clear();

    mTargetSite = targetSite;

    if (!mTargetSite)
    {
        hide();
        return;
    }

    show();

    QList<Event::Type> actionTypes = mAdmin->getAvailableActions(mTargetSite);

    // build action
    if (actionTypes.contains(Event::Type::Construction))
    {
        mActions.push_back(addAction(QIcon(":/res/icons/crane"), tr("Построить")));
        connect(mActions.back(), SIGNAL(clicked(bool)), this, SLOT(onShowBuildDialog()));
    }

    // repair action
    if (actionTypes.contains(Event::Type::Repairing))
    {
        mActions.push_back(addAction(QIcon(":/res/icons/repair"), tr("Ремонт")));
        connect(mActions.back(), SIGNAL(clicked(bool)), this, SLOT(onRepairBuilding()));
    }

    // clean action
    if (actionTypes.contains(Event::Type::Cleaning))
    {
        mActions.push_back(addAction(QIcon(":/res/icons/broom"), tr("Уборка")));
        connect(mActions.back(), SIGNAL(clicked(bool)), this, SLOT(onCleanSite()));
    }

    foreach (QPushButton* action, mActions)
        layout()->addWidget(action);
}

QPushButton* ActionsWidget::addAction(QIcon icon, QString title)
{
    QPushButton* action = new QPushButton(icon, title);
    action->setIconSize(QSize(30, 30));

    return action;
}

void ActionsWidget::onShowBuildDialog()
{
    using Result = Administration::CheckResult;

    BuildDialog dialog;
    if (dialog.exec() != QDialog::Accepted)
        return;

    Building::Type type = dialog.getChosenType();
    Site::Address address = mTargetSite->getAddress();

    Result result = mAdmin->canConstructBuilding(address, type);

    QString warningMessage;
    switch (result)
    {
    case Result::SUCCESS:
        initEvent(mAdmin->constructBuilding(address, type));
        return;
    case Result::NOT_ENOUGH_MONEY:
        warningMessage = tr("Не хватает средств!");
        break;
    case Result::ALREADY_STARTED:
        warningMessage = tr("На участке уже ведется строительство!");
        break;
    }
    QMessageBox::information(this,
                             tr("Строительство"),
                             warningMessage);
}

void ActionsWidget::onRepairBuilding()
{
    using Result = Administration::CheckResult;

    Site::Address address = mTargetSite->getAddress();

    Result result = mAdmin->canRepairBuilding(address);

    QString warningMessage;
    switch (result)
    {
    case Result::SUCCESS:
        initEvent(mAdmin->repairBuilding(address));
        return;
    case Result::NOT_ENOUGH_MONEY:
        warningMessage = tr("Не хватает средств!");
        break;
    case Result::ALREADY_STARTED:
        warningMessage = tr("Ремонт уже начался!");
        break;
    case Result::MAX_VALUE:
        warningMessage = tr("Здание в наилучшем состоянии!");
        break;
    }
    QMessageBox::information(this,
                             tr("Ремонт"),
                             warningMessage);
}

void ActionsWidget::onCleanSite()
{
    using Result = Administration::CheckResult;

    Site::Address address = mTargetSite->getAddress();

    Result result = mAdmin->canCleanSite(address);

    QString warningMessage;
    switch (result)
    {
    case Result::SUCCESS:
        initEvent(mAdmin->cleanSite(address));
        return;
    case Result::NOT_ENOUGH_MONEY:
        warningMessage = tr("Не хватает средств!");
        break;
    case Result::ALREADY_STARTED:
        warningMessage = tr("Уборка уже началась!");
        break;
    case Result::MAX_VALUE:
        warningMessage = tr("Участок достаточно чист!");
        break;
    }
    QMessageBox::information(this,
                             tr("Уборка"),
                             warningMessage);
}
