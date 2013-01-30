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

private:
    Ui::NeiroSetDialog *ui;
    PerceptronContext * perceptronContext;
    FileReader * fileReader;
    QStringList titleList;
    QListWidget * mainListView;
    QListWidget * enterListView;
    QListWidget * exitListView;
    std::vector<QRadioButton*> radioButtonGroup;

private:
    void addDataToList(QListWidget * listView);
    void removeDataFromList(QListWidget * listView);
    void getColumns(std::vector<int> * columns, std::vector<int> * neironsNum, QMap<QString, int> *enters, QMap<QString, int> *neirons);

};

#endif // NEIROSETDIALOG_H
