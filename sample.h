#ifndef SAMPLE_H
#define SAMPLE_H
#include <vector>
#include <QList>
#include <QString>

class Sample
{

public:
    Sample();
private:
    std::vector<int> neironsNum;
    std::vector<double*> examples;
    std::vector<int> examplesNum;
    std::vector<int> entersNum;
    std::vector<std::vector<double> > minMaxNeironsValue;
    std::vector<std::vector<double> > minMaxEntersValue;
    int neironCount;
    int exampleCount;
    int enterCount;
    QList<QString> entersName;
    QList<QString> neironsName;
public:
    void setEntersNum(std::vector<int> entersNum);
    void setNeironsNum(std::vector<int> neironsNum);
    void setExamples(std::vector<double*> examples);
    void setExamplesNum(std::vector<int> examplesNum);
    std::vector<int> * getEntersNum();
    std::vector<int> getNeironsNum();
    std::vector<double*> getExamples();
    std::vector<int>  getExamplesNum();
    void setEnterCount(int enterCount);
    int getEnterCount();
    void setMinMaxNeironsValue(std::vector<std::vector<double> > minMaxNeironsValue);
    void setMinMaxEntersValue(std::vector<std::vector<double> > minMaxEntersValue);
    double getNeironMaxValue(int i);
    double getNeironMinValue(int i);
    int getExamplesCount();
    int getNeironCount();
    void setEnter(int row, int col, double value);
    void setEntersName(QList<QString> names);
    QList<QString> * getEntersName();
    void setNeironsName(QList<QString> names);
    QList<QString> * getNeironsName();
    double normalizEnterValue(double value, int enterNum);




};

#endif // SAMPLE_H
