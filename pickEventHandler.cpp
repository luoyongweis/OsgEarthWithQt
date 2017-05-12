#include "pickEventHandler.h"
#include <osgViewer/Viewer>

PickHandle::PickHandle(const osgEarth::SpatialReference* srs)
{
	m_pEllipsoid = srs->getGeodeticSRS()->getEllipsoid();
}

PickHandle::~PickHandle()
{

}

bool PickHandle::handle(const osgGA::GUIEventAdapter& ea,
	osgGA::GUIActionAdapter& aa)
{
	// �洢��γ����Ϣ
	osg::Vec3d vecPos;
	switch (ea.getEventType())
	{
		// ����¼�
	case osgGA::GUIEventAdapter::PUSH:
	{
		osg::Vec3d pos = getPos(ea, aa, vecPos);
		// ������
		if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
		{
			m_vecPostion = pos;
		}
		break;
	}


	// ����ƶ��¼�
	case osgGA::GUIEventAdapter::MOVE:
	{
		osg::Vec3d pos = getPos(ea, aa, vecPos);
		emit signalMoving(vecPos);
		emit signalMovingXYZ(pos);
		break;
	}

	// ����ͷ��¼�
	case osgGA::GUIEventAdapter::RELEASE:
	{
		osg::Vec3d pos = getPos(ea, aa, vecPos);
		// ������
		if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
		{
			// ����ͷŵĵ�͵���ĵ�ͬһ�����͵����¼�������λ��
			if (m_vecPostion == pos && m_vecPostion != osg::Vec3d(0, 0, 0))
			{
				emit signalPicked(vecPos);
				emit signalPickedXYZ(pos);
			}
		}
		else if (ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
		{
			emit signalRightPicked();
		}
		break;
	}
	}

	return false;
}

osg::Vec3d PickHandle::getPos(const osgGA::GUIEventAdapter& ea,
	osgGA::GUIActionAdapter& aa, osg::Vec3d& pos)
{
	pos = osg::Vec3d(0, 0, 0);
	osgViewer::Viewer* pViewer = dynamic_cast<osgViewer::Viewer*>(&aa);
	if (pViewer == NULL)
	{
		return osg::Vec3d(0, 0, 0);
	}
	// ��ȡ��ǰ��
	osgUtil::LineSegmentIntersector::Intersections intersection;
	double x = ea.getX();
	double y = ea.getY();
	pViewer->computeIntersections(ea.getX(), ea.getY(), intersection);
	osgUtil::LineSegmentIntersector::Intersections::iterator iter
		= intersection.begin();
	if (iter != intersection.end())
	{
		m_pEllipsoid->convertXYZToLatLongHeight(
			iter->getWorldIntersectPoint().x(), iter->getWorldIntersectPoint().y(), iter->getWorldIntersectPoint().z(),
			pos.y(), pos.x(), pos.z());
		pos.x() = osg::RadiansToDegrees(pos.x());
		pos.y() = osg::RadiansToDegrees(pos.y());
		return iter->getWorldIntersectPoint();
	}
	return osg::Vec3d(0, 0, 0);
}