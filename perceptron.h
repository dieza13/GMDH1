#ifndef PERCEPTRON_H
#define PERCEPTRON_H
#include "vector"
#include "neiron.h"
#include "sample.h"
#include <perceptronbody.h>

class Perceptron
{
public:
    Perceptron();
    Perceptron(Sample teachExams, int eraCount, double alpha, double velocity);
    ~Perceptron();
public:
    void teachPerceptron(bool isResult);
    std::vector<int>  getExamplesNum();
    std::vector<int> getNeironsNum();
    double getFunctionValue(int neironNum, int examNum);
    double getFunctionValue(int neironNum, double * exam);
    double * getExample(int num);
    Neiron * getNeiron(int i);
    Sample * getExamples();
    void setPerceptronBody(QString name);
    PerceptronBody * getPerceptronBody();
    void calcError(QString netName, QVBoxLayout *normalErrorPL, bool isResult, std::vector<std::vector<double> > *allNetsError, int errorNum, Sample examples);
    void calcNormDenorm(double * norm, double * denorm, int neironNum, Sample *examples, int exampleNum, double *deNorm, double *value);
    void setTable(int row, int col, double value, QTableWidget *table);
    void setTable(int row, int col, QString value, QTableWidget * table);
    double denormValue(Sample * examples, int valueNum, double value);

    double getError();
    void showErrors(double d, double e, QString *errorD, QString *errorN, int n, int i, QTableWidget *normalErrorT);



public:
    Sample teachExamples;
    std::vector<Neiron*> neirons;

private:



    PerceptronBody * perceptronBody;
public:
    double velocity;
    int eraCount;
    double alpha;

private:
    void setNeirons(int enterCount, double velocity, double alpha);
    void teachingNeirons(bool isResult, double velocity);
    std::vector<double*> randomExamps();
    void generateResultNet();





};

#endif // PERCEPTRON_H
