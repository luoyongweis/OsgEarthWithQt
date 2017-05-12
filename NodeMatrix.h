#pragma once

#include <iostream>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Matrixd>

class NodeMatrix :public osg::MatrixTransform
{
public:

	NodeMatrix();

	virtual ~NodeMatrix();

public:
	/**���õ�ǰģ��ת����ʽ*/
	void rotating(const osg::Vec3d &pivot, const osg::Vec3d &axis, float angularVelocity);

	/**��תģ��*/
	void toRotate(const osg::Matrix &mat);

	/**��תģ��*/
	void toRotate(float angle, const osg::Vec3f &axis);

	/**����ģ��*/
	void toScale(const osg::Matrix &mat);

	/**����ģ��*/
	void toScale(double &lel);

	/**addsChild����*/
	void addsChild(osg::Node *node);

	/**��ģ���Ƶ�Ŀ�ĵ�*/
	void toPosition(osg::Vec3d &pos);

	/**�õ�ģ�͵�ǰ��λ��*/
	osg::Vec3d getPosition();

	/**����ģ�ʹ�С*/
	void adapt(osg::BoundingSphere &sps);

	/**����ģ�ʹ�С*/
	void adapt(osg::Node *node);

private:
	osg::ref_ptr<osg::MatrixTransform> pat;
	osg::BoundingSphere ps;
	osg::Node * pnode;
	float level;
	osg::Vec3d position;
};

