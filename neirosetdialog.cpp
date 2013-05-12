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
    std::vector<int> enterNum;
    std::vector<int> neironsNum;
    std::vector<bool> neironsToNextLevel;
    QMap<QString, int> enters;
    QMap<QString, int> neirons;
    Sample sample;
    Perceptron * newPerceptronNet;
    for (int i = 0; i < radioButtonGroup.size(); i++)
        if (((QRadioButton*)radioButtonGroup.at(i))->isChecked()) layerNum = i + 1;
    getColumns(&columnNum, &neironsNum, &enters, &neirons, &neironsToNextLevel);
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

void NeiroSetDialog::addDataToList(QAbstractItemView *listView)
{
    QList<QListWidgetItem*> stringList = mainListView->selectedItems();
    QStringList list;

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

    if (listView == neironTableView) {
        foreach(QString enter , list) {
            neironTableView->setRowCount( neironTableView->rowCount() + 1);
            QCheckBox * check = new QCheckBox();
            check->setChecked(true);
            int rowNum = neironTableView->rowCount() - 1;
            neironTableView->setCellWidget(rowNum, 1, check);

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
        QList<QTableWidgetItem*> selectedItems = ((QTableWidget*)listView)->selectedItems();
        while (((QTableWidget*)listView)->selectedItems().size() != 0) {

            int t = ((QTableWidget*)listView)->row(((QTableWidget*)listView)->selectedItems().at(0));
            ((QTableWidget*)listView)->removeRow(t);

        }
//        ((QTableWidget*)listView)->selectedItems().removeAll(l(((QTableWidget*)listView)->selectedItems());
    }
//             ((QTableWidget*)listView)->row(
//             ((QTableWidget*)listView)->selecremoveRow(selectRow(rem
//        while (
//        (QTableWidget*)listView)->selectedItems().size()

//        for(int i = 0; i < ((QTableWidget*)listView)->selectedItems().size(); i++)
//            QTableWidgetItem * item, ((QTableWidget*)listView)->selectedItems())
//            delete item;

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
void NeiroSetDialog::getColumns(std::vector<int> * enterNum, std::vector<int> * neironsNum, QMap<QString, int> * enters, QMap<QString, int> * neirons, std::vector<bool> * neironsToNextLevel)
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
