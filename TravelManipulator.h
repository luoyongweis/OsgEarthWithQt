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

	// 把漫游器添加到场景中  
	static TravelManipulator * TravelToScence(osg::ref_ptr<osgViewer::Viewer>viewer);  

private:  

	osg::ref_ptr<osgViewer::Viewer>m_pHostViewer;  

	// 移动速度  
	float m_fMoveSpeed;  

	// 当前位置  
	osg::Vec3 m_vPosition;  

	// 旋转角度  
	osg::Vec3 m_vRotation;  

public:  

	// 鼠标左键状态  
	bool m_bLeftButtonDown;  

	// 鼠标位置  
	float m_fpushX;  
	float m_fpushY;  

	// 设置矩阵  
	virtual void setByMatrix(const osg::Matrix &matrix);  

	// 设置逆矩阵  
	virtual void setByInverseMatrix(const osg::Matrix &matrix);  

	// 获取矩阵  
	virtual osg::Matrixd getMatrix() const;  

	// 获取逆矩阵  
	virtual osg::Matrixd getInverseMatrix() const;  

	// 事件处理函数  
	virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us);  

	// 屏幕角度  
	float m_fAngle;  

	// 位置变换  
	void ChangePosition(osg::Vec3 &delta);  

	// 碰撞检测状态  
	bool m_bPeng;  

	// 设置速度  
	float getSpeed();  

	void setSpeed(float &);  

	// 设置初始位置  
	void SetPosition(osg::Vec3 &position);  

	osg::Vec3 GetPosition();  


}; 