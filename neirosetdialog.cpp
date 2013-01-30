#include "neirosetdialog.h"
#include "ui_neirosetdialog.h"
#include "iostream"
#include "sample.h"
#include "algorithm"
#include <iterator>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QDebug>

NeiroSetDialog::NeiroSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NeiroSetDialog)
{
    ui->setupUi(this);
    mainListView = ui->listWidget;
    enterListView = ui->listWidget_2;
    exitListView = ui->listWidget_3;
    radioButtonGroup.push_back(ui->radioButton);
    radioButtonGroup.push_back(ui->radioButton_2);

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
    std::vector<int> enterNum;
    std::vector<int> neironsNum;
    QMap<QString, int> enters;
    QMap<QString, int> neirons;
    Sample sample;
    Perceptron * newPerceptronNet;
    for (int i = 0; i < radioButtonGroup.size(); i++)
        if (((QRadioButton*)radioButtonGroup.at(i))->isChecked()) layerNum = i + 1;
    getColumns(&columnNum, &neironsNum, &enters, &neirons);
    perceptronContext->addEnters(enters);
    perceptronContext->addNeirons(neirons);
    sample.setEntersNum(columnNum);
    if (layerNum == 1) {        
        sample.setNeironsNum(neironsNum);
        sample.setEnterCount(columnNum.size());
        columnNum.insert(columnNum.end(), neironsNum.begin(), neironsNum.end());
        fileReader->getFirstLayerExams(columnNum, columnNum.size() - neironsNum.size(), &sample);
    } else if (layerNum == 2) {

        std::vector<int> firstLayerExamsIntersect = perceptronContext->getExamplesIntersect();
        std::vector<int> firstLayerNeironsNum = perceptronContext->getFirstLayerNets()->getNeironNum();
        fileReader->getSecondLayerExams(firstLayerExamsIntersect, firstLayerNeironsNum, columnNum, neironsNum, &sample);

    }

    newPerceptronNet = new Perceptron(sample, eraCount, alpha, velocity);

    perceptronContext->addNewNet(newPerceptronNet, layerNum);
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

void NeiroSetDialog::addDataToList(QListWidget *listView)
{
    QList<QListWidgetItem*> stringList = mainListView->selectedItems();
    QStringList list;
    foreach(QListWidgetItem * item1 , stringList)
    {
        bool result = true;
        foreach(QListWidgetItem * item2 , listView->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard))
            if (item1->text() == item2->text()) result = false;
        if (result) list.push_back(item1->text() );
    }
    listView->addItems(list);
}

void NeiroSetDialog::on_toolButton_clicked()
{
    addDataToList(enterListView);
}

void NeiroSetDialog::on_toolButton_3_clicked()
{
    addDataToList(exitListView);
}

void NeiroSetDialog::removeDataFromList(QListWidget *listView)
{
    foreach(QListWidgetItem * item, listView->selectedItems())
        delete item;

}

void NeiroSetDialog::on_toolButton_2_clicked()
{
    removeDataFromList(enterListView);
}

void NeiroSetDialog::on_toolButton_4_clicked()
{
    removeDataFromList(exitListView);
}


//создание списка номеров столбцов выборки, создание списка номеров нейронов
void NeiroSetDialog::getColumns(std::vector<int> * enterNum, std::vector<int> * neironsNum, QMap<QString, int> * enters, QMap<QString, int> * neirons)
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

    for (int i = 0; i < exitListView->count(); i++) {
        QString exitItemName = exitListView->item(i)->text();
        for (int j = 0; j < mainListView->count(); j++) {
            QString mainListItemName = mainListView->item(j)->text();
            if (exitItemName == mainListItemName ) {
                neirons->insert(mainListItemName, j);
                neironsNum->push_back(j);
                break;
            }
        }
    }

}
