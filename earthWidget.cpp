#include "earthWidget.h"
#include <osgEarthDrivers/model_feature_geom/FeatureGeomModelOptions>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
#include <osgEarth/GeoData>
#include <QDateTime>
#include <QString>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/ExampleResources>
#include <osgEarthAnnotation/ImageOverlay>
#include <osgEarthAnnotation/CircleNode>
#include <osgEarthAnnotation/RectangleNode>
#include <osgEarthAnnotation/EllipseNode>
#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthAnnotation/LabelNode>
#include <osgEarthAnnotation/LocalGeometryNode>
#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthAnnotation/AnnotationEditing>
#include <osgEarthAnnotation/ImageOverlayEditor>
#include <osgEarthSymbology/GeometryFactory>

#include <osgEarthUtil/MouseCoordsTool>

#include "pickEventHandler.h"

double earthRotationDegree = -170;
int earthWidget::_count = 0;
int earthWidget::mInterval = 0;



earthWidget::earthWidget(QWidget* parent) : AdapterWidget(parent)
{
	//setCursor(Qt::DragMoveCursor);
	initViewer();
	initEarthWidget();
	setEarthManipulator();
	creatCanvas();
	createLabels();
	connectWidgets();
	addEventHandlers();
	earthRun();
	//test();
}

earthWidget::~earthWidget()
{
}

void earthWidget::initViewer()
{
	/*_view = new osgViewer::Viewer;*/
	_view = this;
	osg::notify(osg::FATAL);
	//int xoffset = 40;
	//int yoffset = 40;
	//osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
	//traits->x = xoffset + 0;
	//traits->y = yoffset + 0;
	//traits->width = 800;
	//traits->height = 680;
	//traits->windowDecoration = true;		//窗口装饰开关
	//traits->doubleBuffer = true;
	//traits->sharedContext = 0;
	//osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
	//
	//osg::ref_ptr<osg::Camera> camera = new osg::Camera;
	
	osg::DisplaySettings::instance()->setNumMultiSamples(16);
	osg::ref_ptr<osg::Viewport> viewPort = new osg::Viewport(0, 0, width(), height());
	_view->getCamera()->setViewport(viewPort);

	//创建对称透视投影
	_view->getCamera()->setProjectionMatrixAsPerspective(30.0f,
		static_cast<double>(width()) / static_cast<double>(height()), 1.0f, 10000.0f);
	_view->getCamera()->setGraphicsContext(getGraphicsWindow());
										    setThreadingModel(osgViewer::ViewerBase::SingleThreaded);
	_view->getCamera()->setNearFarRatio(0.0000001);
	_view->getCamera()->setComputeNearFarMode(osg::CullSettings::COMPUTE_NEAR_FAR_USING_BOUNDING_VOLUMES);//);
	
	//connect(&_timer, SIGNAL(timeout()), this, SLOT(updateGL()));
	//_timer.start(10);

}

void earthWidget::initEarthWidget()
{
	_reveTime = 0;
	_J2000TimeOffset = 0.0;
	_SimStartTime = 0.0;

	_root = new osg::Group();
	

	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("./myearth.earth");
	_pMapNode = osgEarth::MapNode::get(node);
	_pMapNode->setName("earth");

	osgEarth::Util::SkyOptions kk;
	kk.ambient() = 0.20f;

	_sky = osgEarth::Util::SkyNode::create(kk, _pMapNode);
	if (_sky)
	{
		_sky->attach(_view, 0);
	}

	m_pMapSRS = _pMapNode->getMapSRS();			/**地固坐标系*/
	m_pGeoSRS = m_pMapSRS->getGeographicSRS();	/**大地坐标系*/

	//std::cout << "getMapSRS : " << m_pMapSRS->getEllipsoid()->getName() << "getGeographicSRS : " << m_pGeoSRS->getEllipsoid()->getName() << std::endl;

	//_pRoot = new osg::Group;
	double time = 0;
	osgEarth::DateTime dataTime(time);

	_sky->setLighting(true);
	osgEarth::Util::Ephemeris* eph = new osgEarth::Util::Ephemeris;
	_sky->setEphemeris(eph);
	_sky->setDateTime(dataTime);
	_sky->setMoonVisible(true);
	//#_sky->setMinimumAmbient(osg::Vec4(0.3, 0.3, 0.3,1));
	_sky->attach(_view);

	_sky->addChild(_pMapNode.get());

	//_root->addChild(_pMapNode);
	_root->addChild(_sky.get());
	

	//添加轨道父矩阵
	_sateOrbitParentMatrix = new osg::MatrixTransform;
	_root->addChild(_sateOrbitParentMatrix);
	//添加链路父矩阵
	_linkParentMatrix = new osg::MatrixTransform;
	_root->addChild(_linkParentMatrix);
	//_cameraTrans = new osg::MatrixTransform;
	_sateListMatrix = new osg::MatrixTransform;
	//_sateListMatrix->addChild(_cameraTrans);
	_root->addChild(_sateListMatrix);

	//抗锯齿
	osg::Multisample* pms=new osg::Multisample; 
	pms->setSampleCoverage(1,true);// [i] 属性设置对了吗？[/i] 
	_root->getOrCreateStateSet()->setMode(GL_MULTISAMPLE_ARB, true);
	_root->getOrCreateStateSet()->setAttributeAndModes(pms, osg::StateAttribute::ON);


	osg::ref_ptr<const osgEarth::SpatialReference> pGeoSRS = _pMapNode->getMapSRS()->getGeographicSRS();
	osgEarth::GeoPoint geoEarthPt = osgEarth::GeoPoint(pGeoSRS, 112.0, 32.0, 0.0);


	osg::Vec3d earthEndPos;
	geoEarthPt.toWorld(earthEndPos);

	osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
	mt->setMatrix(osg::Matrix::translate(earthEndPos));
	//mt->setUpdateCallback(new osg::AnimationPathCallback(earthEndPos,osg::Z_AXIS, osg::inDegrees(45.0)));
	_root->addChild(mt);

	setStartDateTime(dataTime);

	addLabel();
	
}

void earthWidget::addEventHandlers()
{
	////鼠标位置信息显示
	//osgEarth::Util::Formatter* formatter = new osgEarth::Util::LatLongFormatter();
	//osgEarth::Util::LabelControl* readout = new osgEarth::Util::LabelControl();
	//_root->addChild(readout);
	//osgEarth::Util::ControlCanvas::get(_view)->addControl(readout);
	//osgEarth::Util::MouseCoordsTool* tool = new osgEarth::Util::MouseCoordsTool(_pMapNode);
	//tool->addCallback(new osgEarth::Util::MouseCoordsLabelCallback(readout, formatter));
	//_view->addEventHandler(tool);
	//// add the state manipulator
	//_view->addEventHandler(new osgGA::StateSetManipulator(_view->getCamera()->getOrCreateStateSet()));
	_view->addEventHandler(new PickHandle(m_pMapSRS));
	_view->addEventHandler(new osgViewer::StatsHandler);
	_view->addEventHandler(new osgViewer::WindowSizeHandler);
	_view->addEventHandler(new osgViewer::ThreadingHandler());
	_view->addEventHandler(new osgViewer::LODScaleHandler());
}

void earthWidget::earthRun()
{
	_view->setSceneData(_root.get());

	//osgUtil::Optimizer optimizer;
	//optimizer.optimize(_root.get());
	//_view->setSceneData(_root.get());
	//_view->realize();
	//this->show();
}

void earthWidget::paintGL()
{
	_osgLock.lock();
	updateScene();
	frame();

	/*if (_bRealease)
	{
	release();
	_bRealease = false;

	}*/
	update();
	_osgLock.unlock();
}

void earthWidget::updateScene()
{
	updateSceneTime();
}

void earthWidget::updateSceneTime()
{
	osgEarth::DateTime tt(QDateTime::currentDateTime().toTime_t()/* + 8 * 60 * 60*/ + (_count++)*mInterval);

	

	_sky->setDateTime(tt);
	osgEarth::DateTime date = _sky->getDateTime();
	//std::cout << "DateTime:" <<date.year()<<" "<<date.month()<<" "<<date.day()<<" "<<date.hours() << endl;
	double deltaS = floor(float(tt.asTimeStamp() - _reveTime));
	earthRotationDegree = (deltaS)*   0.00416666666;
	

	//地球自转RsR2D
	_pEarthMan->setByMatrix(_pEarthMan->getMatrix()*osg::Matrix::rotate(osg::inDegrees(-earthRotationDegree), osg::Z_AXIS));
	//std::cout << "earthRotationDegree : " << earthRotationDegree << "  deltaS : " << deltaS <<
	//	"  reveTime : " << _reveTime << " ttTime : " << tt.asTimeStamp() << endl;
	/*if (_coordinateSystem == Coordinate_J2000)
	{
		FixedToJ2000(earthRotationDegree);
		std::cout << "FixedToJ2000 ---> earthRotationDegree" << earthRotationDegree << " " << date.year() << " " << date.month() << " " << date.day() << " " << date.hours() <<
		" "<<tt.year()<<" "<<tt.month()<<" "<<tt.day()<<" "<<tt.hours()<<_sateListMatrix->getMatrix().getTrans()[0]<<
		" " << _sateListMatrix->getMatrix().getTrans()[1] << " " << _sateListMatrix->getMatrix().getTrans()[2] << endl;
	}*/

	QString time;
	time += "Earth Intertial Axes\n\n";
	time += QDateTime::fromTime_t(QDateTime::currentDateTime().toTime_t()).toString("yyyy-MM-dd hh:mm:ss");
	time += " ";
	time += " time step: ";
	time += QString::number(000);
	time += " sec";
	m_timeInfo->printText(time);
	_reveTime = tt.asTimeStamp();
}

void earthWidget::setStartDateTime(const osgEarth::DateTime &dt)
{
	INT64 time = (dt.asTimeStamp() + 8 * 60 * 60);
	osgEarth::DateTime ddt(2000, 1, 1, 12);        //2015/06/01 --5  254.99675728823058    //2015/09/01 --5  344.99675728823058
	double J2000Time = ddt.asTimeStamp() + 60 * 60;   //2015/05/01 --7  224.99677519220859 2015/04/01 -- 9 194.99679252016358   
	_J2000TimeOffset = time - J2000Time;          //2015/03/01 -- 11 164.99681042414159    2015/02/01 -- 13 134.99682660005055
	osgEarth::DateTime dat(time);
	QDateTime qTime;
	osgEarth::DateTime baseTime(2005, 9, 24, 0);
	std::cout << baseTime.year() << " " << baseTime.month() << " " << baseTime.day() << endl;
	osgEarth::DateTime curTime(dat.year(), dat.month(), dat.day(), 0);
	std::cout << "curTime" << curTime.year() << " " << curTime.month() << " " << curTime.day() << "" << curTime.hours() << endl;
	double timeUTC = curTime.asTimeStamp() - baseTime.asTimeStamp();
	int day = timeUTC / (3600 * 24);
	double deg = day*360.0 / 365.0;
	int floorDeg = floor(deg);
	double deltHours = dat.hours() - baseTime.hours();
	double deltHoursDeg = deltHours*360.0 / 24;

	_totalEarthRoateDeg = deg - (floorDeg / 360 * 360) + deltHoursDeg;

	// 2015 05 01 436.72602739726017  224

	_reveTime = dt.asTimeStamp() + 8 * 60 * 60;//_sky->getDateTime().asTimeStamp() - 60*60;
	_SimStartTime = dt.asTimeStamp();
	INT64 maxDeg = (INT64)(ddt.asTimeStamp() *  0.00416666666) % 360;
	//_totalEarthRoateDeg = 0;//_J2000TimeOffset *  0.00416666666 - maxDeg;  //1840784.9970547440;//1818929.9970897122
	//_bStopSim = false;
	FixedToJ2000(_totalEarthRoateDeg);
}

void earthWidget::FixedToJ2000(double deg)
{
	_sateOrbitParentMatrix->setMatrix(_sateOrbitParentMatrix->getMatrix()*
		osg::Matrix::rotate(osg::inDegrees(-deg), osg::Z_AXIS));
	_sateListMatrix->setMatrix(_sateListMatrix->getMatrix()*
		osg::Matrix::rotate(osg::inDegrees(-deg), osg::Z_AXIS));
}

void earthWidget::setEarthManipulator()
{
	//切换视角
	_pEarthMan = new osgEarth::Util::EarthManipulator;
	_pEarthMan->getSettings()->setMinMaxPitch(-80.0, 89.0);
	_view->setCameraManipulator(_pEarthMan);

	_pEarthMan->setViewpoint(osgEarth::Util::Viewpoint("", 105, 33, 0, 0, -90, 30000000));
	_pEarthMan->setHomeViewpoint(osgEarth::Util::Viewpoint("", 105, 33, 0, 0, -90, 30000000));

	_pFollowMan = new osgGA::NodeTrackerManipulator;
	_pFollowMan->setHomePosition(osg::Vec3(20000, -100000.0, 20000.0), osg::Vec3(0, 0, 0), osg::Z_AXIS);
	_pFollowMan->setTrackerMode(osgGA::NodeTrackerManipulator::NODE_CENTER); //
	_pFollowMan->setRotationMode(osgGA::NodeTrackerManipulator::TRACKBALL);
	_pFollowMan->setTrackNode(_root->getChild(0));

}

void earthWidget::creatCanvas()
{

	//控件绘制容器
	_canvas = new ControlCanvas();
	//将要显示的控件加入到root组节点中去
	_root->addChild(_canvas);

	m_timeInfo = new OsgTextNode;
	m_timeInfo->setTextColor(osg::Vec4(255, 255, 255, 0.5));
	m_timeInfo->setTextLocatePos(osg::Vec2d(10, 45));
	m_timeInfo->setTextSize(20);

	//_pRoot->addChild(m_importInfo);
	_root->addChild(m_timeInfo);

	addPlane();
}

void earthWidget::addLabel()
{
	// Style our labels:
	osgEarth::Symbology::Style labelStyle;
	labelStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->alignment() = osgEarth::Symbology::TextSymbol::ALIGN_CENTER_CENTER;
	labelStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->fill()->color() = osgEarth::Symbology::Color::Yellow;

	// A lat/long SRS for specifying points.
	//const SpatialReference* geoSRS = mapNode->getMapSRS()->getGeographicSRS();

	//--------------------------------------------------------------------

	// A series of place nodes (an icon with a text label)
	{
		osgEarth::Symbology::Style pm;
		pm.getOrCreate<osgEarth::Symbology::IconSymbol>()->url()->setLiteral("../data/placemark32.png");
		pm.getOrCreate<osgEarth::Symbology::IconSymbol>()->declutter() = true;
		pm.getOrCreate<osgEarth::Symbology::TextSymbol>()->halo() = osgEarth::Symbology::Color("#5f5f5f");

		// bunch of pins:
		_root->addChild(new osgEarth::Annotation::PlaceNode(_pMapNode, osgEarth::GeoPoint(m_pGeoSRS, 117.5, 39.38), "Beijing", pm));
		_root->addChild(new osgEarth::Annotation::PlaceNode(_pMapNode, osgEarth::GeoPoint(m_pGeoSRS, 108.93, 34.27), "Xian", pm));
		
	}
}

void earthWidget::createLabels()
{
	//下面是设置一个控件，grid的意思是用格网去布局里面的小控件
	// Make the readout:
	grid = new Grid();
	//设置几个Label文字控件显示在场景中的第行
	std::wstring str = L"Coords (Lat, Long)经纬度:";
	grid->setControl(0, 0, new LabelControl(""));
	grid->setControl(0, 1, new LabelControl("Vertical Datum:"));
	grid->setControl(0, 2, new LabelControl("Height (MSL):"));
	grid->setControl(0, 3, new LabelControl("Height (HAE):"));
	grid->setControl(0, 4, new LabelControl("Isect  (HAE):"));
	grid->setControl(0, 5, new LabelControl("Resolution:"));
	//设置几个Label文字控件显示在场景中的第行
	s_posLabel = grid->setControl(1, 0, new LabelControl(""));
	s_vdaLabel = grid->setControl(1, 1, new LabelControl(""));
	s_mslLabel = grid->setControl(1, 2, new LabelControl(""));
	s_haeLabel = grid->setControl(1, 3, new LabelControl(""));
	s_mapLabel = grid->setControl(1, 4, new LabelControl(""));
	s_resLabel = grid->setControl(1, 5, new LabelControl(""));
	_canvas->addChild(grid);
}

void earthWidget::addPlane()
{
	_planeRoot = new osg::Group();
	
	nodePlane = osgDB::readNodeFile("glider.osg");
	//
	//osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
	//mt->setMatrix(osg::Matrix::scale(osg::Vec3(5000, 5000, 5000)));
	//mt->addChild(nodePlane);
	//
	osg::ref_ptr<NodeMatrix> nodeMatrix = new NodeMatrix;
	nodeMatrix->addsChild(nodePlane);
	nodeMatrix->toScale(osg::Matrix::scale(osg::Vec3(5000, 5000, 5000))*osg::Matrix::rotate(osg::DegreesToRadians(-130.0), 0.0, 0.0, 1.0));
	//nodeMatrix->toRotate(-180.0, osg::Z_AXIS);
	

	_geo = new GeoPositionNode(_pMapNode);
	_geo->getPositionAttitudeTransform()->addChild(nodeMatrix);

	_vec3d = new osg::Vec3d(108.93, 34.27, 100000.0);
	GeoPoint p = GeoPoint(SpatialReference::create("wgs84"), *_vec3d);
	_geo->setPosition(p);

	Style style;
	TextSymbol* text = style.getOrCreate<TextSymbol>();
	text->size() = 25.0f;
	text->declutter() = false;
	text->pixelOffset()->set(50, 50);

	_label = new LabelNode(_name, style);
	_label->setDynamic(true);
	_label->setHorizonCulling(false);

	_geo->getPositionAttitudeTransform()->addChild(_label);

	_planeRoot->addChild(_geo.get());

	_root->addChild(_planeRoot.get());
	
}

void earthWidget::slot_isFollowPlane(bool isFollow)
{
	if (isFollow)
	{
		//_geo->getPosition();
		_pFollowMan->setHomePosition(osg::Vec3(0, 0, 100000.0), osg::Vec3(), -osg::Z_AXIS);
		_pFollowMan->setTrackNode(nodePlane);
		_view->setCameraManipulator(_pFollowMan);
		_view->getCamera()->setNearFarRatio(0.001);

		_view->addEventHandler(new updateEventHandler(_geo, _vec3d));

		//Viewpoint vp = _pEarthMan->getViewpoint();
		//double dis = vp.getRange();
		//double pitch = vp.getPitch();
		//std::cout << "dis :" << dis << "pitch :" << pitch << std::endl;
		////vp.setNode( _pat.get() );
		////vp.setNode(_model);
		//vp.range() = 25000.0;
		//vp.pitch() = -45.0;
		//_pEarthMan->setViewpoint(vp, 2.0);
		////_pEarthMan->setViewpoint();
		//_pEarthMan->setTetherNode(nodePlane);
	} 
	else
	{
		_view->setCameraManipulator(_pEarthMan);
	}
}

void earthWidget::slot_addInterval()
{
	if (mInterval < 100)
	{
		mInterval += 10;
	}
}

void earthWidget::slot_subInterval()
{
	if (mInterval > 0)
	{
		mInterval -= 10;
	}
}

void earthWidget::test()
{
	
}

void earthWidget::slot_test()
{
	osg::Vec3f vec3f = _geo->getBound().center();
	//std::cout << "Vec3d x:" << vec3f.x() << ", y:" << vec3f.y() << ", z:" << vec3f.z() << endl;

	if (_vec3d->x() >= 117.5)
	{
		return;
	}
	_vec3d->x() += 0.001428;
	_vec3d->y() += 0.00085166;

	//std::cout << "Vec3d x:" << _vec3d->x() << ", y:" << _vec3d->y() << ", z:" << _vec3d->z() << endl;
}

void earthWidget::slot_startPlay()
{
	if (timer->isActive())
	{
		timer->stop();
	} 
	else
	{
		timer->start(100);
	}
}

void earthWidget::connectWidgets()
{
	timer = new QTimer;
	connect(timer, SIGNAL(timeout()), this, SLOT(slot_test()));
}



void earthWidget::slot_currentMousePos(osg::Vec3d mousePos)
{
	QString str;
	str = QString("%1 , + %2").arg(mousePos.x()).arg(mousePos.y());
	s_posLabel->setText(str.toStdString());
}




