#pragma once  

#include <osgViewer/Viewer>  

#include <osg/LineSegment>  
#include <osg/Point>  
#include <osg/Geometry>  
#include <osg/Node>  
#include <osg/Geode>  
#include <osg/Group>  
#include <osg/MatrixTransform> 
#include <osgGA/CameraManipulator>  


#include <osgUtil/IntersectVisitor>  

#include <vector>  

class TravelManipulator:public osgGA::CameraManipulator  
{  

public:  

	TravelManipulator(void);  
	~TravelManipulator(void);  

	// ����������ӵ�������  
	static TravelManipulator * TravelToScence(osg::ref_ptr<osgViewer::Viewer>viewer);  

private:  

	osg::ref_ptr<osgViewer::Viewer>m_pHostViewer;  

	// �ƶ��ٶ�  
	float m_fMoveSpeed;  

	// ��ǰλ��  
	osg::Vec3 m_vPosition;  

	// ��ת�Ƕ�  
	osg::Vec3 m_vRotation;  

public:  

	// ������״̬  
	bool m_bLeftButtonDown;  

	// ���λ��  
	float m_fpushX;  
	float m_fpushY;  

	// ���þ���  
	virtual void setByMatrix(const osg::Matrix &matrix);  

	// ���������  
	virtual void setByInverseMatrix(const osg::Matrix &matrix);  

	// ��ȡ����  
	virtual osg::Matrixd getMatrix() const;  

	// ��ȡ�����  
	virtual osg::Matrixd getInverseMatrix() const;  

	// �¼�������  
	virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us);  

	// ��Ļ�Ƕ�  
	float m_fAngle;  

	// λ�ñ任  
	void ChangePosition(osg::Vec3 &delta);  

	// ��ײ���״̬  
	bool m_bPeng;  

	// �����ٶ�  
	float getSpeed();  

	void setSpeed(float &);  

	// ���ó�ʼλ��  
	void SetPosition(osg::Vec3 &position);  

	osg::Vec3 GetPosition();  


}; 