#include "OsgTextNode.h"


OsgTextNode::OsgTextNode(void)
{
	m_speed = 10;
	m_deadTime = 2;
	m_bDead = true;
	addChild(createHUD());
}


OsgTextNode::~OsgTextNode(void)
{
}

void OsgTextNode::setPrintText(QString txt, QString sqlit /*= ""*/)
{
	PrintText strucText;
	strucText.needToPrintText = txt;
	strucText.sqlit = sqlit;
	m_waitForPrintVec.push_back(strucText);

	//m_needPrintText = txt.split(sqlit);	
	//m_sqlit = sqlit;
	//m_bDead = false;
}

bool OsgTextNode::printText(QString txt)
{
	text->setText(txt.toStdString());
	return true;
}

bool OsgTextNode::printText(int size)
{
	if (m_waitForPrintVec.size() > 0)
	{
		PrintText strucText = m_waitForPrintVec[0];
		m_sqlit = strucText.sqlit;
		m_needPrintText = strucText.needToPrintText.split(m_sqlit);
	}
	else
	{
		return  false;
	}
	QString printText;
	if (size > m_needPrintText.size())
	{
		size = m_needPrintText.size();
	}
	for (int i = 0; i < size; ++i)
	{
		printText += m_needPrintText[i];
		printText += m_sqlit;
	}
	text->setText(printText.toStdString());
	return true;
}

void OsgTextNode::setPrintSpeed(int num)
{
	m_speed = num;
}

void OsgTextNode::setDeadTime(int s)
{
	m_deadTime = s;
}

void OsgTextNode::setNodeDead()
{
	if (m_waitForPrintVec.size() > 0)
	{
		text->setText("");
		m_waitForPrintVec.pop_front();
	}
}

osg::Node* OsgTextNode::createHUD()
{
	osg::Geode* geode = new osg::Geode();
	//�������壬�����Ǻ������壬���û�п����Լ��Ҹ���������ʲô�ģ������ǻ��Ĳ��ơ� 
	std::string caiyun("fonts/arial.ttf");
	//����״̬���رյƹ� 
	osg::StateSet* stateset = geode->getOrCreateStateSet();
	stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	osg::Vec3 position(10.0f, 1000.0f, 0.0f);
	//������������ 
	text = new osgText::Text;
	geode->addDrawable(text);
	//�������� 
	text->setFont(caiyun);
	//����λ�� 
	text->setPosition(position);


	//text->setText("123");

	//������� 
	osg::Camera* camera = new osg::Camera;
	//����͸�Ӿ��� 
	camera->setProjectionMatrix(osg::Matrix::ortho2D(0, 1920, 0, 1080));
	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	//�õ�Ĭ������ 
	camera->setViewMatrix(osg::Matrix::identity());
	//���ñ���Ϊ͸��������Ļ���������ClearColor 
	camera->setClearMask(GL_DEPTH_BUFFER_BIT);
	//������Ⱦ˳�򣬱����������Ⱦ 
	camera->setRenderOrder(osg::Camera::POST_RENDER);
	camera->addChild(geode);
	return camera;
}

void OsgTextNode::setTextLocatePos(osg::Vec2d pos)
{
	m_txtPos = pos;
	osg::Vec3 txtPos(m_txtPos[0], m_txtPos[1], 0);
	text->setPosition(txtPos);
}

void OsgTextNode::setTextColor(osg::Vec4 color)
{
	text->setColor(color);
}

void OsgTextNode::setTextSize(float size)
{
	text->setCharacterSize(size);
}
