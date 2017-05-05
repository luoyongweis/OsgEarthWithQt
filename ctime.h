#ifndef CTIME_H
#define CTIME_H

#include <QThread>
#include "TypeDef.h"

class cTime : public QThread
{
	Q_OBJECT

public:
	static cTime* getInstance();
	cTime(QObject *parent = NULL);
	~cTime();

	//DeclareProperty(int, SceneCurrentTime);    //������ǰʱ��

private:
	static cTime* _cTime;
};

#endif // CTIME_H
