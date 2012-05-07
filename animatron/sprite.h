#ifndef SPRITE_H
#define SPRITE_H

#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include "draw.h"
#include "widgetcontrol.h"
#include <QDomDocument>

class sprite: public QObject
{
	Q_OBJECT
public:
        sprite():newSprite(true){}
	sprite(int _z, int _dx, int _dy, QString _texture, int _x, int _y, int _w, int _h)
		:z(_z), offset_x(_dx), offset_y(_dy), texture(_texture), tex_x(_x), tex_y(_y), tex_w(_w), tex_h(_h), newSprite(true){}
	QDomNode node;

	int z;
	int offset_x;
	int offset_y;
	QString texture;
	int tex_x;
	int tex_y;
	int tex_w;
        int tex_h;
        bool newSprite;

	bool used;
	bool visible;

	WidgetControl * widget;
	QGraphicsPixmapItem * graphicsItem;
	QGraphicsRectItem * spriteRect;
	draw * drawItem;

	public slots:
		void setUsed(bool b){used = b; spriteRect->setVisible(b);}
		void setVisible(bool b){visible = b; graphicsItem->setVisible(b); spriteRect->setVisible(b);}

		void setOffset_x(int _x){offset_x=_x; updatePos();}
		void setOffset_y(int _y){offset_y=_y; updatePos();}
		void updatePos()
		{
			graphicsItem->setPos(-drawItem->offset_x+offset_x, -drawItem->offset_y+offset_y); 
			spriteRect->setRect(-drawItem->offset_x+offset_x, -drawItem->offset_y+offset_y, tex_w, tex_h);
		}

		void set_z(int _z){z=_z; graphicsItem->setZValue(_z); spriteRect->setZValue(_z);}
		void setData(int _x,int _y,int _w,int _h, QString _tex){ tex_x=_x; tex_y=_y; tex_w=_w; tex_h=_h;texture=_tex;}
};



#endif
