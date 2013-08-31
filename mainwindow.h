#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<vector>
#include "filereader.h"
#include "perceptroncontext.h"
#include <calculatedrange.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    QString openFileDialog();
    QString saveFileDialog();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_tabWidget_currentChanged(int index);


    void setNetsList();

    void showNetParam(QListWidgetItem * item);

    void on_chgNetBTN_clicked();



    void on_tableWidget_itemSelectionChanged();


    void on_pushButton_7_clicked();

    void on_chgAllNetsLW_2_itemClicked(QListWidgetItem *item);

    void on_calcRangePB_clicked();


    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

signals:



private:
    void clearLayout();
    Ui::MainWindow *ui;
    FileReader *  fileReader;
    PerceptronContext * perceptronContext;
    QVBoxLayout * normalErrorPageL;
    QVBoxLayout * denormalErrorPageL;
    QTableWidget * enters;
    QTableWidget * resultValues;
private:
    void keyPressEvent(QKeyEvent * event);
    void setData(std::vector<QStringList> dataList);
    void setResultTable();
    void clearLayout(QLayout* layout, bool deleteWidgets = true);
    void addAlphaParamWidget(Perceptron * net);
    void setRange();


public:

public:
    QListWidget * chgAllNetsLW;
    QListWidget * chgAllNetsLW_2;
    QTableWidget * alphaParams;
    Perceptron * changedNet;
    Perceptron * optimizeNet;
    QTableWidget * alphaTable;
    CalculatedRange calculatedRange;
};



#endif // MAINWINDOW_H
