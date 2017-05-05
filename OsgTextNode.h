#pragma once

#include <osgText/Text>
#include <osg/Camera>
#include <osg/Group>
#include <QString>
#include <QStringList>
#include <QVector>

#ifndef NDEBUG
#pragma comment(lib,"osgTextd.lib")
#else
#pragma comment(lib,"osgText.lib")
#endif

typedef struct  tag_PrintText
{
	struct  tag_PrintText()
	{
		sqlit = "";
		bDead = false;
	}
	QString needToPrintText;
	QString sqlit;
	bool bDead;
}PrintText;
class OsgTextNode :public osg::Group
{
public:
	OsgTextNode();
	~OsgTextNode();
	void setPrintText(QString txt, QString sqlit = "");
	bool printText(int size);
	//直接打印所有字符
	bool printText(QString txt);
	void setPrintSpeed(int num);
	void setTextLocatePos(osg::Vec2d pos);
	void setTextColor(osg::Vec4 color);
	void setTextSize(float size);
	inline int getPrintSpeed()const
	{
		return m_speed;
	}
	void setDeadTime(int s);
	inline int getDeadTime()const
	{
		return m_deadTime;
	}
	void setNodeDead();
	int getTextSize()
	{
		return m_needPrintText.size();
	}
protected:
	osg::Node* createHUD();
public:
	QStringList m_needPrintText;
	QString m_sqlit;
	QVector<PrintText> m_waitForPrintVec;
	int m_speed;
	int m_deadTime;
	osgText::Text* text;
	bool m_bDead;
	osg::Vec2d m_txtPos;
};

class OsgTextPrinter :public osg::NodeCallback
{
public:
	OsgTextPrinter()
	{
		m_delayTimes = 0;
		m_printRef = 0;
	}

	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		OsgTextNode* textNode = (OsgTextNode*)node;
		if (textNode)
		{
			if (m_delayTimes%textNode->getPrintSpeed() == 0)
			{
				m_delayTimes = 0;

				//m_printRef++;

				if (m_printRef - textNode->getTextSize() >= textNode->getDeadTime())
				{
					textNode->setNodeDead();
					m_printRef = 0;
				}
				if (textNode->printText(m_printRef + 1))  //个数
				{
					m_printRef++;
				}

			}
			m_delayTimes++;
		}
	}
	int m_printRef;
	int m_delayTimes;
};

