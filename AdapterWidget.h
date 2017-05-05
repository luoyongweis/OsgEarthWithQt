#pragma once
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
#include <QtCore/qglobal.h>
#include <QGLWidget>
#include <osgViewer/GraphicsWindow>
#include <QKeyEvent>
#include <QEvent>
#include <QGraphicsView>
#include <QTimer>
#include <iostream>

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

/**
* A simple simulator that moves an object around the Earth. We use this to
* demonstrate/test tethering.
*/
struct Simulator : public osgGA::GUIEventHandler
{
	Simulator(osg::Group* root, EarthManipulator* manip, MapNode* mapnode, osg::Node* model, const char* name, char key)
		: _manip(manip), _mapnode(mapnode), _model(model), _name(name), _key(key)
	{
		//model = NULL;
		if (!model)
		{
			_model = osgDB::readNodeFile("glider.osgt");
			//_model = AnnotationUtils::createHemisphere(550.0, osg::Vec4(1, .7, .4, 1));
			float scale = _model->getBound().radius();
			//_model->getBound().set(osg::Vec3f(), 1000);
			cout << "scale :" << scale << endl;
		}	

		//osgManipulator::TrackballDragger* deagger = new osgManipulator::TrackballDragger;

		osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
		mt->setMatrix(osg::Matrix::scale(osg::Vec3(2000, 2000, 2000)));
		mt->addChild(_model);

		_geo = new GeoPositionNode(mapnode);
		_geo->getPositionAttitudeTransform()->addChild(mt);

		Style style;
		TextSymbol* text = style.getOrCreate<TextSymbol>();
		text->size() = 25.0f;
		text->declutter() = false;
		text->pixelOffset()->set(50, 50);

		_label = new LabelNode(_name, style);
		_label->setDynamic(true);
		_label->setHorizonCulling(false);

		_geo->getPositionAttitudeTransform()->addChild(_label);

		root->addChild(_geo.get());
		
	}

	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
	{
		if (ea.getEventType() == ea.FRAME)
		{
			double t0 = osg::Timer::instance()->time_s();
			double t = fmod(t0, 6000.0) / 6000.0;
			double lat, lon;
			GeoMath::interpolate(D2R*_lat0, D2R*_lon0, D2R*_lat1, D2R*_lon1, t, lat, lon);
			GeoPoint p(SpatialReference::create("wgs84"), R2D*lon, R2D*lat, 200.0);
			double bearing = GeoMath::bearing(D2R*_lat0, D2R*_lon0, lat, lon);

			float a = sin(t0*0.2);
			//bearing += a * 0.5 * osg::PI;
			float pitch = 0.0;
			//std::cout << "p.x :" << p.x() << "p.y :" << p.y() << "p.z :" << p.z() << std::endl;
			_geo->setPosition(p);

			_geo->setLocalRotation(
				osg::Quat(pitch, osg::Vec3d(1, 0, 0)) *
				osg::Quat(bearing, osg::Vec3d(0, 0, -1)));
		}
		else if (ea.getEventType() == ea.KEYDOWN)
		{
			if (ea.getKey() == _key)
			{
				Viewpoint vp = _manip->getViewpoint();
				//vp.setNode( _pat.get() );
				vp.setNode(_model);
				vp.range() = 25000.0;
				vp.pitch() = -45.0;
				_manip->setViewpoint(vp, 2.0);
			}
			return true;
		}
		return false;
	}

	std::string                        _name;
	char                               _key;
	MapNode*                           _mapnode;
	EarthManipulator*                  _manip;
	double                             _lat0, _lon0, _lat1, _lon1;
	LabelNode*                         _label;
	osg::Node*                         _model;
	float                              _heading;
	float                              _pitch;

	osg::ref_ptr<GeoPositionNode>      _geo;
};