#ifndef NEIROSETDIALOG_H
#define NEIROSETDIALOG_H

#include <QDialog>
#include <filereader.h>
#include <QLineEdit>
#include "perceptroncontext.h"
#include "QListView"
#include "QListWidget"
#include "QRadioButton"

namespace Ui {
class NeiroSetDialog;
}

class NeiroSetDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NeiroSetDialog(QWidget *parent = 0);
    ~NeiroSetDialog();
public:
    void setParams(PerceptronContext * perceptronContext, FileReader * fileReader);
    void setColumnsName(QStringList titleList);
    
private slots:
    void on_pushButton_clicked();

    void on_toolButton_clicked();

    void on_toolButton_3_clicked();

    void on_toolButton_2_clicked();

    void on_toolButton_4_clicked();

signals:
    void addNewNet();
private:
    Ui::NeiroSetDialog *ui;
    PerceptronContext * perceptronContext;
    FileReader * fileReader;
    QStringList titleList;
    QListWidget * mainListView;
    QListWidget * enterListView;
//    QListWidget * exitListView;
    std::vector<QRadioButton*> radioButtonGroup;
    QTableWidget * neironTableView;

private:
    void addDataToList(QAbstractItemView * listView);
    void removeDataFromList(QAbstractItemView * listView);
    void getColumns(std::vector<std::vector<double> > * minMaxNormNeironsValue, std::vector<int> * columns, std::vector<int> * neironsNum, QMap<QString, int> *enters, QMap<QString, int> *neironTableView, std::vector<bool> * neironsToNextLevel);
    void setLists();
    void setNeironsParam(int num);
};

#endif // NEIROSETDIALOG_H
