#ifndef CALCULATEDRANGE_H
#define CALCULATEDRANGE_H
#include <QList>
#include <vector>
#include<QVector>
#include <QString>


class CalculatedRange
{
public:
    CalculatedRange();
public:
    std::vector<QString> columnNames;
    std::vector<std::vector<double> > allRanges;
public:
};

#endif // CALCULATEDRANGE_H
