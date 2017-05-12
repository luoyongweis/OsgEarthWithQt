#include "AdapterWidget.h"
#include <osgViewer/GraphicsWindow>
#include <QByteArray>
#include <QString>

AdapterWidget::AdapterWidget(QWidget *parent)
{
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
	traits->x = 0;
	traits->y = 0;
	traits->width = width();
	traits->height = height();
	traits->doubleBuffer = true;
	traits->depth = 32;
	traits->samples = 16;
	_gw = new osgViewer::GraphicsWindowEmbedded(traits.get());
	setFocusPolicy(Qt::ClickFocus);
}

AdapterWidget::~AdapterWidget()
{

}

void AdapterWidget::resizeGL(int width, int height)
{
	_gw->getEventQueue()->windowResize(0, 0, width, height);
	_gw->resized(0, 0, width, height);
}

void AdapterWidget::keyPressEvent(QKeyEvent* event)
{
	switch (event->key())
	{
	case Qt::Key_Escape:
		emit quitFullScreen(); //for operation 
		break;
	default:
		_gw->getEventQueue()->keyPress((osgGA::GUIEventAdapter::KeySymbol) *(event->text().toLatin1().data()));
	}
}

void AdapterWidget::keyReleaseEvent(QKeyEvent* event)
{
	_gw->getEventQueue()->keyRelease((osgGA::GUIEventAdapter::KeySymbol) *(event->text()./*toAscii()*/toLatin1().data()));
}

void AdapterWidget::mouseMoveEvent(QMouseEvent* event)
{
	_gw->getEventQueue()->mouseMotion(event->x(), event->y());
}

void AdapterWidget::mousePressEvent(QMouseEvent* event)
{
	int button = 0;
	switch (event->button())
	{
	case(Qt::LeftButton) : button = 1; break;
	case(Qt::MidButton) : button = 2; break;
	case(Qt::RightButton) : button = 3; break;
	case(Qt::NoButton) : button = 0; break;
	default: button = 0; break;
	}
	_gw->getEventQueue()->mouseButtonPress(event->x(), event->y(), button);
}

void AdapterWidget::mouseReleaseEvent(QMouseEvent* event)
{
	int button = 0;
	switch (event->button())
	{
	case(Qt::LeftButton) : button = 1; break;
	case(Qt::MidButton) : button = 2; break;
	case(Qt::RightButton) : button = 3; break;
	case(Qt::NoButton) : button = 0; break;
	default: button = 0; break;
	}
	_gw->getEventQueue()->mouseButtonRelease(event->x(), event->y(), button);
}

void AdapterWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
	QPoint pos = QCursor::pos();
	// 	QList<QGraphicsItem*> listItems = items(mapToScene(pos.x(), pos.y()).toPoint());
	// 	if (listItems.size() > 0)
	// 	{
	// 		return;
	// 	}
	int button = 0;
	switch (event->button())
	{
	case Qt::LeftButton: button = 1; break;
	case Qt::MidButton: button = 2; break;
	case Qt::RightButton: button = 3; break;
	case Qt::NoButton: button = 0; break;
	default: button = 0; break;
	}
	setKeyboardModifiers(event);
	_gw->getEventQueue()->mouseDoubleButtonPress(event->x(), event->y(), button);

	QGLWidget::mouseDoubleClickEvent(event);
}

void AdapterWidget::setKeyboardModifiers(QInputEvent* event)
{
	int modkey = event->modifiers() & (Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier);
	unsigned int mask = 0;
	if (modkey & Qt::ShiftModifier) mask |= osgGA::GUIEventAdapter::MODKEY_SHIFT;
	if (modkey & Qt::ControlModifier) mask |= osgGA::GUIEventAdapter::MODKEY_CTRL;
	if (modkey & Qt::AltModifier) mask |= osgGA::GUIEventAdapter::MODKEY_ALT;

	_gw->getEventQueue()->getCurrentEventState()->setModKeyMask(mask);
}

void AdapterWidget::wheelEvent(QWheelEvent* event)
{
	setKeyboardModifiers(event);
	_gw->getEventQueue()->mouseScroll(
		event->orientation() == Qt::Vertical ?
		(event->delta() > 0 ? osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN) :
		(event->delta() > 0 ? osgGA::GUIEventAdapter::SCROLL_LEFT : osgGA::GUIEventAdapter::SCROLL_RIGHT));
	QGLWidget::wheelEvent(event);
}

updateEventHandler::updateEventHandler(osg::ref_ptr<GeoPositionNode> geo, const osg::Vec3d* vec3d)
{
	_geo = geo;
	_vec3d = vec3d;
}


bool updateEventHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	if (ea.getEventType() == ea.FRAME)
	{
		//std::cout << "Vec3d x:" << _vec3d->x() << ", y:" << _vec3d->y() << ", z:" << _vec3d->z() << endl;
		_geo->setPosition(GeoPoint(SpatialReference::create("wgs84"), *_vec3d));
	}
	return false;
}

/*
Simulator::Simulator(osgViewer::Viewer* viewer, osg::Group* root, EarthManipulator* manip, MapNode* mapnode, osg::Node* model, const char* name, char key)
	: _viewer(viewer), _manip(manip), _mapnode(mapnode), _model(model), _name(name), _key(key)
{

	if (!model)
	{
		_model = AnnotationUtils::createHemisphere(550.0, osg::Vec4(1, .7, .4, 1));
	}

	cout << "scale :" << _model->getBound().radius() << endl;
	//osgManipulator::TrackballDragger* deagger = new osgManipulator::TrackballDragger;

	osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
	mt->setMatrix(osg::Matrix::scale(osg::Vec3(5000, 5000, 5000)));
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

bool Simulator::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	if (ea.getEventType() == ea.FRAME)
	{
		double t0 = osg::Timer::instance()->time_s();
		double t = fmod(t0, 6000.0) / 6000.0;
		double lat, lon;
		GeoMath::interpolate(D2R*_lat0, D2R*_lon0, D2R*_lat1, D2R*_lon1, t, lat, lon);
		GeoPoint p(SpatialReference::create("wgs84"), R2D*lon, R2D*lat, 2000.0);
		double bearing = GeoMath::bearing(D2R*_lat0, D2R*_lon0, lat, lon);

		float a = sin(t0*0.2);
		bearing += a * 0.5 * osg::PI;
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
			//Viewpoint vp = _manip->getViewpoint();
			//double dis = vp.getRange();
			//double pitch = vp.getPitch();
			//std::cout << "dis :" << dis << "pitch :" << pitch << std::endl;
			////vp.setNode( _pat.get() );
			//vp.setNode(_model);
			//vp.range() = 25000.0;
			//vp.pitch() = -45.0;
			//_manip->setViewpoint(vp, 2.0);
			_manip->setTetherNode(_model);
			//不可用
			//_manip->setHomePosition(osg::Vec3(48000 * 3, -96800.0 * 4, 15000.0 * 4), osg::Vec3(0, 0, 0), osg::Z_AXIS);
			////osg::Vec3 ViewPos = stMap[name]->getPos() + osg::Vec3(100000, -100, -10.0);
			//_manip->setTrackNode(_model);
			///*stMap[name]->getOrbitLineMatrix()->setNodeMask(0);/* //todo
			//_viewer->setCameraManipulator(_manip);
			//_viewer->getCamera()->setNearFarRatio(0.001);
		}
		return true;
	}
	return false;
}
*/



//构造函数 
MousePositionHandler::MousePositionHandler(osg::ref_ptr<const osgEarth::SpatialReference> srs)
{
	_srs = srs;
}

void MousePositionHandler::update(float x, float y, osgViewer::View* view)
{
	bool yes = false;

	// look under the mouse:
	//采用线去对地球做碰撞检测，根据鼠标点击点去检测，得到交点，就是当前点的xyz
	osg::Vec3d world;
	osgUtil::LineSegmentIntersector::Intersections hits;
	//判断求交结果是否为空
	if (view->computeIntersections(x, y, hits))
	{
		//得到世界坐标系下面的坐标，就是osg的xyz坐标
		world = hits.begin()->getWorldIntersectPoint();

		// convert to map coords:
		//将其转换为地球的地理坐标，转换方法都照抄即可
		GeoPoint mapPoint;
		mapPoint.fromWorld(_srs, world);

		// do an elevation query:
		double query_resolution = 0; // 1/10th of a degree
		double out_hamsl = 0.0;
		double out_resolution = 0.0;

		//根据输入参数查询当前点位置的高程，需要设置分辨率，就是查询精度
		//bool ok = _query.getElevation(
		//	mapPoint,
		//	out_hamsl,
		//	query_resolution,
		//	&out_resolution);

		//如果查询成功
		//if (ok)
		//{
		// convert to geodetic to get the HAE:
		mapPoint.z() = out_hamsl;
		GeoPoint mapPointGeodetic(_srs->getGeodeticSRS(), mapPoint);

		//经纬度坐标的格式化工具，也可以自己用字符串去拼接xyz数字
		//static LatLongFormatter s_f;

		//更新显示的xyz值，label是传入的控件
		_lat = mapPointGeodetic.y();
		_long = mapPointGeodetic.x();
		/*s_posLabel->setText(Stringify()
		<< std::fixed << std::setprecision(2)
		<< mapPointGeodetic.y()
		<< ", "
		<< mapPointGeodetic.x());*/

		cout << "lat long : " << _lat << _long << endl;

		//emit signal_latlong(_lat, _long);
		//还可以输出分辨率，椭球体信息等
		//s_mslLabel->setText(Stringify() << out_hamsl);
		//s_haeLabel->setText(Stringify() << mapPointGeodetic.z());
		//s_resLabel->setText(Stringify() << out_resolution);

		yes = true;
		//}

		// finally, get a normal ISECT HAE point.
		GeoPoint isectPoint;
		isectPoint.fromWorld(_srs->getGeodeticSRS(), world);
		//s_mapLabel->setText(Stringify() << isectPoint.alt());
	}

	//如果查询不到高程的话
	if (!yes)
	{
		/*s_posLabel->setText("-");
		s_mslLabel->setText("-");
		s_haeLabel->setText("-");
		s_resLabel->setText("-");*/
	}
}

bool MousePositionHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	//判断如果是移动鼠标事件才进行更新当前的坐标显示
	if (ea.getEventType() == osgGA::GUIEventAdapter::MOVE /*&&
														  aa.asView()->getFrameStamp()->getFrameNumber() % 10 == 0*/)
	{
		osgViewer::View* view = static_cast<osgViewer::View*>(aa.asView());
		update(ea.getX(), ea.getY(), view);
	}

	return false;
}
