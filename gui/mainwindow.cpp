#include "mainwindow.h"
#include "minimap.h"
#include "../core/game.h"
#include "../core/administration.h"
#include "../core/sites/site.h"
#include "../core/sites/building.h"
#include "gameinfowidget.h"
#include "siteinfodialog.h"
#include "actionswidget.h"

#include <QGridLayout>
#include <QMessageBox>
#include <QTextEdit>
#include <QLabel>
#include <QCloseEvent>


MainWindow::MainWindow(Game* game, QWidget *parent)
    : QWidget(parent),
      mGame(game)
{
    setWindowTitle(tr("Район №13"));

    initializeWidgets();
    setupLayout();
    setStyleSheet("#minimap { border: 1px solid DarkGrey }");

    updateGameInfo();
}

MainWindow::~MainWindow()
{
}

void MainWindow::onNextTurn()
{   
    if (!mGame->nextTurn())
        gameOver();

    printMessage(tr("<b>Ход №%1</b>").arg(mGame->getTurn()));
    updateGameInfo();
    printTurnSummary();

    if (mMinimapWidget->getSelectedItem())
        onSelectSite(mMinimapWidget->getSelectedItem()->getSite());
    else
        onSelectSite(nullptr);
    mMinimapWidget->updateMinimap();
}

void MainWindow::onEnqueueEvent(Event* event)
{
    if (!event)
        return;

    mGame->enqueueEvent(event);

    EventLogger logger;
    printMessage(logger.getEventLog(*event, true));

    updateGameInfo();
}

void MainWindow::onSelectSite(Site* site)
{
    DistrictMinimapItem* previouslySelected = mMinimapWidget->getSelectedItem();
    if (previouslySelected)
    {
        Site* site = previouslySelected->getSite();
        Site::Address addr = site->getAddress();
        int area = 0;
        if (site->getBuilding())
            area = site->getBuilding()->getInfluenceArea();
        mMinimapWidget->highlightArea(addr.first, addr.second, area, false);
    }

    mSiteInfo->showSiteInfo(site);
    mActionsWidget->chooseActions(site);

    if (site)
    {
        int area = 0;
        if (site->getBuilding())
            area = site->getBuilding()->getInfluenceArea();
        mMinimapWidget->highlightArea(site->getAddress().first, site->getAddress().second, area);
    }
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("Конец игры"));
    msgBox.setText(tr("Желаете закончить игру или начать заново?"));
    msgBox.setIcon(QMessageBox::Question);
    msgBox.addButton(tr("Закончить игру"), QMessageBox::RejectRole);
    msgBox.addButton(tr("Начать заново"), QMessageBox::AcceptRole);

    if (msgBox.exec() == QMessageBox::Rejected)
    {
        event->accept();
        close();
    }
    else
    {
        event->ignore();
        restart();
    }
}

void MainWindow::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    static bool firstStart = true;
    if (firstStart)
    {
        QMessageBox::information(this,
                                 tr("Цель игры"),
                                 tr("Ваша задача - не дать счастью жителей района опуститься ниже %1.\n").arg(mGame->getHappinessGoal()) +
                                 tr("Для этого стройте объекты инфраструктуры и следите за состоянием зданий и участков."));
        firstStart = false;
    }
}

void MainWindow::initializeWidgets()
{
    mMinimapWidget = new DistrictMinimap(mGame->getAdministration()->getDistrict(), this);
    mMinimapWidget->setObjectName("minimap");
    mMinimapWidget->layout()->setSizeConstraint(QLayout::SetFixedSize);
    connect(mMinimapWidget, SIGNAL(siteSelected(Site*)), this, SLOT(onSelectSite(Site*)));

    mLogWidget = new QTextEdit(tr("Добро пожаловать в Район №13!"));
    mLogWidget->setReadOnly(true);
    mLogWidget->setFixedHeight(150);

    mNextTurnButton = new QPushButton(tr("Следующий ход"));
    mNextTurnButton->setToolTip(tr("Текущий ход - ") + QString::number(mGame->getTurn()));
    mNextTurnButton->setIcon(QIcon(":/res/icons/time"));
    mNextTurnButton->setIconSize(QSize(30, 30));
    connect(mNextTurnButton, SIGNAL(clicked(bool)), this, SLOT(onNextTurn()));

    mGameInfo = new GameInfoWidget(this);
    mGameInfo->setObjectName("gameinfo");
    mGameInfo->setCurrentTurn(mGame->getTurn());

    mSiteInfo = new SiteInfoDialog(this);
    mSiteInfo->hide();
    connect(mMinimapWidget, SIGNAL(siteSelected(Site*)), mSiteInfo, SLOT(showSiteInfo(Site*)));

    mActionsWidget = new ActionsWidget(mGame->getAdministration(), this);
    connect(mActionsWidget, SIGNAL(initEvent(Event*)), this, SLOT(onEnqueueEvent(Event*)));
    connect(mMinimapWidget, SIGNAL(siteSelected(Site*)), mActionsWidget, SLOT(chooseActions(Site*)));
}

void MainWindow::setupLayout()
{
    auto mainLayout = new QGridLayout();
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    mainLayout->addWidget(mMinimapWidget, 0, 0, 3, 1);
    mainLayout->addWidget(mNextTurnButton, 3, 1, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    mainLayout->addWidget(mLogWidget, 3, 0, 1, 1, Qt::AlignTop);
    mainLayout->addWidget(mGameInfo, 0, 1, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    mainLayout->addWidget(mSiteInfo, 1, 1, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    mainLayout->addWidget(mActionsWidget, 2, 1, 1, 1, Qt::AlignTop | Qt::AlignLeft);

    setLayout(mainLayout);
}

void MainWindow::updateGameInfo()
{
    mNextTurnButton->setToolTip(tr("Текущий ход - ") + QString::number(mGame->getTurn()));

    Administration* admin = mGame->getAdministration();

    mGameInfo->setCurrentTurn(mGame->getTurn());
    mGameInfo->setAverageHappiness(admin->calcAverageHappiness());
    mGameInfo->setCurrentMoney(admin->getCurrentMoney(), admin->calcIncome());
}

void MainWindow::printMessage(const QString& message)
{
    mLogWidget->append(message);
}

void MainWindow::printMessage(const EventLogger::EventSummary& summary)
{
    for (const QString& message : summary)
        printMessage(message);
}

void MainWindow::gameOver()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("Конец игры"));
    msgBox.setText(tr("Вы проиграли! Ваши жители слишком несчастны!\nНачать игру заново?"));
    msgBox.setIconPixmap(QPixmap(":/res/icons/sad_face"));
    msgBox.addButton(tr("Закончить игру"), QMessageBox::RejectRole);
    msgBox.addButton(tr("Начать заново"), QMessageBox::AcceptRole);

    if (msgBox.exec() == QMessageBox::Rejected)
        close();

    restart();
}

void MainWindow::restart()
{
    mLogWidget->clear();
    printMessage(tr("Новая игра началась"));
    printMessage(tr("Добро пожаловать в Район №13"));

    mGame->restartGame();
    mMinimapWidget->setDistrict(mGame->getAdministration()->getDistrict());
    mActionsWidget->setAdministration(mGame->getAdministration());
    updateGameInfo();
}

void MainWindow::printTurnSummary()
{
    EventLogger logger;

    Game::EventQueue events = mGame->getExecutedEvents();
    for (Event* event : events)
        printMessage(logger.getEventLog(*event));
}
