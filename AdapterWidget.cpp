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



