#include "dbset.h"
#include "ui_dbset.h"

dbset::dbset(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dbset)
{
    ui->setupUi(this);
}

dbset::~dbset()
{
    delete ui;
}



void dbset::on_okButton_clicked()
{
    Interval = ui->lineEdit->text().toInt();
	emit signal_Interval(Interval);
    this->close();
}

void dbset::on_cancelButton_clicked()
{
    this->close();
}
