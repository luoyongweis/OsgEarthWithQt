#include "ctime.h"

cTime* cTime::_cTime = NULL;

cTime::cTime(QObject *parent)
	: QThread(parent)
{

}

cTime::~cTime()
{

}

cTime* cTime::getInstance()
{
	if (_cTime == NULL)
	{
		_cTime = new cTime;
	}
	return _cTime;
}
