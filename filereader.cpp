#include "filereader.h"
#include <QFile>
#include <qstringlist.h>
#include "QRegExp"
#include "vector"
#include <sample.h>
#include "iostream"
#include <stdio.h>
#define INCORRECT_VALUE -10000000000



FileReader::FileReader()
{
    columnNum = 0;
}

std::vector<QStringList> FileReader::getDataFromFile(QString string)
{
    std::vector<QStringList> data;
    QFile file(string);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString content = file.readLine();
    QStringList list = content.split(QRegExp("(\\s+)|(\\t+)"), QString::SkipEmptyParts);
    data.push_back(list);
    while (!file.atEnd()) {
        QString content = file.readLine();
        QStringList list = content.split(QRegExp("(\\s+)|(\\t+)"), QString::SkipEmptyParts);
        double * params = new double[list.size()];
        if (list.size() > columnNum) columnNum = list.size();
        for (int i = 0; i < list.size(); i++) {
            QString s = list.at(i);
            if (s.compare("") == 0) {
                params[i] = INCORRECT_VALUE;
                continue;
            }
            if (checkIfNumber(s) ) params[i] = s.toDouble();
            else params[i] = INCORRECT_VALUE;
        }
        dataRows.push_back(params);
        data.push_back(list);
    }
    normalizeData();
    return data;
}


void FileReader::getFirstLayerExams(std::vector<int> columnsNum, int enterCount, Sample *sample)
{

    std::vector<double*> columns;
    std::vector<int> examplesNum;
    std::vector<std::vector<double> > minMaxValueForNeirons;
    std::vector<std::vector<double> > minMaxValueForEnters;
    for (int i = 0; i < dataRows.size() - 1; i++) {
        bool fullResult = true;
        double * row = new double[columnsNum.size()];
        int k = 0;
        for (int j = 0; j < columnsNum.size(); j++) {
            double num = dataRows[i][columnsNum[j]];
            row[k] = num;
            if (row[k] == INCORRECT_VALUE) {
                fullResult = false;
                break;
            }
            k++;
        }

        if (fullResult) {
            columns.push_back(row);
            examplesNum.push_back(i);

        }
    }

    for (int i = 0; i < enterCount; i++) {
        minMaxValueForEnters.push_back(minMaxForColumn[columnsNum[i]]);
    }

    for (int i = enterCount; i < columnsNum.size(); i++) {
        minMaxValueForNeirons.push_back(minMaxForColumn[columnsNum[i]]);
    }

    sample->setMinMaxEntersValue(minMaxValueForEnters);
    sample->setMinMaxNeironsValue(minMaxValueForNeirons);
    sample->setExamplesNum(examplesNum);
    sample->setExamples(columns);
}



bool FileReader::checkIfNumber(const QString &s)
{
    QRegExp re("(-?[0-9]+[.,][0-9]+)|[0-9]+");
    return re.exactMatch(s);
}

//нормализация исходной выборки
void FileReader::normalizeData()
{
    for (int i = 0; i < columnNum; i++) {
        double max = -100000000, min = 100000000;
        for (int j = 0; j < dataRows.size(); j++) {
            if (dataRows[j][i] < min && dataRows[j][i] != INCORRECT_VALUE) min = dataRows[j][i];
            if (dataRows[j][i] > max) max = dataRows[j][i];
        }
        for (int j = 0; j < dataRows.size(); j++) {
            if (dataRows[j][i] != INCORRECT_VALUE) {
                double value = (dataRows[j][i] - min) / (max - min);
                dataRows[j][i] = value;
            } else {
                double value = dataRows[j][i];
                dataRows[j][i] = value;
            }
        }
        std::vector<double> minMax;
        minMax.push_back(min);
        minMax.push_back(max);
        minMaxForColumn.push_back(minMax);
    }
}

void FileReader::getSecondLayerExams(std::vector<int> exampleNums, std::vector<int> neironNums, std::vector<int> newEnters, std::vector<int> newNeirons, Sample * sample)
{    
    std::vector<int> resultNeirons;
    std::vector<int> newExampleNums;
    getResultNeironNums(&neironNums, &newNeirons, &resultNeirons);
    if (sample->getEntersNum()->size() != 0) {
        sample->getEntersNum()->insert(sample->getEntersNum()->end(), neironNums.begin(), neironNums.end());        
    } else {
        sample->setEntersNum(neironNums);
    }
    sample->setEnterCount(sample->getEntersNum()->size());

    std::vector<double*> resultExams;
    std::vector<std::vector<double> > minMaxValueForColumn;
    std::vector<std::vector<double> > minMaxValueForEnters;
    int examLen = neironNums.size() + newEnters.size() + resultNeirons.size();
    for (int i = 0; i < exampleNums.size(); i++) {

        double * fullExam = dataRows[exampleNums[i]];

        //определение,является ли пример корректным относительно новых входов
        if (!isCorrect(&newEnters, fullExam)) continue;

        //определение,является ли пример корректным относительно новых выходов
        if (!isCorrect(&newNeirons, fullExam)) continue;

        newExampleNums.push_back(exampleNums[i]);
        //помещение в пример результирующей выборки значений новых добавленных входов
        double * example = new double[examLen];
        for (int j = 0; j < newEnters.size(); j++) {
            int posOfNewEnter = neironNums.size() + j;
            int indOfVal = newEnters[j];         
            example[posOfNewEnter] = fullExam[indOfVal];
        }
        //помещение в пример результирующей выборки значений результирующих выходов
        for (int j = 0; j < resultNeirons.size(); j++) {
            int neironPos = neironNums.size() + newEnters.size() + j;
            example[neironPos] = fullExam[resultNeirons[j]];
        }

        resultExams.push_back(example);
//        for ( int j = 0; j < examLen; j ++) {
//            std::cout << resultExams[resultExams.size() - 1][j] << ' ';
//        }
//        std::cout <<std::endl;
    }

    //получение значений min/ max для результирующих выходов
    for (int j = 0; j < resultNeirons.size(); j++) {
        minMaxValueForColumn.push_back(minMaxForColumn[resultNeirons[j]]);
    }
    //получение значений min/ max для результирующих выходов
    for (int j = 0; j < newEnters.size(); j++) {
        minMaxValueForEnters.push_back(minMaxForColumn[newEnters[j]]);
    }

    sample->setMinMaxEntersValue(minMaxValueForEnters);
    sample->setMinMaxNeironsValue(minMaxValueForColumn);
    sample->setExamplesNum(newExampleNums);
    sample->setExamples(resultExams);
    sample->setNeironsNum(resultNeirons);
    sample->setEnterCount(neironNums.size() + newEnters.size());
    for (int i = 0; i < sample->getExamplesCount(); i++) {
        double * example = sample->getExamples()[i];
        for (int j = 0; j < examLen; j++) {

            if (j < examLen - sample->getEnterCount() + 1)
            /*std::cout << */printf("%4.5f ", example[j]) /*<< ' '*/;
            else
                printf("%4.5f ", example[j] * (sample->getNeironMaxValue(j - sample->getEnterCount()) - sample->getNeironMinValue(j - sample->getEnterCount())) + sample->getNeironMinValue(j - sample->getEnterCount()));
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

//поиск номеров нейронов результирующей выборки
void FileReader::getResultNeironNums(std::vector<int> * neironNums, std::vector<int> * newNeironNums, std::vector<int> * resultNeironNums)
{
    for (int i = 0; i < newNeironNums->size(); i++) {
        int num = newNeironNums->at(i);
        resultNeironNums->push_back(num);
    }
    for (int i = 0; i < neironNums->size(); i++) {
        int num = neironNums->at(i);
        bool isRepeat = false;
        for (int j = 0; j < resultNeironNums->size(); j++) {
            if (num == resultNeironNums->at(j)) {
                isRepeat = true;
                break;
            }
        }
        if (!isRepeat) resultNeironNums->push_back(num);
    }
}


bool FileReader::isCorrect(std::vector<int> * checkedConteiner, double * example)
{
    for (int j = 0; j < checkedConteiner->size(); j++) {
        if (example[checkedConteiner->at(j)] == INCORRECT_VALUE) {
            return false;
        }
    }
    return true;
}

