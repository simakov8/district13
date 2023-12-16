#include "factor.h"


HappinessFactor::HappinessFactor(int minValue, int maxValue, int step, int baseValue, int happinessImpact)
    : mValue(baseValue),
      mStep(step),
      mMaxValue(maxValue),
      mMinValue(minValue),
      mBaseValue(baseValue),
      mMaxHappinessImpact(happinessImpact)
{

}

HappinessFactor::HappinessFactor(const HappinessFactor& factor)
    : mValue(factor.mValue),
      mStep(factor.mStep),
      mMaxValue(factor.mMaxValue),
      mMinValue(factor.mMinValue),
      mBaseValue(factor.mBaseValue),
      mMaxHappinessImpact(factor.mMaxHappinessImpact)
{
}

HappinessFactor& HappinessFactor::operator++()
{
    mValue += mStep;
    if (mValue > mMaxValue)
        mValue = mMaxValue;
    return *this;
}

HappinessFactor& HappinessFactor::operator++(int)
{
    return ++(*this);
}

HappinessFactor& HappinessFactor::operator--()
{
    mValue -= mStep;
    if (mValue < mMinValue)
        mValue = mMinValue;
    return *this;
}

HappinessFactor& HappinessFactor::operator--(int)
{
    return --(*this);
}

void HappinessFactor::setValue(int value)
{
    if (value > mMaxValue)
        mValue = mMaxValue;
    else if (value < mMinValue)
        mValue = mMinValue;
    else
        mValue = value;
}

void HappinessFactor::setStep(int step)
{
    mStep = step;
}

int HappinessFactor::getHappinessImpact() const
{
    const int difference = mValue - mBaseValue;
    const int maxDifference = difference <= 0 ? mBaseValue - mMinValue : mMaxValue - mBaseValue;

    const int happinessImpact = ((double)difference / maxDifference) * mMaxHappinessImpact;
    return happinessImpact;
}
