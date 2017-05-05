HEADERS += \
    mainwindow.h \
    menubar.h \
    toolbar.h \
    widget.h \
    giswnd.h \
    valuectl.h \
    ledctl.h \
    minewnd.h \
    scrolltabctl.h \
    rtwavewnd.h \
    wellwnd.h \
    statusbar.h \
    datamanager.h \
    communication.h \
    commset.h \
    testmap.h \
    rtdatatablewnd.h \
    serialport.h \
	modbus/modbus.h	\
	connectdb.h	\
	plotlines.h	\
	dbset.h

SOURCES += \
    mainwindow.cpp \
    main.cpp \
    menubar.cpp \
    toolbar.cpp \
    widget.cpp \
    giswnd.cpp \
    valuectl.cpp \
    ledctl.cpp \
    minewnd.cpp \
    scrolltabctl.cpp \
    rtwavewnd.cpp \
    wellwnd.cpp \
    statusbar.cpp \
    datamanager.cpp \
    communication.cpp \
    commset.cpp \
    testmap.cpp \
    rtdatatablewnd.cpp \
    serialport.cpp	\
	connectdb.cpp	\
	plotlines.cpp	\
	dbset.cpp

INCLUDEPATH += $(QTDIR)/include/QWT
INCLUDEPATH += opmap

Debug:LIBS += qwtd.lib \
              opmap/SDKlib/debug/opmapwidget.lib \
			  modbus/lib/debug/libModbus3.0.6.lib

Release:LIBS += qwt.lib \
                opmap/SDKlib/release/opmapwidget.lib \
				modbus/lib/release/libModbus3.0.6.lib

TRANSLATIONS = language_en.ts language_cn.ts

QT += widgets
QT += serialport
QT += sql

DEFINES += QWT_DLL

Release:DESTDIR += ./bin
