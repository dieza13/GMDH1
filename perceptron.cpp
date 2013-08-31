#include "perceptron.h"
#include <stdlib.h>
#include <algorithm>
#include "iostream"


Perceptron::~Perceptron()
{
    for (int i = neirons.size() - 1; i >=  0; i--) {
        Neiron * neiron = neirons.at(i);
        delete neiron;
    }
}

//установка параметров сети
Perceptron::Perceptron(Sample teachExams, int eraCount, double alpha,  double velocity)
{
    this->eraCount = eraCount;
    this->velocity = velocity;
    this->alpha = alpha;
    teachExamples = teachExams;    
    setNeirons(teachExams.getEnterCount(), this->velocity, this->alpha);
}

Perceptron::Perceptron()
{
}

//создание нейронов и установка их параметров: количество входов, скорость обучения, параметр сигмоидальной функции alpha
void Perceptron::setNeirons(int enterCount, double velocity, double alpha)
{
    for (int i = 0; i < teachExamples.getNeironsNum().size(); i++) {
        Neiron * neiron = new Neiron(enterCount, velocity, alpha, teachExamples.getNeironsNum().at(i));
        neiron->setMinMaxFunkValue(teachExamples.getNeironMinValue(i), teachExamples.getNeironMaxValue(i));
        neirons.push_back(neiron);
    }
}

//обучение сети
void Perceptron::teachPerceptron(bool isResult) {

    for (int i = 0; i < neirons.size(); i++) {
        Neiron * neiron = neirons.at(i);
        neiron->iniWeight();
    }


    for (int i = 0; i < eraCount; i++) {
        double _velocity = this->velocity - this->velocity * i / eraCount;
        teachingNeirons(isResult, _velocity);
    }
}
//обучение нейронов
void Perceptron::teachingNeirons(bool isResult, double velocity){
    std::vector<double*> examples = teachExamples.getExamples();
    for (int i = 0; i < examples.size(); i ++) {
        int numb = (int)(examples.size()*(double)rand()/RAND_MAX);
        double * example = examples[numb];
        examples.erase(examples.begin() + numb);
        for (int j = 0; j < teachExamples.neironCount; j++) {
            if (isResult) {
                if (teachExamples.neironsToNextLevel[j]) {
                    Neiron * neiron = neirons[j];
                    neiron->setVelocity(velocity);
                    neiron->calculateWeights(example, j);
                }
            } else {
                Neiron * neiron = neirons[j];
                neiron->setVelocity(velocity);
                neiron->calculateWeights(example, j);
            }
        }
    }
}

std::vector<int> Perceptron::getExamplesNum()
{
    return teachExamples.getExamplesNum();
}

std::vector<int> Perceptron::getNeironsNum()
{
    return teachExamples.getNeironsNum();
}

double Perceptron::getFunctionValue(int neironNum, int examNum)
{
    Neiron * neiron = neirons[neironNum];
    double * exam;
    for (int i = 0; i < getExamplesNum().size(); i++) {
        int num = getExamplesNum()[i];
        if (examNum == num) {
            exam = getExample(examNum);
            break;
        }
    }
    return neiron->calculateY(exam);
}

double * Perceptron::getExample(int num)
{
    int examNum;
    for (int i = 0; i < teachExamples.getExamplesNum().size(); i++) {
        if (num == teachExamples.getExamplesNum().at(i)) examNum = i;
    }
    return teachExamples.getExamples()[examNum];
}

Neiron * Perceptron::getNeiron(int i)
{
    return neirons[i];
}

Sample * Perceptron::getExamples()
{
    return &teachExamples;
}

//установка параметров выводимого на экран объекта - "Perceptron"
void Perceptron::setPerceptronBody(QString name)
{
    std::vector<QGraphicsItem*> neironsBody;
    for (int i = 0; i < neirons.size(); i++) {
        Neiron * neiron = neirons[i];
        neironsBody.push_back(neiron->getNeironBody());
    }
    teachExamples.getEntersNum();
    perceptronBody = new PerceptronBody(name, neironsBody, *teachExamples.getEntersNum());
}

PerceptronBody *Perceptron::getPerceptronBody()
{
    return perceptronBody;
}

void Perceptron::calcError(QString netName, QVBoxLayout * normalErrorPL, bool isResult, std::vector<std::vector<double> > * allNetsError, int errorNum, Sample examples)
{
    std::cout << "Входы:" << std::endl;
    for (int i = 0; i < examples.entersName.size(); i++) {
        std::cout << examples.entersName[i].toStdString() <<  "\t";
    }
    std::cout << std::endl;
    std::cout << "Выходы:" << std::endl;
    for (int i = 0; i < examples.neironsName.size(); i++) {
        std::cout << examples.neironsName[i].toStdString() <<  "\t";
    }
    std::cout << std::endl;
    std::cout << "Параметры настройки сети:" << std::endl;
    std::cout << "Скорость:" << "\t" << velocity << "\t" << "Параметр сигмоидальной функции:" << "\t" << alpha << "\t" << "Количество эпох обучения"
                 << "\t" << eraCount << std::endl;
    int neironsCount = examples.getNeironCount();
     int n = examples.getExamplesCount();
     int num = 3;
    QTableWidget * normalErrorT = new QTableWidget(n + num, neironsCount + 1);
    normalErrorT->verticalHeader()->setVisible(false);
    double * denorm = new double[neironsCount] ;
    double * norm = new double[neironsCount] ;
    for (int i = 0; i < neironsCount; i++) {
        denorm[i] = 0;
        norm[i] = 0;
    }
    for (int i = 0; i < examples.getExamplesCount(); i++) {        
        setTable(i, 0, examples.getExamplesNum()[i] + 1, normalErrorT);
        std::cout << examples.getExamplesNum()[i] + 1 << '\t';
        for (int k = 0; k < getNeironsNum().size(); k++) {
            double deNorm = 0;
            double value = 0;
            if (isResult) {
                if (!examples.neironsToNextLevel[k]) {
                    deNorm = 0;
                    std::cout << 0 << '\t' << "("<< 0 << ')' <<'\t';
                } else {
                    calcNormDenorm(norm, denorm, k, &examples, i, &deNorm, &value);
                }
            } else {
                calcNormDenorm(norm, denorm, k, &examples, i, &deNorm, &value);
            }
            setTable(i, k + 1, deNorm, normalErrorT);
            std::cout << value << '\t' << "("<< deNorm << ')' <<'\t';
        }
        std::cout << std::endl;
    }
    std::cout << "Ошибка по " + netName.toStdString() + " сети: ";
    QString errorD = "Denorm: \n ";
    QString errorN = "Norm: \n ";
    double normE = 0;
    int prevLev = 0;
    for (int i = 0; i < neironsCount; i++) {
        if (isResult) {

            if (!examples.neironsToNextLevel[i]) {
                std::vector<double> vec = allNetsError->at(prevLev);
                prevLev++;                
                showErrors(vec[1], vec[0], &errorD, &errorN, n, i, normalErrorT);
                normE += vec[0];
            } else {                
                showErrors(denorm[i], norm[i], &errorD, &errorN, n, i, normalErrorT);
                normE += norm[i];
            }
        } else {
            normE += norm[i];
            showErrors(denorm[i], norm[i], &errorD, &errorN, n, i, normalErrorT);
            if (!examples.neironsToNextLevel[i]) {
                std::vector<double> vec;
                vec.push_back(norm[i]);
                vec.push_back(denorm[i]);
                allNetsError->push_back(vec);
            }            
        }
    }
    std::cout << errorD.toStdString() << std::endl << errorN.toStdString() << " " << std::endl;
    normalErrorPL->addWidget(normalErrorT);
    setTable(n, 0, "Норм. ошибка", normalErrorT);
    setTable(n + 1, 0, "Денорм. ошибка", normalErrorT);
    setTable(n + 2, 0, "Общ. норм. ошибка", normalErrorT);
    normE = sqrt(normE/(neironsCount * n));
    std::cout << "Общ. норм. ошибка:" << "\t" << QString::number(normE).toStdString() << std::endl;
    setTable(n + 2, 1, normE, normalErrorT);
}

void Perceptron::showErrors(double d, double e,  QString * errorD, QString * errorN, int n, int i, QTableWidget * normalErrorT)
{
    double deE = sqrt(d / n);
    double noE = sqrt(e / n);
    errorD->append(QString::number(deE) + '\t');
    errorN->append(QString::number(noE) + '\t');
    setTable(n, i + 1, noE, normalErrorT);
    setTable(n + 1, i + 1, deE, normalErrorT);
}

void Perceptron::calcNormDenorm(double * norm, double * denorm, int neironNum, Sample * examples, int exampleNum, double * deNorm, double * value)
{

    double * exam = examples->examples[exampleNum];
    *value = getFunctionValue(neironNum, exam);
    *deNorm = denormValue(examples, neironNum, *value);
    double v = examples->getExamples()[exampleNum][examples->getEnterCount() + neironNum];
    double celevoe = denormValue(examples, neironNum, v);
    denorm[neironNum] += (*deNorm - celevoe) * (*deNorm - celevoe);
    norm[neironNum] += (*value - v) * (*value - v);
}

void Perceptron::setTable(int row, int col, double value, QTableWidget * table)
{
    QTableWidgetItem * cell = new QTableWidgetItem(QString::number(value));
    table->setItem(row, col, cell);
}

void Perceptron::setTable(int row, int col, QString value, QTableWidget *table)
{
    QTableWidgetItem * cell = new QTableWidgetItem(value);
    table->setItem(row, col, cell);
}

double Perceptron::denormValue(Sample *examples, int valueNum, double value)
{
    Neiron * neiron = neirons[valueNum];
    double minVal = neiron->minVal;
    double maxVal = neiron->maxVal;
//    double minVal = examples->getNeironMinValue(valueNum);
//    double maxVal = examples->getNeironMaxValue(valueNum);
    return (value * (maxVal - minVal) + minVal);
}

double Perceptron::getError()
{
    int neironsCount = teachExamples.getNeironCount();
    int n = teachExamples.getExamplesCount();
     int num = 3;
    double * norm = new double[neironsCount] ;
    for (int i = 0; i < neironsCount; i++) {
        norm[i] = 0;
    }
    for (int i = 0; i < teachExamples.getExamplesCount(); i++) {
        double * exam = teachExamples.examples[i];
        int neironPos = 0;
        for (int k = 0; k < getNeironsNum().size(); k++) {
            double value = getFunctionValue(k, exam);
            double v = teachExamples.getExamples()[i][teachExamples.getEnterCount() + k];
            norm[k] += (value - v) * (value - v);
        }
    }
    double normE = 0;
    for (int i = 0; i < neironsCount; i++) {
        double e = norm[i];
        normE += e;
    }
    normE = sqrt(normE/(neironsCount * n));
    return normE;
}



double Perceptron::getFunctionValue(int neironNum, double *exam)
{
    Neiron * neiron = neirons[neironNum];
    return neiron->calculateY(exam);
}



