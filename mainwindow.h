#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "menubar.h"
#include "toolbar.h"
#include "statusbar.h"
#include "widget.h"
#include "communication.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

signals:

public slots :
	void slot_sendCmd();
	//void slot_CommState(CommSet::CommState state);
	//void slot_sRec(QByteArray b);
	void slot_setDB();
	void slot_setDBInterval(int);

private:
	MenuBar *m_pMenuBar;
	ToolBar *m_pToolBar;
	StatusBar *m_pStatusBar;
	Widget *m_pWidget;
	Communication *m_pComm;
	QTimer *m_pTimer;
	ConnectDB *DBOperate;
	unsigned char m_id;
	int m_cmdCount;
};

#endif // MAINWINDOW_H
