#pragma once
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgEarthAnnotation/AnnotationUtils>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarth/MapNode>
#include <osgEarth/Utils>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/Controls>
#include <osgEarthAnnotation/LabelNode>
#include <osgEarth/GeoMath>
#include <osgEarth/GeoTransform>
#include <osgManipulator/TrackballDragger>
#include <osgEarthUtil/LogarithmicDepthBuffer>
#include <osgGA/NodeTrackerManipulator>
#include <osgEarth/Viewpoint>
#include <QtCore/qglobal.h>
#include <QGLWidget>
#include <osgViewer/GraphicsWindow>
#include <QKeyEvent>
#include <QEvent>
#include <QGraphicsView>
#include <QTimer>
#include <iostream>
#include <QObject>
#include <osgEarth/ElevationQuery>
#include <osgEarth/ElevationLayer>
#include <osgEarth/ElevationLOD>
#include <osgEarthUtil/Formatter>
#include <osgEarthUtil/LatLongFormatter>
#include <osgEarth/TerrainEngineNode>
#include <osgEarth/StringUtils>
#include <osgEarth/Terrain>
#include <osgEarthUtil/Formatter>
#include <osgEarthUtil/LatLongFormatter>
#include <osgEarth/ElevationQuery>
#include <osgUtil/LineSegmentIntersector>

using namespace std;

#define D2R (osg::PI/180.0)
#define R2D (180.0/osg::PI)



class AdapterWidget : public QGLWidget
{
	Q_OBJECT

public:
	AdapterWidget(QWidget *parent = NULL);
	~AdapterWidget();

	osgViewer::GraphicsWindow* getGraphicsWindow() { return _gw.get(); }
	const osgViewer::GraphicsWindow* getGraphicsWindow() const { return _gw.get(); }
	osg::GraphicsContext* getGraphicsContext() { return _gc.get(); }
	const osg::GraphicsContext* getGraphicsContext() const { return _gc.get(); }

protected:
	void init();
	virtual void resizeGL(int width, int height);
	virtual void keyPressEvent(QKeyEvent* event);
	virtual void keyReleaseEvent(QKeyEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mouseDoubleClickEvent(QMouseEvent* event);
	virtual void wheelEvent(QWheelEvent* event);

private:

	void setKeyboardModifiers(QInputEvent* event);
	osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> _gw;
	osg::ref_ptr<osg::GraphicsContext>	_gc;	
signals:
	void quitFullScreen();
};


using namespace osgEarth::Util;
using namespace osgEarth::Util::Controls;
using namespace osgEarth::Annotation;

//更新飞机位置
class updateEventHandler : public osgGA::NodeTrackerManipulator/*, public QObject*/
{
	//Q_OBJECT
public:
	
	updateEventHandler(const osg::ref_ptr<GeoPositionNode> _geo, const osg::Vec3d* vec3d);
	virtual ~updateEventHandler(){};
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
	//GeoPoint p = GeoPoint(SpatialReference::create("wgs84"), osg::Vec3(0 + 0.001*a++, 0, 8000000.0));
	//_geo->setPosition(p);
private:
	updateEventHandler();
	osg::ref_ptr<GeoPositionNode>      _geo;
	const osg::Vec3d*				   _vec3d;

};


struct MousePositionHandler : public osgGA::GUIEventHandler
{
public:
	//Q_OBJECT
	//构造函数
	MousePositionHandler(osg::ref_ptr<const osgEarth::SpatialReference>);

	//更新回调，具体的内容可以参考父类，传进来的参数是屏幕坐标xy，和osgViewer
	void update(float x, float y, osgViewer::View* view);

	//参数一个是事件的动作，另外一个是对应的操作
	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

	//Map对象
	const Map*       _map;
	//地形对象
	//const Terrain*   _terrain;
	osgEarth::MapNode* _mapNode;
	osg::ref_ptr<const osgEarth::SpatialReference> _srs;
	bool             _mouseDown;
	//查询高程使用的对象
	ElevationQuery   _query;
	osg::NodePath    _path;
	std::string _lat, _long;
public:
	//signals :
	//void signal_latlong(std::string, std::string);
};



