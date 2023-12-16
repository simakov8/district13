#pragma once

#include "eventlogger.h"
#include <QWidget>

class DistrictMinimap;
class Game;
class QPushButton;
class QTextEdit;
class QLabel;
class GameInfoWidget;
class SiteInfoDialog;
class ActionsWidget;
class QCloseEvent;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(Game* game, QWidget *parent = 0);
    ~MainWindow();

    void printTurnSummary();

public slots:
    void onNextTurn();

    void onEnqueueEvent(Event* event);

    void onSelectSite(Site* site);

protected:
    virtual void closeEvent(QCloseEvent *event);
    virtual void showEvent(QShowEvent *event);

private:
    void initializeWidgets();
    void setupLayout();

    void updateGameInfo();

    void printMessage(const QString& message);
    void printMessage(const EventLogger::EventSummary& summary);

    void gameOver();
    void restart();

private:
    Game* mGame;

    DistrictMinimap* mMinimapWidget;
    QTextEdit* mLogWidget;
    QPushButton* mNextTurnButton;
    GameInfoWidget* mGameInfo;
    SiteInfoDialog* mSiteInfo;
    ActionsWidget* mActionsWidget;
};
