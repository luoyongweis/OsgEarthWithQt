#include "menubar.h"

MenuBar::MenuBar(QWidget *parent) : QMenuBar(parent)
	, m_pMenuSet(0)
	, m_pMenuTool(0)
	, m_pMenuHelp(0)
{
	m_pMenuSet = new QMenu(tr("Set"));
	m_pMenuTool = new QMenu(tr("Tool"));
	m_pMenuHelp = new QMenu(tr("Help"));
	m_pMenuMapSet = new QMenu(tr(""));

	m_pMapSet = new QAction(tr("Map Set"), this);
	m_pMapSatellite = new QAction(tr("Satellite"), this);
	m_pMapHybrid = new QAction(tr("Hybrid"), this);
	m_pCommSet = new QAction(tr("COMM Set"), this);

	m_pMenuSet->addAction(m_pCommSet);
	m_pMenuSet->addAction(m_pMapSet);
	
	m_pstartDB = new QAction(tr("DB Start"), this);
	m_psetDB = new QAction(tr("DB Set"), this);
	m_pMenuTool->addAction(m_pstartDB);
	m_pMenuTool->addAction(m_psetDB);
	
	m_pMenuMapSet->addAction(m_pMapSatellite);
	m_pMenuMapSet->addAction(m_pMapHybrid);

	m_pMapSet->setMenu(m_pMenuMapSet);

	this->addMenu(m_pMenuSet);
	this->addMenu(m_pMenuTool);
	this->addMenu(m_pMenuHelp);

	connect(m_pCommSet, SIGNAL(triggered()), this, SIGNAL(signal_setComm()));
	connect(m_pMapSatellite, SIGNAL(triggered()), this, SIGNAL(signal_setMapSatellite()));
	connect(m_pMapHybrid, SIGNAL(triggered()), this, SIGNAL(signal_setMapHybrid()));
	connect(m_psetDB, SIGNAL(triggered()), this, SIGNAL(signal_setDB()));
	connect(m_pstartDB, SIGNAL(triggered()), this, SIGNAL(signal_startDB()));
}

MenuBar::~MenuBar()
{
	delete m_pMenuSet;
	delete m_pMenuTool;
	delete m_pMenuHelp;
}

