#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<vector>
#include "filereader.h"
#include "perceptroncontext.h"

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
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_pushButton_7_clicked();

    void setNetsList();

    void showNetParam(QListWidgetItem * item);

    void on_chgNetBTN_clicked();

signals:



private:
    Ui::MainWindow *ui;
    FileReader *  fileReader;
    PerceptronContext * perceptronContext;
private:
    void setData(std::vector<QStringList> dataList);
    void setResultTable();
public:

public:
    QListWidget * chgAllNetsLW;
};

#endif // MAINWINDOW_H
