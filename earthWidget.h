#pragma once

#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <osg/Node>  
#include <osg/Geode>  
#include <osg/Group>  
#include <osg/LineSegment>
#include <osg/PositionAttitudeTransform>
#include <osgUtil/IntersectVisitor>
#include <osgGA/DriveManipulator>
#include "TravelManipulator.h"
#include <osgUtil/Optimizer>
#include <osgViewer/ViewerEventHandlers>  
#include <osgGA/StateSetManipulator> 
#include <osgGA/CameraManipulator>  
#include <osgEarth/MapNode>
#include <osgEarthUtil/Sky>
#include <osgEarthUtil/AutoClipPlaneHandler>
#include <osgEarth/MapNode>
#include <osgParticle/PrecipitationEffect>
#include <osg/Multisample>
#include <osgEarth/Utils>
#include <osgEarthUtil/EarthManipulator>
#include <QTimer>


#include "AdapterWidget.h"
#include "ctime.h"
#include "OsgTextNode.h"

//class CGLWidget;



class earthWidget : public AdapterWidget, public osgViewer::Viewer
{
	Q_OBJECT
public:

	earthWidget(QWidget* parent = NULL);
	~earthWidget();

	void initViewer();
	void initEarthWidget();
	void earthRun();

	inline void setStartDateTime(const osgEarth::DateTime &dt);
	void setEarthManipulator();
	void addLabel();
	void addPlane();

protected:

	void FixedToJ2000(double deg);
	virtual void paintGL();
	void updateScene();
	void updateSceneTime();
	void creatCanvas();

private:

	static int mInterval;
	osg::ref_ptr<osgViewer::Viewer> _view;
	osg::ref_ptr<osgEarth::MapNode> _pMapNode;
	osg::ref_ptr<osg::Group> _root, _planeRoot;
	osg::ref_ptr<const osgEarth::SpatialReference> m_pMapSRS;
	osg::ref_ptr<const osgEarth::SpatialReference> m_pGeoSRS;
	osg::ref_ptr<osgEarth::Util::SkyNode> _sky;
	osg::ref_ptr<osgEarth::Util::EarthManipulator>   _pEarthMan;
	osg::ref_ptr<osg::MatrixTransform>             _sateOrbitParentMatrix;
	osg::ref_ptr<osg::MatrixTransform>              _sateListMatrix;
	/**Á´Â·¸¸¾ØÕó*/
	osg::ref_ptr<osg::MatrixTransform>             _linkParentMatrix;
	/**×ø±êÏµ*/
	CoordinateSystem _coordinateSystem;

	QMutex   _osgLock;
	//QTimer _timer;
	OsgTextNode *m_timeInfo;

	double _reveTime;
	double _J2000TimeOffset;
	double _totalEarthRoateDeg;
	double _SimStartTime;
};

