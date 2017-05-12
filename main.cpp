#pragma once
#include <QtCore/QCoreApplication>
#include <QApplication>
#include <QHBoxLayout>
#include "earthWidget.h"
#include <QDialog>
#include <QMainWindow>
#include <QMimeData>
#include <QMdiSubWindow>
#include <QMdiArea>
#include "earthwnd.h"

int main(int argc, char *argv[])
{
	//QApplication::setStyle("cleanlooks");
	QApplication a(argc, argv);
// 	earthWidget *earth = new earthWidget;
// 	QMainWindow* mw = new QMainWindow();
// 	//mw->setWindowOpacity(1);
// 	//mw->setWindowFlags(Qt::FramelessWindowHint);
// 	//mw->setAttribute(Qt::WA_TranslucentBackground);
// 	mw->setWindowTitle("Osg Eearth");
// 	QMdiArea* mdiArea = new QMdiArea(mw);
// 	mw->setCentralWidget(mdiArea);
// 	QMdiSubWindow *subWindow = mdiArea->addSubWindow(earth);
// 	subWindow->showMaximized();
// 	subWindow->setWindowFlags(Qt::FramelessWindowHint);
// 	//subWindow->setWindowTitle("New Window");
// 	mw->show();
	earthWnd* wnd = new earthWnd();

	return a.exec();
}

