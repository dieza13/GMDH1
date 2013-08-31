#include "neirosetdialog.h"
#include "ui_neirosetdialog.h"
#include "iostream"
#include "sample.h"
#include "algorithm"
#include <iterator>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QDebug>
#include <stdio.h>
#include <QDoubleSpinBox>

const std::string NEIRON_NAME = "Выход";
const std::string TO_NEXT_LAYER = "Вход для  \n следующего \n слоя";
//нормализация происходит от NORM_FROM до NORM_TO, параметры макс верхней и нежней границы в пределах [0,1];
const std::string NORM_FROM = "Нижняя норм. \n граница"; // нормализовать от ...
const std::string NORM_TO = "Верхняя норм. \n граница"; // нормализовать до ...
const int NEIRON_COLUMN_WID_1 = 70;
const int NEIRON_COLUMN_WID_2 = 110;
const int NEIRON_COLUMN_WID_3 = 120;
const int NEIRON_COLUMN_WID_4 = 120;
const double MIN_NORM_VAL = 0;
const double STEP_NORM_VAL = 0.01;
const double MAX_NORM_VAL = 1;
const int NEIRON_PARAM_COUNT = 4;

std::string headerTitle[] = {NEIRON_NAME, TO_NEXT_LAYER, NORM_FROM, NORM_TO};
int headerWid[] = {NEIRON_COLUMN_WID_1, NEIRON_COLUMN_WID_2, NEIRON_COLUMN_WID_3, NEIRON_COLUMN_WID_4};

NeiroSetDialog::NeiroSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NeiroSetDialog)
{
    ui->setupUi(this);
    mainListView = ui->listWidget;
    enterListView = ui->listWidget_2;
//    exitListView = ui->listWidget_3;
    radioButtonGroup.push_back(ui->radioButton);
    radioButtonGroup.push_back(ui->radioButton_2);
    neironTableView = ui->neirons;
    setLists();

//    neironTableView->

}

NeiroSetDialog::~NeiroSetDialog()
{
    delete ui;
}
//создание новой сети
void NeiroSetDialog::on_pushButton_clicked()
{
    int layerNum = 0;
    double velocity = ui->doubleSpinBox->value();
    double alpha = ui->doubleSpinBox_2->value();
    int eraCount = ui->spinBox->value();
    std::vector<int> columnNum;
    std::vector<int> neironsNum;
    std::vector<bool> neironsToNextLevel;
    QMap<QString, int> enters;
    QMap<QString, int> neirons;
    Sample sample;
    std::vector<std::vector<double> > minMaxNormNeironsValue;
    Perceptron * newPerceptronNet;
    for (int i = 0; i < radioButtonGroup.size(); i++)
        if (((QRadioButton*)radioButtonGroup.at(i))->isChecked()) layerNum = i + 1;
    getColumns(&minMaxNormNeironsValue, &columnNum, &neironsNum, &enters, &neirons, &neironsToNextLevel);
    perceptronContext->addEnters(enters);
    perceptronContext->addNeirons(neirons);
    sample.setEntersNum(columnNum);
    sample.setMinMaxNormNeironsValue(minMaxNormNeironsValue);
    if (layerNum == 1) {
        sample.setNeironsNum(neironsNum, neironsToNextLevel);
        sample.setEnterCount(columnNum.size());
        columnNum.insert(columnNum.end(), neironsNum.begin(), neironsNum.end());
        fileReader->getFirstLayerExams(&(fileReader->dataRows),columnNum, columnNum.size() - neironsNum.size(), &sample, true);
    } else if (layerNum == 2) {
        sample.setNeironsNum(neironsNum);
        std::vector<Sample> samples;
        for (int i = 0; i < perceptronContext->firstLayerNets->Nets.size(); i++) {
            Perceptron * net = perceptronContext->firstLayerNets->Nets[i];
            samples.push_back(net->teachExamples);
        }
        std::vector<int> firstLayerExamsIntersect = perceptronContext->getExamplesIntersect(samples);
        perceptronContext->getFirstLayerNets()->setResultSample(&sample);
        fileReader->getSecondLayerExams(&fileReader->dataRows, firstLayerExamsIntersect, columnNum, neironsNum, &sample, true);
    }

    newPerceptronNet = new Perceptron(sample, eraCount, alpha, velocity);
    if (layerNum == 1) {
        newPerceptronNet->teachExamples.entersName = enters.keys();
        newPerceptronNet->teachExamples.neironsName = neirons.keys();
    }


    perceptronContext->addNewNet(newPerceptronNet, layerNum);
    emit addNewNet();
    close();
}

void NeiroSetDialog::setParams(PerceptronContext * perceptronContext, FileReader * fileReader)
{
    this->perceptronContext = perceptronContext;
    this->fileReader = fileReader;
}



void NeiroSetDialog::setColumnsName(QStringList titleList)
{

    this->titleList = titleList;
    mainListView->addItems(titleList);

}

void NeiroSetDialog::addDataToList(QAbstractItemView *listView)
{
    QList<QListWidgetItem*> stringList = mainListView->selectedItems();
    QStringList list;
//установить компоненты для настройки параметров входов
    foreach(QListWidgetItem * item1 , stringList)
    {
        bool result = true;
        if (listView == neironTableView) {
            for (int i = 0; i <  ((QTableWidget*)listView)->rowCount(); i ++) {
                QTableWidgetItem * item2 = ((QTableWidget*)listView)->item(i, 0);
//            foreach(QTableWidgetItem * item2 , ((QTableWidget*)listView)->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard)){
                QString t1 = item1->text();
                QString t2 = item2->text();
                printf("asd %d",4);
                if (t1 == t2) result = false;
            }
        } else {
            foreach(QListWidgetItem * item2 , ((QListWidget*)listView)->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard))
                if (item1->text() == item2->text()) result = false;
        }
        if (result) {
            list.push_back(item1->text());
        }
    }
//установить компоненты для настройки параметров нейрона
    if (listView == neironTableView) {
        foreach(QString enter , list) {
            neironTableView->setRowCount( neironTableView->rowCount() + 1);
            QCheckBox * check = new QCheckBox();
            check->setChecked(true);
            int rowNum = neironTableView->rowCount() - 1;
            neironTableView->setCellWidget(rowNum, 1, check);
            QDoubleSpinBox * from = new QDoubleSpinBox();
            from->setMinimum(MIN_NORM_VAL);
            from->setMaximum(MAX_NORM_VAL);
            from->setValue(0);
            from->setSingleStep(STEP_NORM_VAL);
            QDoubleSpinBox * to = new QDoubleSpinBox();
            to->setMinimum(MIN_NORM_VAL);
            to->setMaximum(MAX_NORM_VAL);
            to->setValue(1);
            to->setSingleStep(STEP_NORM_VAL);
            neironTableView->setCellWidget(rowNum, 2, from);
            neironTableView->setCellWidget(rowNum, 3, to);
            QTableWidgetItem * item = new QTableWidgetItem(enter);
            neironTableView->setItem(rowNum, 0, new QTableWidgetItem(enter));
        }
    } else
        ((QListWidget*)listView)->addItems(list);
}

void NeiroSetDialog::on_toolButton_clicked()
{
    addDataToList(enterListView);
}

void NeiroSetDialog::on_toolButton_3_clicked()
{
    addDataToList(neironTableView);
}

void NeiroSetDialog::removeDataFromList(QAbstractItemView *listView)
{
    if (listView == neironTableView) {
        std::vector<QTableWidgetItem*> selectedItems = ((QTableWidget*)listView)->selectedItems();
        while (((QTableWidget*)listView)->selectedItems().size() != 0) {

            int t = ((QTableWidget*)listView)->row(((QTableWidget*)listView)->selectedItems().at(0));
            ((QTableWidget*)listView)->removeRow(t);

        }
    }

    if (listView == enterListView) {
        qDeleteAll(enterListView->selectedItems());
    }
}

void NeiroSetDialog::on_toolButton_2_clicked()
{
    removeDataFromList(enterListView);
}

void NeiroSetDialog::on_toolButton_4_clicked()
{
    removeDataFromList(neironTableView);
}


//создание списка номеров столбцов выборки, создание списка номеров нейронов
void NeiroSetDialog::getColumns(std::vector<std::vector<double> > * minMaxNormNeironsValue, std::vector<int> * enterNum, std::vector<int> * neironsNum, QMap<QString, int> * enters, QMap<QString, int> * neirons, std::vector<bool> * neironsToNextLevel)
{
    for (int i = 0; i < enterListView->count(); i++) {
        QString enterItemName = enterListView->item(i)->text();
        for (int j = 0; j < mainListView->count(); j++) {
            QString mainListItemName = mainListView->item(j)->text();
            if (enterItemName == mainListItemName) {
                enters->insert(mainListItemName, j);
                enterNum->push_back(j);
                break;
            }
        }
    }

    for (int i = 0; i < neironTableView->rowCount(); i++) {
        QString neironItemName = neironTableView->item(i, 0)->text();//exitListView->item(i)->text();
        QCheckBox* pCheckB(qobject_cast<QCheckBox*>(neironTableView->cellWidget(i, 1)));
        QDoubleSpinBox * from = (QDoubleSpinBox*)(neironTableView->cellWidget(i, 2));
        QDoubleSpinBox * to = (QDoubleSpinBox*)(neironTableView->cellWidget(i, 3));
        std::vector<double> neironNormParam;
        neironNormParam.push_back(from->value());
        neironNormParam.push_back(to->value());
        minMaxNormNeironsValue->push_back(neironNormParam);
        bool state =  pCheckB->isChecked();
        for (int j = 0; j < mainListView->count(); j++) {
            QString mainListItemName = mainListView->item(j)->text();
            if (neironItemName == mainListItemName ) {
                neirons->insert(mainListItemName, j);
                neironsToNextLevel->push_back(state);
                neironsNum->push_back(j);
                break;
            }
        }
    }

}

//настройка таблиц входов и выходов
void NeiroSetDialog::setLists()
{
    for (int i = 0; i < NEIRON_PARAM_COUNT; i++)
        setNeironsParam(i);
}

void NeiroSetDialog::setNeironsParam(int num)
{
    QTableWidgetItem * item = new QTableWidgetItem(headerTitle[num].c_str());
    neironTableView->setColumnWidth(num, headerWid[num]);
    neironTableView->setHorizontalHeaderItem(num, item);
}


