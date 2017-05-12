#ifndef EARTHWND_H
#define EARTHWND_H

#include <QMainWindow>
#include <QtCore/QCoreApplication>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include "earthWidget.h"
#include <QDialog>
#include <QMimeData>
#include <QMdiSubWindow>
#include <QMdiArea>
#include <QCheckBox>
#include <QToolBar>
#include <QRadioButton>
#include <QPushButton>

class earthWnd : public QWidget
{
	Q_OBJECT

public:
	earthWnd(QWidget *parent = NULL);
	~earthWnd();
	void updatePlanePosition(double _long, double _lat, double _altitude);

protected:
	void createToolBars();
	void createWidgets();
	void connectWidgets();
private:
	
	earthWidget *earth;
	QMdiArea* mdiArea;
	QToolBar* toolBar;
	QCheckBox* isfollow;
	QHBoxLayout*tooBarLayout;
	QVBoxLayout * mainLayout;
	QPushButton* addbtn, *subbtn, *playbtn;
public slots:
	
signals:
	//void stateChanged(int state);
};

#endif // EARTHWND_H
