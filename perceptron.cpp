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

void Perceptron::generateResultNet()
{
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

//    for (int i = 0; i < teachExamples.getEnterCount(); i++) {
//        std::cout<< exam[i] <<  ' ';
//    }
//    std::cout << std::endl;
//    neiron->showWeights();


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
    QList<QGraphicsItem*> neironsBody;
    for (int i = 0; i < neirons.size(); i++) {
        Neiron * neiron = neirons[i];
        neironsBody.push_back(neiron->getNeironBody());
    }
//    std::vector<int> nums;
    teachExamples.getEntersNum();
    perceptronBody = new PerceptronBody(name, neironsBody, *teachExamples.getEntersNum());
}

PerceptronBody *Perceptron::getPerceptronBody()
{
    return perceptronBody;
}



void Perceptron::calcError(QString netName, QVBoxLayout * normalErrorPL, bool isResult, std::vector<double> * allNetsError, int errorNum)
{
    std::cout << "Входы:" << std::endl;
    for (int i = 0; i < teachExamples.entersName.size(); i++) {
        std::cout << teachExamples.entersName[i].toStdString() <<  "\t";
    }
    std::cout << std::endl;

    std::cout << "Выходы:" << std::endl;
    for (int i = 0; i < teachExamples.neironsName.size(); i++) {
        std::cout << teachExamples.neironsName[i].toStdString() <<  "\t";
    }
    std::cout << std::endl;

    std::cout << "Параметры настройки сети:" << std::endl;

    std::cout << "Скорость:" << "\t" << velocity << "\t" << "Параметр сигмоидальной функции:" << "\t" << alpha << "\t" << "Количество эпох обучения"
                 << "\t" << eraCount << std::endl;


    int neironsCount = teachExamples.getNeironCount();



     int n = teachExamples.getExamplesCount();


     int num = 3;
    QTableWidget * normalErrorT = new QTableWidget(n + num, neironsCount + 1);
    normalErrorT->verticalHeader()->setVisible(false);

    double * denorm = new double[neironsCount] ;
    double * norm = new double[neironsCount] ;
    for (int i = 0; i < neironsCount; i++) {
        denorm[i] = 0;
        norm[i] = 0;
    }



    for (int i = 0; i < teachExamples.getExamplesCount(); i++) {
        double * exam = teachExamples.examples[i];

//        for (int k = 0; k < teachExamples.enterCount; k++)
//            std::cout << exam[k] << " ";
//        std::cout << std::endl;

        setTable(i, 0, teachExamples.getExamplesNum()[i] + 1, normalErrorT);
        std::cout << teachExamples.getExamplesNum()[i] + 1 << '\t';
        int neironPos = 0;
        for (int k = 0; k < getNeironsNum().size(); k++) {
            double value = getFunctionValue(k, exam);
            double deNorm = value * (teachExamples.getNeironMaxValue(k) - teachExamples.getNeironMinValue(k)) + teachExamples.getNeironMinValue(k);
            double v = teachExamples.getExamples()[i][teachExamples.getEnterCount() + k];

            double celevoe = v * (teachExamples.getNeironMaxValue(k) - teachExamples.getNeironMinValue(k)) + teachExamples.getNeironMinValue(k);
            denorm[k] += (deNorm - celevoe) * (deNorm - celevoe);
            norm[k] += (value - v) * (value - v);
            if (isResult) {
                if (!teachExamples.neironsToNextLevel[k]) {
                    setTable(i, k + 1, 0, normalErrorT);
                    std::cout << 0 << '\t' << "("<< 0 << ')' <<'\t';
                } else {
                    setTable(i, k + 1, deNorm, normalErrorT);
                    std::cout << value << '\t' << "("<< deNorm << ')' <<'\t';
                }
            } else {
                setTable(i, k + 1, deNorm, normalErrorT);
                std::cout << value << '\t' << "("<< deNorm << ')' <<'\t';
            }
//            std::cout << value << '\t' << "("<< deNorm << ')' <<'\t';

//            setTable(i, k + 1, value, normalErrorT);
        }
        std::cout << std::endl;
    }

    std::cout << "Ошибка по " + netName.toStdString() + " сети: ";

    QString errorD = "Denorm: \n ";
    QString errorN = "Norm: \n ";
    double normE = 0;
    for (int i = 0; i < neironsCount; i++) {
        if (isResult) {

            if (!teachExamples.neironsToNextLevel[i]) {
                setTable(n, i + 1, 0, normalErrorT);
                setTable(n + 1, i + 1, 0, normalErrorT);
                double e = allNetsError->at(i);
                normE += e;
            } else {
                denorm[i] = sqrt(denorm[i] / n);
                errorD.append(QString::number(denorm[i]) + '\t');
                errorN.append(QString::number(sqrt(norm[i] / n)) + '\t');
                setTable(n, i + 1, norm[i], normalErrorT);
                setTable(n + 1, i + 1, denorm[i], normalErrorT);
                normE += norm[i];
            }
        } else {
            double e = norm[i];
            normE += e;
            errorD.append(QString::number(sqrt(denorm[i] / n)) + '\t');
            allNetsError->push_back(e);
            errorN.append(QString::number(sqrt(norm[i] / n)) + '\t');
            setTable(n, i + 1, norm[i], normalErrorT);
            setTable(n + 1, i + 1, denorm[i], normalErrorT);
        }

    }
    std::cout << errorD.toStdString() << std::endl << errorN.toStdString() << " " << std::endl;
    normalErrorPL->addWidget(normalErrorT);
    setTable(n, 0, "Норм. ошибка", normalErrorT);
    setTable(n + 1, 0, "Денорм. ошибка", normalErrorT);
    setTable(n + 2, 0, "Общ. норм. ошибка", normalErrorT);
    normE = sqrt(normE/(neironsCount * n));
    std::cout << "Общ. норм. ошибка: " << QString::number(normE).toStdString() << std::endl;
    setTable(n + 2, 1, normE, normalErrorT);
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



