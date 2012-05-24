#-------------------------------------------------
#
# Project created by QtCreator 2011-12-15T22:15:21
#
#-------------------------------------------------

QT       += core gui xml

TARGET = ResEditor
TEMPLATE = app


SOURCES += main.cpp\
        reseditor.cpp \
    #xmlparser.cpp \
    imgcuter/imgcuter.cpp \
    imgcuter/graphicsview.cpp \
    imgcuter/graphicsscene.cpp \
    imgcuter/graphicsitem.cpp \
    animatron/animatron.cpp \
    resxmlparser.cpp

HEADERS  += reseditor.h \
    #xmlparser.h \
    #treewidgetitem.h \
    dlgtexture.h \
    imgcuter/imgcuter.h \
    imgcuter/graphicsview.h \
    imgcuter/graphicsscene.h \
    imgcuter/graphicsitem.h \
    animatron/widgetcontrol.h \
    animatron/types.h \
    animatron/sprite.h \
    animatron/frame.h \
    animatron/draw.h \
    animatron/animatron.h \
    animatron/anim.h \
    resxmlparser.h \
    animatron/graphicsunit.h

FORMS    += reseditor.ui \
    dialogTextures.ui \
    imgcuter/imgcuter.ui \
    animatron/widget.ui \
    animatron/animatron.ui

TRANSLATIONS += reseditor_ru.ts reseditor_eng.ts

CODECFORTR = UTF-8
CODECFORSRC = UTF-8

RESOURCES += \
    reseditor.qrc

























