#ifndef GETEXMPLEDIALOG_H
#define GETEXMPLEDIALOG_H

#include <QDialog>
#include <QList>
#include <QTableWidget>
#include <perceptroncontext.h>


namespace Ui {
class GetExmpleDialog;
}

class GetExmpleDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit GetExmpleDialog(QWidget *parent = 0);
    ~GetExmpleDialog();
    
public:
    void setParamsNameList(PerceptronContext * perceptronContext);
private:
    Ui::GetExmpleDialog *ui;
    QTableWidget * paramsValue;
    PerceptronContext * perceptronContext;

private:
};

#endif // GETEXMPLEDIALOG_H
