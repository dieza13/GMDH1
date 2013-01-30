#include "getexmpledialog.h"
#include "ui_getexmpledialog.h"

GetExmpleDialog::GetExmpleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GetExmpleDialog)
{
    ui->setupUi(this);
    paramsValue = ui->tableWidget;

}

GetExmpleDialog::~GetExmpleDialog()
{
    delete ui;
}

void GetExmpleDialog::setParamsNameList(PerceptronContext *perceptronContext)
{

    this->perceptronContext = perceptronContext;
    QList<QString> entersName = perceptronContext->getEntersName();
    paramsValue->setRowCount(entersName.size());
    int i = 0;
    foreach(QString name, entersName) {
        QTableWidgetItem * header = new QTableWidgetItem(name);
        header->setForeground(Qt::black);
        header->setBackground(QColor("lightblue"));
        header->setFont(QFont("arial", 12));
        paramsValue->setVerticalHeaderItem(i, header);
        i++;
    }
    QTableWidgetItem * header = new QTableWidgetItem("Значение");
    header->setForeground(Qt::black);
    header->setBackground(QColor("lightblue"));
    header->setFont(QFont("arial", 12));
    paramsValue->setHorizontalHeaderItem(0, header);
}
