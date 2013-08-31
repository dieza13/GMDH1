#include "sample.h"

Sample::Sample()
{

}

void Sample::setNeironsNum(std::vector<int> neironsNum, std::vector<bool> neironsToNextLevel)
{
    this->neironsToNextLevel = neironsToNextLevel;
    this->neironsNum = neironsNum;
    neironCount = neironsNum.size();
}

void Sample::setNeironsNum(std::vector<int> neironsNum)
{
    this->neironsNum = neironsNum;
    neironCount = neironsNum.size();
}

void Sample::setExamples(std::vector<double*> examples)
{
    this->examples = examples;
    exampleCount = examples.size();
}

void Sample::setExamplesNum(std::vector<int> examplesNum)
{
    this->examplesNum = examplesNum;
}

std::vector<int> Sample::getNeironsNum()
{
    return neironsNum;
}

std::vector<double*> Sample::getExamples()
{
    return examples;
}

std::vector<int> Sample::getExamplesNum()
{
    return examplesNum;
}

void Sample::setEnterCount(int enterCount)
{
    this->enterCount = enterCount;
}

int Sample::getEnterCount()
{
    return enterCount;
}



void Sample::setMinMaxNeironsValue(std::vector<std::vector<double> > minMaxColumnsValue)
{
    this->minMaxNeironsValue.clear();
    this->minMaxNeironsValue = minMaxColumnsValue;
}

double Sample::getNeironMinValue(int i)
{
    return minMaxNeironsValue[i][0];
}

double Sample::getNeironNormMaxValue(int i)
{
    return minMaxNormNeironsValue[i][1];
}

double Sample::getNeironNormMinValue(int i)
{
    return minMaxNormNeironsValue[i][0];
}

double Sample::getNeironMaxValue(int i)
{
    return minMaxNeironsValue[i][1];
}

int Sample::getExamplesCount()
{
    return exampleCount;
}

int Sample::getNeironCount()
{
    return neironCount;
}

void Sample::setEnter(int row, int col, double value)
{
    examples[row][col] = value;
}

void Sample::setEntersNum(std::vector<int> entersNum)
{
    for (int i = 0; i < entersNum.size(); i++)
        this->entersNum.push_back(entersNum[i]);
    enterCount = entersNum.size();
}

std::vector<int> *Sample::getEntersNum()
{
    return &entersNum;
}

void Sample::setEntersName(QList<QString> names)
{
    entersName = names;
}

QList<QString> *Sample::getEntersName()
{
    return &entersName;
}

void Sample::setNeironsName(QList<QString> names)
{
    neironsName = names;
}
QList<QString> *Sample::getNeironsName()
{
    return &neironsName;
}

double Sample::normalizEnterValue(double value, int enterNum)
{
    int j = 0;
    for (int i = 0; i < enterCount; i++) {
        if (enterNum == entersNum[i]) {
            j = i;
            break;
        }
    }
    double maxVal = minMaxEntersValue[j][1];
    double minVal = minMaxEntersValue[j][0];
    double normValue = (value - minVal) / (maxVal - minVal);
    return normValue;
}

void Sample::setMinMaxEntersValue(std::vector<std::vector<double> > minMaxEntersValue)
{
    this->minMaxEntersValue.clear();
    this->minMaxEntersValue = minMaxEntersValue;
}

void Sample::setMinMaxNormNeironsValue(std::vector<std::vector<double> > minMaxNeironsValue)
{
    this->minMaxNormNeironsValue.clear();
    this->minMaxNormNeironsValue = minMaxNeironsValue;

}

void Sample::setMinMaxNormEntersValue(std::vector<std::vector<double> > minMaxEntersValue)
{
    this->minMaxNormEntersValue.clear();
    this->minMaxNormEntersValue = minMaxEntersValue;
}






