#ifndef FILEREADER_H
#define FILEREADER_H
#include "QString"
#include "vector"
#include "sample.h"

class FileReader
{
public:
    FileReader();
    ~FileReader();
public:
    std::vector<QStringList> getDataFromFile(QString string);
    void getFirstLayerExams(std::vector<int> columnsNum, int enterCount, Sample * sample);
     void getSecondLayerExams(std::vector<int> exampleNums, std::vector<int> newEnters, std::vector<int> newNeirons, Sample *sample);
public:
    std::vector<std::vector<double> > minMaxForColumn;


private:
    std::vector<double*> dataRows;
    std::vector<double*> normalizeDataRows;
    int columnNum;
private:
    bool checkIfNumber(const QString &s);
    void normalizeData();
    void getResultNeironNums(std::vector<int> * neironNums, std::vector<int> * newNeironNums, std::vector<int> * resultNeironNums);
    bool isCorrect(std::vector<int> * checkedConteiner, double * example);
};

#endif // FILEREADER_H
