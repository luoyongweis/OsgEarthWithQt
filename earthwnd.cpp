#include "earthwnd.h"
#include <QApplication>

earthWnd::earthWnd(QWidget *parent)
	: QWidget(parent)
{
	mainLayout = new QVBoxLayout;
	createToolBars();
	createWidgets();

	mainLayout->addLayout(tooBarLayout);
	mainLayout->addWidget(earth);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(mainLayout);
	setFixedSize(1366, 768);
	connectWidgets();
	show();
}

earthWnd::~earthWnd()
{
	
}

void earthWnd::createToolBars()
{
	tooBarLayout = new QHBoxLayout;
	isfollow = new QCheckBox("Follow"); 
	tooBarLayout->addWidget(isfollow);

	addbtn = new QPushButton(tr("+"));
	subbtn = new QPushButton(tr("-"));
	playbtn = new QPushButton(tr("P"));
	tooBarLayout->addWidget(addbtn);
	tooBarLayout->addWidget(subbtn);
	tooBarLayout->addWidget(playbtn);
	tooBarLayout->addStretch();
}

void earthWnd::createWidgets()
{
	QApplication::setStyle("cleanlooks");
	earth = new earthWidget;
	setWindowTitle("Osg Eearth");
	mdiArea = new QMdiArea(this);


	QMdiSubWindow *subWindow = mdiArea->addSubWindow(earth);
	//subWindow->setFixedSize(1000, 680);
	subWindow->setWindowFlags(Qt::FramelessWindowHint);

}

void earthWnd::connectWidgets()
{
	connect(isfollow, SIGNAL(clicked(bool)), earth, SLOT(slot_isFollowPlane(bool)));
	connect(addbtn, SIGNAL(clicked()), earth, SLOT(slot_addInterval()));
	connect(subbtn, SIGNAL(clicked()), earth, SLOT(slot_subInterval()));
	connect(playbtn, SIGNAL(clicked()), earth, SLOT(slot_startPlay()));
}

void earthWnd::updatePlanePosition(double _long, double _lat, double _altitude)
{

}

