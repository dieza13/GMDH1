#ifndef FILEREADER_H
#define FILEREADER_H
#include "QString"
#include "vector"
#include "sample.h"
#include <qprogressbar.h>
#include <QLabel>
#include <perceptroncontext.h>

class FileReader
{
public:
    FileReader();
    ~FileReader();
public:
    std::vector<QStringList> getDataFromFile(QString string, std::vector<double *> * dataRows, std::vector<std::vector<double> > * minMaxForColumn);
    QStringList getDataFromFile(QString string, std::vector<double *> * dataRows);
    void getFirstLayerExams(std::vector<double *> *dataRows, std::vector<int> columnsNum, int enterCount, Sample * sample, bool needSetMinMax);
     void getSecondLayerExams(std::vector<double *> * dataRows, std::vector<int> exampleNums, std::vector<int> newEnters, std::vector<int> newNeirons, Sample *sample, bool needMinMax);
public:
    std::vector<std::vector<double> > minMaxForColumn;
    std::vector<std::vector<double> > minMaxNormForColumn;
    void writeRangesToFile(std::vector<std::vector<double> > ranges, QString fileName, std::vector<QString> columnNames, QProgressBar * saveRangePrB, QLabel * saveLabel);
    void saveModel(PerceptronContext * model, QString fileName);
    PerceptronContext * loadModel(QString fileName);
    template <typename S, typename V> QMap<S, V> parseMap(QString string);
    std::vector<Sample> getTeachSamples(PerceptronContext * model, QString fileName);
private:    
    std::vector<double*> normalizeDataRows;
    int columnNum;
private:
    bool checkIfNumber(const QString &s);
    void normalizeData(std::vector<double *> * dataRows, std::vector<std::vector<double> > * minMaxForColumn/*, std::vector<std::vector<double> > *minMaxNormForColumn*/);
    void getResultNeironNums(std::vector<int> * neironNums, std::vector<int> * newNeironNums, std::vector<int> * resultNeironNums);
    bool isCorrect(std::vector<int> * checkedConteiner, double * example);   

    std::string writeVecOfVec(std::vector<std::vector<double> > vector);
    std::vector<std::vector<double> > readVecOfVec(QFile * file);
    template<typename T> std::string writeVec(std::vector<T> vector);
    std::string writeList(QList<QString> l);
    QList<QString> readList(QFile * file);
    template<typename T> std::vector<T> readVec(QFile * file);
    std::string writePerceptron(Perceptron * net, std::string name);
    Perceptron * readPerceptron(QFile * file);

public:
    std::vector<double*> dataRows;
};

#endif // FILEREADER_H
