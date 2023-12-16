#include "gameinfowidget.h"

#include <QLabel>
#include <QVBoxLayout>

GameInfoWidget::GameInfoWidget(QWidget *parent)
    : QWidget(parent)
{
    mCurrentTurn = new QLabel();
    mHappiness = new QLabel();
    mMoney = new QLabel();

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(mCurrentTurn);
    layout->addWidget(mHappiness);
    layout->addWidget(mMoney);
    setLayout(layout);
}

void GameInfoWidget::setCurrentTurn(int turn)
{
    mCurrentTurn->setText(tr("Ход №%1").arg(turn));
}

void GameInfoWidget::setAverageHappiness(int happiness)
{
    mHappiness->setText(tr("<b>Уровень счастья: %1</b>").arg(happiness));
}

void GameInfoWidget::setCurrentMoney(int money, int income)
{
    mMoney->setText(tr("Деньги: %1 (+%2)").arg(money).arg(income));
}

