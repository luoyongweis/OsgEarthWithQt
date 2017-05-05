#include "mainwindow.h"
#include "connectdb.h"
#include "dbset.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
, m_pMenuBar(0)
, m_pToolBar(0)
, m_pWidget(0)
, m_pStatusBar(0)
, m_pComm(0)
, m_pTimer(0)
{
	m_pMenuBar = new MenuBar;
	m_pToolBar = new ToolBar(this);
	m_pStatusBar = new StatusBar;
	m_pWidget = new Widget;
	m_pComm = new Communication(this);
	DBOperate = new ConnectDB("localhost", "postgres", "111", "dbtest", 5432);

	this->setMenuBar(m_pMenuBar);
	this->addToolBar(m_pToolBar);
	this->setStatusBar(m_pStatusBar);
	this->setCentralWidget(m_pWidget);

	connect(m_pMenuBar, SIGNAL(signal_setMapSatellite()), m_pWidget, SLOT(slot_setMapSatellite()));
	connect(m_pMenuBar, SIGNAL(signal_setMapHybrid()), m_pWidget, SLOT(slot_setMapHybrid()));
	connect(m_pToolBar, SIGNAL(signal_testMapLatLng(double, double)), m_pWidget, SLOT(slot_MapCreateWp(double, double)));
	connect(m_pToolBar, SIGNAL(signal_testMapClear()), m_pWidget, SLOT(slot_MapClearWp()));
	connect(m_pMenuBar, SIGNAL(signal_setComm()), m_pComm, SLOT(slot_setComm()));
	connect(m_pComm, SIGNAL(signal_commType(CommSet::CommType)), m_pStatusBar, SLOT(slot_CommType(CommSet::CommType)));
	connect(m_pComm, SIGNAL(signal_commState(CommSet::CommState)), m_pStatusBar, SLOT(slot_CommState(CommSet::CommState)));
	connect(m_pComm, SIGNAL(signal_readValueId(int)), m_pWidget, SLOT(slot_wellData(int)));
	connect(m_pComm, SIGNAL(signal_readStateId(int)), m_pWidget, SLOT(slot_wellState(int)));
	connect(m_pComm, SIGNAL(signal_commState(CommSet::CommState)), this, SLOT(slot_CommState(CommSet::CommState)));
	connect(m_pMenuBar, SIGNAL(signal_startDB()), DBOperate, SLOT(slot_startDB()));
	connect(m_pMenuBar, SIGNAL(signal_setDB()), this, SLOT(slot_setDB()));
	//connect(m_pComm, SIGNAL(signal_sRec(QByteArray)), this, SLOT(slot_sRec(QByteArray)));
}

MainWindow::~MainWindow()
{
	delete m_pMenuBar;
	delete m_pToolBar;
	delete m_pStatusBar;
	delete m_pWidget;
	delete m_pComm;
}




