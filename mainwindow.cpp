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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    perceptronContext = new PerceptronContext();
}

MainWindow::~MainWindow()
{
    delete fileReader;
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
     fileReader = new FileReader();
     std::vector<QStringList> dataList = fileReader->getDataFromFile(openFileDialog());
     setData(dataList);

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
    QStringList titleList;
    NeiroSetDialog * neiroDialog = new NeiroSetDialog();
    QTableWidget * table = ui->tableWidget;
    for (int i = 0; i < table->columnCount(); i++) titleList.push_back(table->horizontalHeaderItem(i)->text());

    neiroDialog->setColumnsName(titleList);
    neiroDialog->setParams(perceptronContext, fileReader);
    neiroDialog->show();
}

void MainWindow::on_pushButton_3_clicked()
{


   QMap<QString, int> map;
   QString a("a");
   QString b("c");
   QString c("b");
   map[a] = 1;
   map[b] = 2;
   map[c] = 3;
   std::cout << map["a"] << std::endl << map["b"] << std::endl << map["c"] << std::endl<< map["d"] << map["e"] <<std::endl;


}

void MainWindow::on_pushButton_4_clicked()
{
    perceptronContext->teachNets();
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
            break;
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
        QTableWidget * resultTable = ui->Enters;
        if (resultTable->rowCount() == 0)
            setResultTable();
        break;
    }


}

void MainWindow::setResultTable()
{
    QTableWidget * resultTable = ui->Enters;
    QList<QString> entersName = perceptronContext->getEntersName();
    resultTable->setRowCount(entersName.size());
    int i = 0;
    foreach(QString name, entersName) {
        QTableWidgetItem * header = new QTableWidgetItem(name);
        header->setForeground(Qt::black);
        header->setBackground(QColor("lightblue"));
        header->setFont(QFont("arial", 12));
        resultTable->setVerticalHeaderItem(i, header);
        i++;
    }



    QTableWidget * resultValues = ui->ResultValues;
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
    resultTable->setHorizontalHeaderItem(0, header);
    resultValues->setHorizontalHeaderItem(0, header);

}


