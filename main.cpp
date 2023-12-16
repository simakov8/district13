#include "gui/mainwindow.h"
#include "core/game.h"
#include "core/administration.h"
#include <QApplication>
#include <time.h>

int main(int argc, char *argv[])
{
    srand(time(NULL));

    QApplication a(argc, argv);

    const int happinessGoal = 20;
    const int districtSize = 10;
    const int startMoney = 50;
    const int maxIncome = 10;

    Game game(districtSize, happinessGoal, startMoney, maxIncome);

    MainWindow w(&game);
    w.show();

    return a.exec();
}
