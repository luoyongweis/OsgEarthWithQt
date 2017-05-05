#ifndef DBSET_H
#define DBSET_H

#include <QDialog>

namespace Ui {
class dbset;
}

class dbset : public QDialog
{
    Q_OBJECT

public:
    explicit dbset(QWidget *parent = 0);
    ~dbset();

private slots:

    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::dbset *ui;
    int Interval;
signals:
	void signal_Interval(int);
};

#endif // DBSET_H
