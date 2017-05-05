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
	//设置字体，必须是汉字字体，如果没有可以自己找个黑体宋体什么的，这里是华文彩云。 
	std::string caiyun("fonts/arial.ttf");
	//设置状态，关闭灯光 
	osg::StateSet* stateset = geode->getOrCreateStateSet();
	stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	osg::Vec3 position(10.0f, 1000.0f, 0.0f);
	//设置字体属性 
	text = new osgText::Text;
	geode->addDrawable(text);
	//设置字体 
	text->setFont(caiyun);
	//设置位置 
	text->setPosition(position);


	//text->setText("123");

	//设置相机 
	osg::Camera* camera = new osg::Camera;
	//设置透视矩阵 
	camera->setProjectionMatrix(osg::Matrix::ortho2D(0, 1920, 0, 1080));
	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	//得到默认设置 
	camera->setViewMatrix(osg::Matrix::identity());
	//设置背景为透明，否则的话可以设置ClearColor 
	camera->setClearMask(GL_DEPTH_BUFFER_BIT);
	//设置渲染顺序，必须在最后渲染 
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
