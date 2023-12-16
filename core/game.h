#pragma once

#include <forward_list>
#include "events/event.h"

class Administration;

class Game
{
public:
    using EventQueue = std::forward_list<Event*>;

    Game(size_t districtSize, int happinessGoal, int startMoney, int maxIncome);

    Administration* getAdministration() const { return mAdmin; }
    int getHappinessGoal() const { return mHappinessGoal; }

    unsigned getTurn() const { return mTurn; }
    bool nextTurn();

    void restartGame();
    bool isGameOver() const;

    EventQueue getExecutedEvents() const { return mExecutedEvents; }
    void enqueueEvent(Event* event);

private:
    Administration* mAdmin;

    EventQueue mEvents;
    EventQueue mUserEvents;
    EventQueue mExecutedEvents;

    int mHappinessGoal;
    unsigned mTurn;
};
