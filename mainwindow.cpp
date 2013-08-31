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
#include <EnterRange.h>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>


std::string PERCEPTRON_COMPLEX_NULL = "Полностью настройте перцептронный комплекс! (создайте хотя бы одну сеть первого уровня и результирующую сеть)";
std::string FIRST_LAYER_NETS_SIZE_0 = "Создайте хотя бы одну сеть первого звена!";
std::string CHANGED_ENTER = "Изменяемый параметр";
std::string NEIRONS = "Выходные параметры";
std::string RANGE_START = "Начальное значение";
std::string RANGE_END = "Конечное значение";
std::string ENTER_RANGE_STEP = "Шаг";
int ENTER_RANGE_COLUMN_WIDTH = 200;



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
         std::vector<QStringList> dataList = fileReader->getDataFromFile(s, &fileReader->dataRows, &fileReader->minMaxForColumn);
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
    ui->entersRangeTW->clear();
    ui->entersRangeTW->setRowCount(0);
    ui->alphaParamsTbl_2->clear();
    ui->alphaParamsTbl_2->setRowCount(0);

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
        case 3:
            setRange();
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

void MainWindow::setRange()
{
    QList<QString> entersName = perceptronContext->getEntersName();
    if (entersName.size() != 0) {
        ui->entersRangeTW->setRowCount(entersName.size());
        for (int i = 0; i < ui->entersRangeTW->columnCount(); i++) {
            ui->entersRangeTW->setColumnWidth(i, ENTER_RANGE_COLUMN_WIDTH);
        }
        ui->entersRangeTW->setHorizontalHeaderItem(0,  new QTableWidgetItem(CHANGED_ENTER.c_str()));
        ui->entersRangeTW->setHorizontalHeaderItem(1,  new QTableWidgetItem(RANGE_START.c_str()));
        ui->entersRangeTW->setHorizontalHeaderItem(2,  new QTableWidgetItem(RANGE_END.c_str()));
        ui->entersRangeTW->setHorizontalHeaderItem(3,  new QTableWidgetItem(ENTER_RANGE_STEP.c_str()));
        for (int i = 0; i < entersName.size(); i++) {
            ui->entersRangeTW->setItem(i, 0, new QTableWidgetItem(entersName[i]));
        }
    }

    QList<QString> neironsName = perceptronContext->getNeironsName();
    if (neironsName.size() != 0) {
        ui->neironsRangeTW->setRowCount(neironsName.size());
        for (int i = 0; i < ui->neironsRangeTW->columnCount(); i++) {
            ui->neironsRangeTW->setColumnWidth(i, ENTER_RANGE_COLUMN_WIDTH);
        }
        ui->neironsRangeTW->setHorizontalHeaderItem(0,  new QTableWidgetItem(NEIRONS.c_str()));
        ui->neironsRangeTW->setHorizontalHeaderItem(1,  new QTableWidgetItem(RANGE_START.c_str()));
        ui->neironsRangeTW->setHorizontalHeaderItem(2,  new QTableWidgetItem(RANGE_END.c_str()));
        for (int i = 0; i < neironsName.size(); i++) {
            ui->neironsRangeTW->setItem(i, 0, new QTableWidgetItem(neironsName[i]));
        }
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

void MainWindow::on_chgAllNetsLW_2_itemClicked(QListWidgetItem *item)
{
    int row = chgAllNetsLW_2->row(item);
    Perceptron * net = ((row + 1) > perceptronContext->firstLayerNets->getNetsCount()) ? perceptronContext->resultPerceptron : perceptronContext->firstLayerNets->Nets[row];
    optimizeNet = net;
    ui->optVelosityDSB->setValue(net->velocity);
    ui->optEraSB->setValue(net->eraCount);
    std::cout << QString::number(row).toStdString() << std::endl;
}

void MainWindow::on_calcRangePB_clicked()
{
    QLabel * rangePrBInfoLab = ui->rangePrBInfoLab;
    QMap<int, QString> indStr;
    QTableWidget * table1 = ui->entersRangeTW;
    QTableWidget * table2 = ui->neironsRangeTW;
    std::vector<EnterRange> range;
    std::vector<QString> columnNames;
    for (int i = 0; i < table1->rowCount(); i++) {
        EnterRange e;
        e.paramTitle = table1->item(i, 0)->text();
        e.index = perceptronContext->enters[e.paramTitle];
        indStr[e.index] = e.paramTitle;
        e.startValue = table1->item(i, 1)->text().toDouble();
        e.endValue = table1->item(i, 2)->text().toDouble();
        e.step = table1->item(i, 3)->text().toDouble();
        range.push_back(e);
    }

    for (int i = 0; i < table2->rowCount(); i++) {
        EnterRange e;
        e.paramTitle = table2->item(i, 0)->text();
        e.index = -1;
        e.startValue = table2->item(i, 1)->text().toDouble();
        e.endValue = table2->item(i, 2)->text().toDouble();
        range.push_back(e);
    }

    calculatedRange.allRanges = perceptronContext->calcRange(range, &columnNames, rangePrBInfoLab, ui->saveRangePrB);
    for (int i = 0; i < table2->rowCount(); i++) {
        columnNames.push_back(table2->item(i, 0)->text());
    }


    calculatedRange.columnNames = columnNames;
}

QString MainWindow::saveFileDialog()
{
    QString fileName = QFileDialog::getSaveFileName().toUtf8();
    return fileName;
}

void MainWindow::on_pushButton_8_clicked()
{
    QString fileName = saveFileDialog();
    if (!fileName.isEmpty() && calculatedRange.columnNames.size() != 0)
        fileReader->writeRangesToFile(calculatedRange.allRanges, fileName,calculatedRange.columnNames, ui->saveRangePrB, ui->rangePrBInfoLab);
}

void MainWindow::on_pushButton_9_clicked()
{
    QString fileName = saveFileDialog();
    if (!fileName.isEmpty())
        fileReader->saveModel(perceptronContext, fileName);
}

void MainWindow::on_pushButton_10_clicked()
{
    QString fileName = openFileDialog();
    perceptronContext = fileReader->loadModel(fileName);
    setNetsList();
}

void MainWindow::on_pushButton_11_clicked()
{
    clearLayout(normalErrorPageL);
    QString fileName = openFileDialog();
    std::vector<Sample> samples = fileReader->getTeachSamples(perceptronContext, fileName);
    int count = 0;
    std::vector<std::vector<double> > netsErr;
    for (int j = 0; j < perceptronContext->firstLayerNets->Nets.size(); j++) {
        Perceptron * net = perceptronContext->firstLayerNets->Nets[j];
        net->calcError("net №" + QString::number(j + 1), ui->normalErrorPageL_3, false, &netsErr, count, samples.at(j));
        count += net->teachExamples.neironCount;
        std::cout << std::endl;
    }
    perceptronContext->firstLayerNets->setResultExamples(&samples);
    perceptronContext->resultPerceptron->calcError("result net", ui->normalErrorPageL_3,true,&netsErr, 0, samples.at(samples.size() - 1));

}
