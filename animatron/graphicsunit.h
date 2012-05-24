#ifndef GRAPHICSUNIT_H
#define GRAPHICSUNIT_H

#include <QObject>
#include <QString>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include "draw.h"
#include "widgetcontrol.h"


class GraphicsUnit: public QObject
{
public:
    explicit GraphicsUnit(QString _texture, int _x, int _y, int _w, int _h ,QObject * parent=0)
        :QObject(parent), texture(_texture),tex_x(_x),tex_y(_y),tex_w(_w),tex_h(_h){}

    QString texture;
    int tex_x;
    int tex_y;
    int tex_w;
    int tex_h;

    bool used;
    bool visible;

   WidgetControl * widget;

   QGraphicsPixmapItem * graphicsItem;
   QGraphicsRectItem *  grRectItem;//, *spriteRect;
   draw * drawItem;


};

#endif // GRAPHICSITEM_H
