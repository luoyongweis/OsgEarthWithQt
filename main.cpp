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

int main(int argc, char *argv[])
{
	QApplication::setStyle("cleanlooks");
	QApplication a(argc, argv);
	earthWidget *earth = new earthWidget;
	QMainWindow* mw = new QMainWindow();
	//mw->setWindowOpacity(1);
	//mw->setWindowFlags(Qt::FramelessWindowHint);
	//mw->setAttribute(Qt::WA_TranslucentBackground);
	mw->setWindowTitle("Osg Eearth");
	QMdiArea* mdiArea = new QMdiArea(mw);
	mw->setCentralWidget(mdiArea);
	QMdiSubWindow *subWindow = mdiArea->addSubWindow(earth);
	subWindow->showMaximized();
	subWindow->setWindowFlags(Qt::FramelessWindowHint);
	//subWindow->setWindowTitle("New Window");
	mw->show();

	return a.exec();
}

//int main()
//{
//	osg::ref_ptr<osgViewer::Viewer> _view = new osgViewer::Viewer();
//
//	int xoffset = 40;
//	int yoffset = 40;
//
//	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
//	traits->x = xoffset + 0;
//	traits->y = yoffset + 0;
//	traits->width = 800;
//	traits->height = 680;
//	traits->windowDecoration = true;		//窗口装饰开关
//	traits->doubleBuffer = true;
//	traits->sharedContext = 0;
//
//	osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
//
//	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
//	camera->setGraphicsContext(gc.get());
//	camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
//	GLenum buffer = traits->doubleBuffer ? GL_BACK : GL_FRONT;
//	camera->setDrawBuffer(buffer);
//	camera->setReadBuffer(buffer);
//	camera->setNearFarRatio(0.0000001);
//	camera->setComputeNearFarMode(osg::CullSettings::COMPUTE_NEAR_FAR_USING_BOUNDING_VOLUMES);//);
//	camera->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(600) / static_cast<double>(480), 1.0f, 10000.0f);
//	_view->setCamera(camera);
//	_view->setName("Rs");
//
//
//	_view->addEventHandler(new osgViewer::StatsHandler);
//	_view->addEventHandler(new osgViewer::WindowSizeHandler);
//	_view->addEventHandler(new osgViewer::ThreadingHandler());
//	_view->addEventHandler(new osgViewer::LODScaleHandler());
//
//	osg::ref_ptr<osg::Group> root = new osg::Group();
//	osg::Timer* timer = osg::Timer::instance();
//
//
//	//initEarth->>>>>
//	//osg::ref_ptr<osg::Node> node=osgDB::readNodeFile("./models/earthData/mymap.earth");  
//	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("./myearth1.earth");
//	node->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
//	node->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
//
//	osg::ref_ptr<osgEarth::MapNode> _pMapNode = osgEarth::MapNode::get(node);
//	_pMapNode->setName("earth");
//
//	osgEarth::Util::SkyOptions kk;
//	kk.ambient() = 0.20f;
//
//	osg::ref_ptr<osgEarth::Util::SkyNode> _sky = osgEarth::Util::SkyNode::create(kk, _pMapNode);
//	if (_sky)
//	{
//		_sky->attach(_view, 0);
//	}
//
//	osg::ref_ptr<const osgEarth::SpatialReference> m_pMapSRS = _pMapNode->getMapSRS();/**地固坐标系*/
//	osg::ref_ptr<const osgEarth::SpatialReference> m_pGeoSRS = m_pMapSRS->getGeographicSRS();/**大地坐标系*/
//
//	//_pRoot = new osg::Group;
//	double time = 0;
//	osgEarth::DateTime dataTime(time);
//	_sky->setLighting(true);
//	osgEarth::Util::Ephemeris* eph = new osgEarth::Util::Ephemeris;
//	_sky->setEphemeris(eph);
//	_sky->setDateTime(dataTime);
//	_sky->setMoonVisible(true);
//	//#_sky->setMinimumAmbient(osg::Vec4(0.3, 0.3, 0.3,1));
//	_sky->attach(_view);
//
//
//
//	root->addChild(_pMapNode.get());
//	root->addChild(_sky.get());
//	//_pRoot->addChild( _sky );
//
//	////osgUtil::Optimizer optimizer ;
//	////optimizer.optimize(_pRoot.get()); 
//	//
//	////添加轨道父矩阵
//	//_sateOrbitParentMatrix = new osg::MatrixTransform;
//	//_pRoot->addChild(_sateOrbitParentMatrix);
//	////添加链路父矩阵
//	//_linkParentMatrix = new osg::MatrixTransform;
//	//_pRoot->addChild(_linkParentMatrix);
//	////_cameraTrans = new osg::MatrixTransform;
//	//_sateListMatrix = new osg::MatrixTransform;
//	////_sateListMatrix->addChild(_cameraTrans);
//	//_pRoot->addChild(_sateListMatrix);
//
//	////抗锯齿
//	////osg::Multisample* pms=new osg::Multisample; 
//	////pms->setSampleCoverage(1,true);// [i] 属性设置对了吗？[/i] 
//	////g_Root->getOrCreateStateSet()->setMode(GL_MULTISAMPLE_ARB, true);
//	////_pRoot->getOrCreateStateSet()->setAttributeAndModes(pms,osg::StateAttribute::ON); 
//	//osg::Multisample *ms = new osg::Multisample;  
//	//ms->setHint(osg::Multisample::NICEST);
//	//_pRoot->getOrCreateStateSet()->setMode(GL_MULTISAMPLE_ARB, true);
//	//_pRoot->getOrCreateStateSet()->setAttributeAndModes(ms, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
//
//
//	//   //修改意见：3D页面右下角色条无意义。修改：隐藏该色条。
//	//m_linkFreqScalar = new CCustomScalarBar("",8,CCustomScalarBar::SCALAR_RIGHT);//Nav Indication
//	//_pRoot->addChild(m_linkFreqScalar);
//	////m_linkFreqScalar->setNodeMask(0);
//
//	//Style circleStyle;
//	//circleStyle.getOrCreate<PolygonSymbol>()->fill()->color() = Color(Color::Green, 0.8);
//	//circleStyle.getOrCreate<AltitudeSymbol>()->clamping() = AltitudeSymbol::CLAMP_TO_TERRAIN;
//	//circleStyle.getOrCreate<AltitudeSymbol>()->technique() = AltitudeSymbol::TECHNIQUE_DRAPE;
//
//
//
//
//	//osg::ref_ptr<const osgEarth::SpatialReference> pGeoSRS =  _pMapNode->getMapSRS()->getGeographicSRS();
//	//GeoPoint geoEarthPt = GeoPoint(pGeoSRS,112.0, 32.0,0.0);
//
//	//osg::Vec3d earthEndPos;
//	//geoEarthPt.toWorld(earthEndPos);
//
//	//root->addChild(node.get());  
//	//root->addChild(node);  
//
//	osgUtil::Optimizer optimizer;
//	optimizer.optimize(root.get());
//	_view->setSceneData(root.get());
//	_view->realize();
//	_view->run();
//	return 0;
//}