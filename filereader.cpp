#include "filereader.h"
#include <QFile>
#include <qstringlist.h>
#include "QRegExp"
#include "vector"
#include <sample.h>
#include "iostream"
#include <stdio.h>
#include <QMapIterator>

#define INCORRECT_VALUE -10000000000

std::string SAVING_RANGE_LABEL = "Сохранение диапазона...";
std::string SAVED_RANGE_LABEL = "Диапазон сохранен.";
std::string PERCEPTRON_MODEL = "-PerceptronModel-";
std::string NEIRON_NAMES = "NEIRON_NAMES";
std::string ENTER_NAMES = "ENTER_NAMES";
std::string NET = "--NET--\n";
std::string ALPHA = "ALPHA";
std::string VELOCITY = "VELOCITY";
std::string ERA_COUNT = "ERA_COUNT";
std::string SAMPLE = "---SAMPLE---";
std::string NEIRON = "---NEIRON---";
std::string RESULT_PERCEPTRON = "--RESULT_PERCEPTRON--\n";


FileReader::FileReader()
{
    columnNum = 0;
}

FileReader::~FileReader()
{
    for (int i = 0; i < dataRows.size(); i++)
        delete dataRows[i];
    for (int i = 0; i < normalizeDataRows.size(); i++)
        delete normalizeDataRows[i];
}

std::vector<QStringList> FileReader::getDataFromFile(QString string,  std::vector<double *> * dataRows, std::vector<std::vector<double> > * minMaxForColumn)
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
            if (checkIfNumber(s) ) {
                double d = s.toDouble();
                params[i] = d;
            }
            else params[i] = INCORRECT_VALUE;
        }
        dataRows->push_back(params);
        data.push_back(list);
    }
    normalizeData(dataRows, minMaxForColumn);
    file.close();
    return data;
}

void FileReader::getFirstLayerExams(std::vector<double*> * dataRows, std::vector<int> columnsNum, int enterCount, Sample *sample, bool needSetMinMax)
{
    std::vector<double*> columns;
    std::vector<int> examplesNum;    
    for (int i = 0; i < dataRows->size(); i++) {
        bool fullResult = true;
        double * row = new double[columnsNum.size()];
        int k = 0;
        for (int j = 0; j < columnsNum.size(); j++) {
            row[k] = dataRows->at(i)[columnsNum[j]];
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

    if (needSetMinMax) {
        std::vector<std::vector<double> > minMaxValueForNeirons;
        std::vector<std::vector<double> > minMaxValueForEnters;
        for (int i = 0; i < enterCount; i++) {
            minMaxValueForEnters.push_back(minMaxForColumn[columnsNum[i]]);
        }
        for (int i = enterCount; i < columnsNum.size(); i++) {
            minMaxValueForNeirons.push_back(minMaxForColumn[columnsNum[i]]);
        }
        sample->setMinMaxEntersValue(minMaxValueForEnters);
        sample->setMinMaxNeironsValue(minMaxValueForNeirons);
    }
    sample->setExamplesNum(examplesNum);
    sample->setExamples(columns);
}

bool FileReader::checkIfNumber(const QString &s)
{
    QRegExp re("(-?[0-9]+[.,][0-9]+)|(-?[0-9]+[.,][0-9]+[Ee][+-][0-9]+)|[0-9]+");
    bool isNumber;
    isNumber = re.exactMatch(s);
    return isNumber;
}

//нормализация исходной выборки
void FileReader::normalizeData(std::vector<double *> * dataRows, std::vector<std::vector<double> > * minMaxForColumn/*, std::vector<std::vector<double> > * minMaxNormForColumn*/)
{
    for (int i = 0; i < columnNum; i++) {
        double max = -100000000, min = 100000000;
        for (int j = 0; j < dataRows->size(); j++) {
            if (dataRows->at(j)[i] < min && dataRows->at(j)[i] != INCORRECT_VALUE) min = dataRows->at(j)[i];
            if (dataRows->at(j)[i] > max) max = dataRows->at(j)[i];
        }
        for (int j = 0; j < dataRows->size(); j++) {
            if (dataRows->at(j)[i] != INCORRECT_VALUE) {
                double value = (dataRows->at(j)[i] - min) / (max - min);
                dataRows->at(j)[i] = value;
            } else {
                double value = dataRows->at(j)[i];
                dataRows->at(j)[i] = value;
            }
        }
        std::vector<double> minMax;
        minMax.push_back(min);
        minMax.push_back(max);
        minMaxForColumn->push_back(minMax);
    }
}

void FileReader::getSecondLayerExams(std::vector<double *> *dataRows, std::vector<int> exampleNums, std::vector<int> newEnters, std::vector<int> newNeirons, Sample * sample, bool needMinMax)
{    
    std::vector<int> newExampleNums;
    std::vector<double*> resultExams;    
    int examLen =sample->enterCount + sample->neironCount;
    for (int i = 0; i < exampleNums.size(); i++) {
        std::cout << i;
        std::cout << std::endl;
        double * fullExam = dataRows->at(exampleNums[i]);
        //определение,является ли пример корректным относительно новых входов
        if (!isCorrect(&newEnters, fullExam)) continue;
        //определение,является ли пример корректным относительно новых выходов
        if (!isCorrect(&newNeirons, fullExam)) continue;
        newExampleNums.push_back(exampleNums[i]);
        //помещение в пример результирующей выборки значений новых добавленных входов
        double * example = new double[examLen];
        for (int j = 0; j < newEnters.size(); j++) {
            int posOfNewEnter = sample->neironsNum.size() + j;
            int indOfVal = newEnters[j];         
            example[posOfNewEnter] = fullExam[indOfVal];
        }
        //помещение в пример результирующей выборки значений результирующих выходов
        for (int j = 0; j < sample->neironCount; j++) {
            int neironPos = sample->enterCount + j;
            double value = fullExam[sample->neironsNum[j]];
            example[neironPos] = value;
        }
        resultExams.push_back(example);
    }
    if (needMinMax) {
        std::vector<std::vector<double> > minMaxValueForColumn;
        std::vector<std::vector<double> > minMaxNormValueForColumn;
        std::vector<std::vector<double> > minMaxValueForEnters;
        //получение значений min/ max для результирующих выходов
        for (int j = 0; j < sample->neironCount; j++) {
            minMaxValueForColumn.push_back(minMaxForColumn[sample->neironsNum[j]]);

        }
        //получение значений min/ max для результирующих выходов
        for (int j = 0; j < newEnters.size(); j++) {
            minMaxValueForEnters.push_back(minMaxForColumn[newEnters[j]]);
        }

        sample->setMinMaxEntersValue(minMaxValueForEnters);
        sample->setMinMaxNeironsValue(minMaxValueForColumn);
    }
    sample->setExamplesNum(newExampleNums);
    sample->setExamples(resultExams);
    for (int i = 0; i < sample->getExamplesCount(); i++) {
        double * example = sample->getExamples()[i];
        for (int j = 0; j < examLen; j++) {
             printf("%4.5f ", example[j]);
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

std::string FileReader::writeVecOfVec(std::vector<std::vector<double> > vec)
{
    QString string = "";
    for (int i = 0; i < vec.size(); i++) {
        std::vector<double> mmNeiron = vec[i];
        for (int j = 0; j < mmNeiron.size(); j++) {
            string += QString::number((double)mmNeiron.at(j)) + "\t";
        }
    }
    return string.toStdString();
}

std::vector<std::vector<double> > FileReader::readVecOfVec(QFile * file)
{
    QString string = file->readLine();
    QStringList list = string.split(QRegExp("[\t\n]"), QString::SkipEmptyParts);
    std::vector<std::vector<double> > values;
    for (int i = 0; i < list.size() / 2; i++) {
        std::vector<double> vector;
        double min = list.at(2 * i).toDouble();
        double max = list.at(2 * i + 1).toDouble();
        vector.push_back(min);
        vector.push_back(max);
        values.push_back(vector);
    }
    return values;
}

std::string FileReader::writeList(QList<QString> l)
{
    std::string string = "";
    for (int i = 0; i < l.size(); i++) {
        string += l.at(i).toStdString() + "\t";
    }
    return string;
}

QList<QString> FileReader::readList(QFile *file)
{
    QString string = file->readLine();
    return string.split(QRegExp("[\t\n]"), QString::SkipEmptyParts);
}

std::string FileReader::writePerceptron(Perceptron *net, std::string name)
{
    std::string string = name;
    string += ALPHA + "\t" + QString::number(net->alpha).toStdString() + "\t";
    string += VELOCITY + "\t" + QString::number(net->velocity).toStdString() + "\t";
    string += ERA_COUNT + "\t" + QString::number(net->eraCount).toStdString() + "\t";
    string += "\n";
    string += SAMPLE + "\n";
    Sample * sample = &net->teachExamples;
    string += writeList(sample->entersName) + "\n";
    string += writeList(sample->neironsName) + "\n";
    string += writeVecOfVec(sample->minMaxEntersValue);
    string += "\n";
    string += writeVecOfVec(sample->minMaxNeironsValue);
    string += "\n";
    string += writeVec(sample->entersNum);
    string += "\n";
    string += writeVec(sample->neironsNum);
    string += "\n";
    string += writeVec(sample->neironsToNextLevel);
    string += "\n";
    string += QString::number(net->neirons.size()).toStdString() + "\n";
    for (int j = 0; j < net->neirons.size(); j++) {
        string += NEIRON + "\n";
        Neiron * neiron = net->neirons[j];
        string += QString::number(neiron->neironNum).toStdString() + "\t" + QString::number(neiron->enterCount).toStdString() + "\t" + QString::number(neiron->alpha).toStdString() + "\t" + QString::number(neiron->velocity).toStdString() + "\n";
        string += QString::number(neiron->minVal).toStdString() + "\t" + QString::number(neiron->maxVal).toStdString() + "\n";
        for (int k = 0; k < neiron->enterCount + 1; k++) {
            string += QString::number(neiron->weight[k]).toStdString() + "\t";
        }
        string += "\n";
    }
    return string;
}

Perceptron * FileReader::readPerceptron(QFile *file)
{
    QString string = file->readLine();
    QStringList list = string.split(QRegExp("[\t\n]"),QString::SkipEmptyParts);
    double alpha = list.at(1).toDouble();
    double velocity = list.at(3).toDouble();
    int eraCount = list.at(5).toDouble();
    file->readLine();
    QList<QString> entersName = readList(file);
    QList<QString> neironsName = readList(file);
    std::vector<std::vector<double> > minMaxNeironsValue;
    std::vector<std::vector<double> > minMaxEntersValue;

    minMaxEntersValue = readVecOfVec(file);
    minMaxNeironsValue = readVecOfVec(file);
    std::vector<int> entersNum = readVec<int>(file);
    std::vector<int> neironsNum = readVec<int>(file);
    std::vector<bool> neironsToNextLevel = readVec<bool>(file);
    string = file->readLine();
    int neironCount = string.toInt();
    Perceptron * newNet = new Perceptron();
    Sample sample;
    sample.enterCount = entersNum.size();
    sample.entersNum = entersNum;
    sample.entersName = entersName;
    sample.neironCount = neironsNum.size();
    sample.neironsNum = neironsNum;
    sample.neironsName = neironsName;
    sample.minMaxEntersValue = minMaxEntersValue;
    sample.minMaxNeironsValue = minMaxNeironsValue;
    sample.neironsToNextLevel = neironsToNextLevel;
    newNet->teachExamples = sample;
    newNet->alpha = alpha; newNet->velocity = velocity; newNet->eraCount = eraCount;
    for (int i = 0; i < neironCount; i++) {
        file->readLine();
        string = file->readLine();
        QStringList l = string.split(QRegExp("[\t\n]"),QString::SkipEmptyParts);
        Neiron * neiron = new Neiron(l.at(1).toInt(), l.at(3).toDouble(), l.at(2).toDouble(),l.at(0).toInt());
        string = file->readLine();
        l = string.split(QRegExp("[\t\n]"),QString::SkipEmptyParts);
        neiron->minVal = l.at(0).toDouble();
        neiron->maxVal = l.at(1).toDouble();
        string = file->readLine();
        l = string.split(QRegExp("[\t\n]"),QString::SkipEmptyParts);
        neiron->weight = new double[l.size()];
        for (int i = 0; i < l.size(); i++) {
            neiron->weight[i] = l.at(i).toDouble();
        }
        newNet->neirons.push_back(neiron);
    }
    return newNet;
}

std::vector<Sample> FileReader::getTeachSamples(PerceptronContext *model, QString fileName)
{
    std::vector<Sample> samples;
    std::vector<double *> dataRows;
    std::vector<std::vector<double> > minMaxForColumn;
    QStringList titles = getDataFromFile(fileName, &dataRows, &minMaxForColumn).at(0);
    int columnCount = titles.size();

    for (int i = 0; i < model->firstLayerNets->Nets.size(); i++) {
        Perceptron * net = model->firstLayerNets->Nets[i];
        Sample sample = net->teachExamples;
        std::vector<int> columnsNum(sample.entersNum);
        columnsNum.insert(columnsNum.end(), sample.neironsNum.begin(), sample.neironsNum.end());
        getFirstLayerExams(&dataRows, columnsNum, sample.enterCount, &sample, false);
        samples.push_back(sample);
    }   
    std::vector<int> firstLayerIntersect = model->getExamplesIntersect(samples);
    Sample resultSample = model->resultPerceptron->teachExamples;
    std::vector<int> newEnters;
    std::vector<int> newNeirons;
    getSecondLayerExams(&dataRows, firstLayerIntersect, newEnters, newNeirons, &resultSample, false);
    samples.push_back(resultSample);
    return samples;

}

//рассчет диапазона(один из входных параметров изменяется в указанных пределах с указанным шагом, остальные принимают фиксированное, заданное значение)
void FileReader::writeRangesToFile(std::vector<std::vector<double> > ranges, QString fileName, std::vector<QString> columnNames, QProgressBar * saveRangePrB, QLabel *saveLabel)
{
    saveLabel->setText(QString::fromStdString(SAVING_RANGE_LABEL));
    saveRangePrB->setMaximum(ranges.size());
    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QString title = "";
    for (int j = 0; j < columnNames.size(); j++) {
        title += columnNames[j] + "\t";
    }
    title += "\n";
    file.write(title.toStdString().c_str());
    for (int i = 0; i < ranges.size(); i++) {
        QString string = "";
        std::vector<double> range = ranges[i];
        for (int k = 0; k < range.size(); k++) {
            string += QString::number(range[k]) + "\t";
        }
        string += "\n";
        file.write(string.toStdString().c_str());
        saveRangePrB->setValue(i + 1);
    }
    saveLabel->setText(QString::fromStdString(SAVED_RANGE_LABEL));
    file.close();
}

void FileReader::saveModel(PerceptronContext * model, QString fileName)
{
    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(PERCEPTRON_MODEL.c_str());
    QMap<QString, int> enters = model->enters;
    QMap<QString, int> neirons = model->neirons;
    std::string string = "";
    QMapIterator<QString, int> iter1(enters);
    string += "\n" + ENTER_NAMES + "\n";
    while (iter1.hasNext()) {
        QString name = iter1.next().key();
        int num = iter1.value();
        string += name.toStdString() + "\t" + QString::number(num).toStdString() + "\t";
    }
    string += "\n";
    QMapIterator<QString, int> iter2(neirons);
    string += NEIRON_NAMES + "\n";
    while (iter2.hasNext()) {
        QString name = iter2.next().key();
        int num = iter2.value();
        string += name.toStdString() + "\t" + QString::number(num).toStdString() + "\t";
    }
    string += "\n";
    file.write(string.c_str());
    string = "";
    for(int i = 0; i < model->firstLayerNets->Nets.size(); i++) {
        string += "\n";
        Perceptron * net = model->firstLayerNets->Nets[i];
        string = writePerceptron(net, NET);
        file.write(string.c_str());
    }
    string = "";
    string = writePerceptron(model->resultPerceptron, RESULT_PERCEPTRON).c_str();
    file.write(string.c_str());


    file.close();

}

PerceptronContext * FileReader::loadModel(QString fileName)
{
    PerceptronContext * model = new PerceptronContext();
    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    file.readLine();
    file.readLine();
    QString string = file.readLine();
    QMap<QString, int> enters = parseMap<QString, int>(string);
    file.readLine();
    string = file.readLine();
    QMap<QString, int> neirons = parseMap<QString, int>(string);
    model->neirons = neirons;
    model->enters = enters;
    model->firstLayerNets = new FirstLayerNets();
    while (!file.atEnd()) {
        string = file.readLine();
        if (string.compare(QString::fromStdString(NET)) == 0)
            model->firstLayerNets->addNewNet(readPerceptron(&file));
        else if (string.compare(QString::fromStdString(RESULT_PERCEPTRON)) == 0)
            model->resultPerceptron = readPerceptron(&file);
    }
    file.close();


    return model;
}


template<typename T> std::string FileReader::writeVec(std::vector<T> vector)
{
    QString string = "";
    for (int i = 0; i < vector.size(); i++) {
        string += QString::number(vector[i]) + "\t";
    }
    return string.toStdString();
}


template <typename S, typename V>  QMap<S, V> FileReader::parseMap(QString string)
{
    QMap<S, V> map;
    QStringList splitline = string.split(QRegExp("[\t\n]"),QString::SkipEmptyParts);
    for (int i = 0; i < splitline.size() / 2; i++) {
        S name = splitline[i * 2];
        V value = (V)splitline.at(i * 2 + 1).toDouble();
        map[name] = value;
    }
    return map;
}


template<typename T> std::vector<T> FileReader::readVec(QFile *file)
{
    QString string = file->readLine();
    QStringList list = string.split(QRegExp("[\t\n]"),QString::SkipEmptyParts);
    std::vector<T> vector;
    for (int i = 0; i < list.size(); i++) {
        vector.push_back(list.at(i).toDouble());
    }
    return vector;
}
