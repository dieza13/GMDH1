#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QLineEdit"
#include "QRegExp"
#include "QTableWidget"
#include "QComboBox"
#include "QFileDialog"
#include "filereader.h"
#include "iostream"
#include "neirosetdialog.h"
#include "perceptroncontext.h"
#include <algorithm>
#include <ctime>
#include <QHBoxLayout>
#include <neironbody.h>
#include <perceptronbody.h>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QMap>
#include <getexmpledialog.h>
#include <QGraphicsItem>

std::string PERCEPTRON_COMPLEX_NULL = "Полностью настройте перцептронный комплекс! (создайте хотя бы одну сеть первого уровня и результирующую сеть)";
std::string FIRST_LAYER_NETS_SIZE_0 = "Создайте хотя бы одну сеть первого звена!";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    perceptronContext = new PerceptronContext();
    chgAllNetsLW = ui->chgAllNetsLW;
    chgAllNetsLW_2 = ui->chgAllNetsLW_2;
    connect(chgAllNetsLW, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(showNetParam(QListWidgetItem *)));
    normalErrorPageL = ui->normalErrorPageL_3;
    enters = ui->Enters;
    resultValues = ui->ResultValues;
    QColor color();
    ui->textBrowser->setTextColor(QColor( "red" ));
    alphaParams = ui->alphaParamsTbl_2;
    alphaTable = ui->alphaTbl;

}

MainWindow::~MainWindow()
{
    delete fileReader;
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
     fileReader = new FileReader();
     QString s = openFileDialog();
     if (!s.isEmpty()) {
         std::vector<QStringList> dataList = fileReader->getDataFromFile(s);
         setData(dataList);
     }
}


QString MainWindow::openFileDialog()
{
    QString fileName = QFileDialog::getOpenFileName().toUtf8();
    return fileName;
}

void MainWindow::setData(std::vector<QStringList> dataList)
{
    QTableWidget * table = ui->tableWidget;
    QStringList list = dataList.at(1);
    table->setColumnCount(list.size());
    table->setRowCount(dataList.size() - 2);

    for (int i = 0; i < dataList.size(); i++) {
        QStringList row = dataList.at(i);
        for (int j = 0; j < row.size(); j++) {
            if (i == 0) table->setHorizontalHeaderItem(j,  new QTableWidgetItem(row.at(j)));
            else table->setItem(i - 1, j, new QTableWidgetItem(row.at(j)));

        }
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->textBrowser->clear();
    QStringList titleList;
    NeiroSetDialog * neiroDialog = new NeiroSetDialog();

    QObject::connect(neiroDialog, SIGNAL(addNewNet()),
                     this, SLOT(setNetsList()));

    QTableWidget * table = ui->tableWidget;
    for (int i = 0; i < table->columnCount(); i++) titleList.push_back(table->horizontalHeaderItem(i)->text());

    neiroDialog->setColumnsName(titleList);
    neiroDialog->setParams(perceptronContext, fileReader);
    neiroDialog->show();
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->textBrowser->clear();
    chgAllNetsLW->clear();
    chgAllNetsLW_2->clear();
    clearLayout(normalErrorPageL);
    for (int i = enters->rowCount() - 1; i >= 0; i--) {
        enters->removeRow(i);
    }

    for (int i = resultValues->rowCount() - 1; i >= 0; i--) {
        resultValues->removeRow(i);
    }

    delete perceptronContext;
    enters->setRowCount(0);
    perceptronContext = new PerceptronContext();
    delete ui->graphicsView->scene();
    perceptronContext->resultPerceptron = NULL;
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->textBrowser->clear();
    if (perceptronContext->firstLayerNets != NULL && perceptronContext->resultPerceptron != NULL) {
        if (perceptronContext->firstLayerNets->getNetsCount() == 0) {
            ui->textBrowser->setText(trUtf8(FIRST_LAYER_NETS_SIZE_0.c_str()));


        } else {
            clearLayout(normalErrorPageL);
            perceptronContext->teachNets(normalErrorPageL);
            setResultTable();
        }

    } else {
        ui->textBrowser->setText(trUtf8(PERCEPTRON_COMPLEX_NULL.c_str()));
    }

}





void MainWindow::on_pushButton_5_clicked()
{
    QGraphicsScene * scene = perceptronContext->getStructureVisualObj();

    ui->graphicsView->setScene(scene);


}

void MainWindow::on_pushButton_6_clicked()
{

    QMap<QString, int> enters = perceptronContext->getEnters();
    QTableWidget * entersTable = ui->Enters;
    bool isNumeric = true;

    for (int i = 0; i < entersTable->rowCount(); i++) {
        QTableWidgetItem * item = entersTable->item(i, 0);

        item->text().toDouble(&isNumeric);
        if (!isNumeric) {
            ui->textBrowser->setText("Введено не числовое значение. проверьте данные");
            return;
        }
    }

    QMap<int, double> example;
    for (int i = 0; i < entersTable->rowCount(); i++) {
        QTableWidgetItem * item = entersTable->item(i, 0);
        QTableWidgetItem * headerItem = entersTable->verticalHeaderItem(i);
        example.insert(enters[headerItem->text()], item->text().toDouble());
    }

    QMapIterator<int, double> iter(example);
    while (iter.hasNext()) {
         iter.next();
//         std::cout << iter.key() << ' ' << iter.value()  <<  std::endl;
    }

    QMap<QString, double> results = perceptronContext->getResults(example);
    QTableWidget * resultValuesTable = ui->ResultValues;
    for (int i = 0; i < resultValuesTable->rowCount(); i++) {
        QTableWidgetItem * headerItem = resultValuesTable->verticalHeaderItem(i);
        double value = results[headerItem->text()];
        QString val = QString::number(value);
        QTableWidgetItem * item = new QTableWidgetItem(val);
        resultValuesTable->setItem(i, 0, item);


    }



}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    switch (index) {
        case 2:
            if (enters->rowCount() == 0)
                setResultTable();
            break;
    }


}

void MainWindow::setResultTable()
{
    QList<QString> entersName = perceptronContext->getEntersName();
    enters->setRowCount(entersName.size());
    int i = 0;
    foreach(QString name, entersName) {
        QTableWidgetItem * header = new QTableWidgetItem(name);
        header->setForeground(Qt::black);
        header->setBackground(QColor("lightblue"));
        header->setFont(QFont("arial", 12));
        enters->setVerticalHeaderItem(i, header);
        i++;
    }



    QList<QString> neironsName = perceptronContext->getNeironsName();
    resultValues->setRowCount(neironsName.size());
    i = 0;
    foreach(QString name, neironsName) {
        QTableWidgetItem * header = new QTableWidgetItem(name);
        std::cout << name.toStdString() << std::endl;
        header->setForeground(Qt::black);
        header->setBackground(QColor("lightblue"));
        header->setFont(QFont("arial", 12));
        resultValues->setVerticalHeaderItem(i, header);
        i++;
    }

    QTableWidgetItem * header = new QTableWidgetItem("Значение");
    header->setForeground(Qt::black);
    header->setBackground(QColor("lightblue"));
    header->setFont(QFont("arial", 12));
    enters->setHorizontalHeaderItem(0, header);
    QTableWidgetItem * header1 = new QTableWidgetItem("Значение");
    header1->setForeground(Qt::black);
    header1->setBackground(QColor("lightblue"));
    header1->setFont(QFont("arial", 12));
    resultValues->setHorizontalHeaderItem(0, header1);

}

void MainWindow::clearLayout(QLayout *layout, bool deleteWidgets)
{
    while (QLayoutItem* item = layout->takeAt(0))
    {
        if (deleteWidgets)
        {
            if (QWidget* widget = item->widget())
                delete widget;
        }
        else if (QLayout* childLayout = item->layout())
            clearLayout(childLayout, deleteWidgets);
        delete item;
    }
}

void MainWindow::addAlphaParamWidget(Perceptron * net)
{
    QList<QString> neironNames = net->teachExamples.neironsName;

    alphaParams->clear();
    alphaParams->setRowCount( net->neirons.size());
    for (int i = 0; i < net->neirons.size(); i++) {
        Neiron * neiron = net->neirons.at(i);

        QDoubleSpinBox * spinBox = new QDoubleSpinBox();
        spinBox->setValue(neiron->alpha);
        alphaParams->setCellWidget(i, 1, spinBox);

        QTableWidgetItem * item = new QTableWidgetItem(neironNames[i]);
        alphaParams->setItem(i, 0, item);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{


    QString cbStr;
    QClipboard *cb = QApplication::clipboard();
    QModelIndexList list =  ui->tableWidget->selectionModel()->selectedIndexes();
    int i, j, firstRow, lastRow, rowCount;

   if( list.isEmpty() ) return;

       firstRow = list.first().row();  lastRow = list.last().row();
       rowCount = lastRow - firstRow + 1;

     for(i = 0; i < rowCount; ++i, cbStr += '\n')
      for(j = i; j < list.count(); j += rowCount, cbStr += '\t')
        cbStr += ui->tableWidget->model()->data(list[ j ], Qt::EditRole).toString();

     cb->setText(cbStr);
}


void MainWindow::setNetsList()
{
    QListWidget * lw = ui->chgAllNetsLW;
    QListWidget * lw2 = chgAllNetsLW_2;
    QStringList netsList;
    for (int i = 0; i < perceptronContext->firstLayerNets->getNetsCount(); i++) {
        netsList.append("Сеть " + QString::number(i + 1) + " первого слоя");
    }

    if (perceptronContext->resultPerceptron != NULL) {
        netsList.append("Результирующая сеть");
    }
    lw->clear();
    lw->addItems(netsList);
    lw2->clear();
    lw2->addItems(netsList);
}

void MainWindow::showNetParam(QListWidgetItem * item)
{
    int row = chgAllNetsLW->row(item);
    Perceptron * net = ((row + 1) > perceptronContext->firstLayerNets->getNetsCount()) ? perceptronContext->resultPerceptron : perceptronContext->firstLayerNets->Nets[row];

    addAlphaParamWidget(net);
    changedNet = net;
    ui->chgEraDSB_3->setValue(net->eraCount);
    ui->chgVelosityDSB_3->setValue(net->velocity);

    std::cout << QString::number(row).toStdString() << std::endl;
}



void MainWindow::on_chgNetBTN_clicked()
{
    int netNum = -1;
    int size = ui->chgAllNetsLW->count();
    for (int i = 0; i < size; i++) {
        if ( ui->chgAllNetsLW->item(i)->isSelected()) {
            netNum = i;
        }
    }
    Perceptron * net = NULL;
    if (netNum > (perceptronContext->firstLayerNets->getNetsCount() - 1))
        net = perceptronContext->resultPerceptron;
    else {
        net = perceptronContext->firstLayerNets->Nets[netNum];
    }

    if (net != NULL) {
        for(int i = 0; i < net->neirons.size(); i++) {
            QDoubleSpinBox * doubleSpinBox(qobject_cast<QDoubleSpinBox*>(alphaParams->cellWidget(i, 1)));
            double value = doubleSpinBox->value();

            ((Neiron*)net->neirons[i])->alpha = value;
        }

        net->velocity = ui->chgVelosityDSB_3->value();
        net->eraCount = ui->chgEraDSB_3->value();
    }
}



void MainWindow::on_tableWidget_itemSelectionChanged()
{
    QString cbStr;
    QClipboard *cb = QApplication::clipboard();
    QModelIndexList list =  ui->tableWidget->selectionModel()->selectedIndexes();
    int i, j, firstRow, lastRow, rowCount;
    if( list.isEmpty() ) return;
    firstRow = list.first().row();  lastRow = list.last().row();
    rowCount = lastRow - firstRow + 1;
    for(i = 0; i < rowCount; ++i, cbStr += '\n')
        for(j = i; j < list.count(); j += rowCount, cbStr += '\t')
            cbStr += ui->tableWidget->model()->data(list[ j ], Qt::EditRole).toString();
    cb->setText(cbStr);
}



void MainWindow::on_pushButton_7_clicked()
{
    int row = chgAllNetsLW_2->row(chgAllNetsLW_2->selectedItems().at(0));
    double startAlpha = ui->optStartDSB->value();
    double endAlpha = ui->optEndDSB->value();
    double stepAlpha = ui->optStepDSB->value();
    double velocity = ui->optVelosityDSB->value();
    int eraCount = ui->optEraSB->value();
    int optimizeParam = ui->optParamSB->value();

    Perceptron * net = (row < perceptronContext->firstLayerNets->Nets.size()) ? perceptronContext->firstLayerNets->Nets[row] : perceptronContext->resultPerceptron;
    net->velocity = velocity;
    net->eraCount = eraCount;

    alphaTable->clear();
    alphaTable->setRowCount(net->neirons.size());

    for (int i = 0; i < net->neirons.size(); i++) {
        QString neironName = net->teachExamples.neironsName[i];
        alphaTable->setItem(i, 0, new QTableWidgetItem(neironName));
    }
    std::vector<double> alphas = perceptronContext->optimizeAlpha(startAlpha,endAlpha,stepAlpha,optimizeParam,row);
    for (int i = 0; i < alphas.size(); i++) {
        alphaTable->setItem(i, 1, new QTableWidgetItem(QString::number(alphas.at(i))));

    }


}






void MainWindow::on_chgNetBTN_2_clicked()
{

}

void MainWindow::on_chgAllNetsLW_2_itemClicked(QListWidgetItem *item)
{
    int row = chgAllNetsLW_2->row(item);
    Perceptron * net = ((row + 1) > perceptronContext->firstLayerNets->getNetsCount()) ? perceptronContext->resultPerceptron : perceptronContext->firstLayerNets->Nets[row];

    optimizeNet = net;

   /* ui->optEndDSB->setValue(1.5);
    ui->optStartDSB->setValue(1.5)*/;
    ui->optVelosityDSB->setValue(net->velocity);
    ui->optEraSB->setValue(net->eraCount);
    std::cout << QString::number(row).toStdString() << std::endl;

}
