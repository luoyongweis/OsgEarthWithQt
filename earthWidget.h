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
#include <osgGA/NodeTrackerManipulator>
#include <osgEarth/MapNode>
#include <osgParticle/PrecipitationEffect>
#include <osg/Multisample>
#include <osgEarth/Utils>
#include <osgEarthUtil/EarthManipulator>
#include <QTimer>

#include "NodeMatrix.h"
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

public slots:

	void slot_isFollowPlane(bool isFollow);
	void slot_addInterval();
	void slot_subInterval();
	void slot_startPlay();
	void slot_test();
	void slot_currentMousePos(osg::Vec3d);

protected:

	void connectWidgets();
	void FixedToJ2000(double deg);
	virtual void paintGL();
	void updateScene();
	void updateSceneTime();
	void createLabels();
	void creatCanvas();
	void test();
	void addEventHandlers();

private:

	static int mInterval, _count;
	osg::ref_ptr<osgViewer::Viewer> _view;
	osg::ref_ptr<osgEarth::MapNode> _pMapNode;
	osg::ref_ptr<osg::Group> _root, _planeRoot;
	osg::ref_ptr<const osgEarth::SpatialReference> m_pMapSRS;
	osg::ref_ptr<const osgEarth::SpatialReference> m_pGeoSRS;
	osg::ref_ptr<osgEarth::Util::SkyNode> _sky;
	osg::ref_ptr<osgEarth::Util::EarthManipulator>   _pEarthMan;
	/**节点跟随操作器*/
	osg::ref_ptr<osgGA::NodeTrackerManipulator>   _pFollowMan;
	osg::ref_ptr<osg::MatrixTransform>             _sateOrbitParentMatrix;
	osg::ref_ptr<osg::MatrixTransform>              _sateListMatrix;
	/**链路父矩阵*/
	osg::ref_ptr<osg::MatrixTransform>             _linkParentMatrix;
	/**坐标系*/
	CoordinateSystem _coordinateSystem;
	osg::ref_ptr<osg::Node> nodePlane;
	LabelNode*                         _label;
	//飞机坐标
	osg::ref_ptr<GeoPositionNode>      _geo;
	osg::Vec3d*						   _vec3d; 
	ControlCanvas* _canvas;

	QMutex   _osgLock;
	//QTimer _timer;
	OsgTextNode *m_timeInfo;

	double _reveTime;
	double _J2000TimeOffset;
	double _totalEarthRoateDeg;
	double _SimStartTime;
	Grid* grid;
	LabelControl*  s_posLabel = 0L;
	LabelControl*  s_vdaLabel = 0L;
	LabelControl*  s_mslLabel = 0L;
	LabelControl*  s_haeLabel = 0L;
	LabelControl*  s_mapLabel = 0L;
	LabelControl*  s_resLabel = 0L;
	QTimer* timer;
};


