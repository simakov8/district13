#include "game.h"
#include "administration.h"
#include "district.h"

Game::Game(size_t districtSize, int happinessGoal, int startMoney, int maxIncome)
    : mAdmin(nullptr),
      mHappinessGoal(happinessGoal),
      mTurn(0)
{
    District* district = new District(districtSize);

    mAdmin = new Administration(district, startMoney, maxIncome);
}

bool Game::nextTurn()
{
    if (isGameOver())
        return false;

    mExecutedEvents.clear();

    // execute environment events
    for (Event* event : mEvents)
        if (event->canBeExecuted())
        {
            event->execute();
            mExecutedEvents.push_front(event);
        }
    mEvents.clear();

    // execute user events
    for (Event* event : mUserEvents)
        if (event->canBeExecuted())
        {
            event->execute();
            mExecutedEvents.push_front(event);
        }
    mUserEvents.clear();

    mAdmin->nextTurn();
    mAdmin->getDistrict()->nextTurn();

    ++mTurn;
    return true;
}

void Game::restartGame()
{
    mEvents.clear();
    mUserEvents.clear();
    mExecutedEvents.clear();

    District* newDistrict = new District(mAdmin->getDistrict()->getSize());
    Administration* newAdmin = new Administration(newDistrict, 50, 10);

    delete mAdmin;
    mAdmin = newAdmin;

    mTurn = 0;
}

bool Game::isGameOver() const
{
    return mAdmin->calcAverageHappiness() < mHappinessGoal;
}

void Game::enqueueEvent(Event* event)
{
    if (!event)
        return;

    if (event->isCalledByUser())
        mUserEvents.push_front(event);
    else
        mEvents.push_front(event);
}
