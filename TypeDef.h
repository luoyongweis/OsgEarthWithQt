#ifndef _TYPEDEF_H
#define _TYPEDEF_H
#include <QString>
#include <QMetaType.h>
#include <string>
using namespace std;

#define DeclareProperty(kind, v_property)	\
	public:\
	const kind& get##v_property() const { return m_##v_property; };\
	kind& get##v_property(){ return m_##v_property; };\
	void set##v_property(kind value){ m_##v_property = value; };\
	protected:\
	kind m_##v_property

typedef enum
{
	Coordinate_Earthfixed = 0, //µØ¹Ì
	Coordinate_J2000
}CoordinateSystem;


#endif


