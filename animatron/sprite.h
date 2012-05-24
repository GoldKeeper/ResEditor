#ifndef SPRITE_H
#define SPRITE_H

#include "graphicsunit.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include "draw.h"
#include "widgetcontrol.h"
#include <QDomDocument>

class sprite: public /*QObject*/ GraphicsUnit
{
	Q_OBJECT
public:
    sprite(int _z=0, int _dx=0, int _dy=0, QString _texture = QString(), int _x=0, int _y=0, int _w=0, int _h=0)
                :GraphicsUnit(_texture,_x,_y,_w,_h), z(_z), offset_x(_dx), offset_y(_dy), newSprite(true){}
	QDomNode node;

	int z;
	int offset_x;
	int offset_y;
        bool newSprite;

	public slots:
                void setUsed(bool b){used = b; grRectItem->setVisible(b);}
                void setVisible(bool b){visible = b; graphicsItem->setVisible(b); grRectItem->setVisible(b);}

		void setOffset_x(int _x){offset_x=_x; updatePos();}
		void setOffset_y(int _y){offset_y=_y; updatePos();}
		void updatePos()
		{
			graphicsItem->setPos(-drawItem->offset_x+offset_x, -drawItem->offset_y+offset_y); 
                        grRectItem->setRect(-drawItem->offset_x+offset_x, -drawItem->offset_y+offset_y, tex_w, tex_h);
		}

                void set_z(int _z){z=_z; graphicsItem->setZValue(_z); grRectItem->setZValue(_z);}
		void setData(int _x,int _y,int _w,int _h, QString _tex){ tex_x=_x; tex_y=_y; tex_w=_w; tex_h=_h;texture=_tex;}
};



#endif
