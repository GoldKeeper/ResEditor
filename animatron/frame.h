#ifndef FRAME_H
#define FRAME_H

#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include "draw.h"
#include "widgetcontrol.h"
#include <QDomDocument>

class frame: public QObject
{
	Q_OBJECT
public:
	frame(){startSettings();}
	frame(QString _texture, int _x, int _y, int _w, int _h)
		:texture(_texture),tex_x(_x),tex_y(_y),tex_w(_w),tex_h(_h){ startSettings();};
	void startSettings(){visible=allVisible=used=allUsed=true; series=false; seriesNum=-1; newFrame=true;}
	QString texture;
	int tex_x;
	int tex_y;
	int tex_w;
	int tex_h;
	
	QDomNode node;
	bool newFrame;
	bool series;
	int seriesNum;

	bool used;
	bool allUsed;
	bool visible;
	bool allVisible;

	WidgetControl * widget;
	QGraphicsPixmapItem * graphicsItem;
	QGraphicsRectItem * frmRect;
	draw * drawItem;

	public slots:
		void setUsibility(bool b)
		{
			used = b; 
			if(allUsed)
			{frmRect->setVisible(used); }
			else
			{frmRect->setVisible(false);}
		}
		void setUsible(bool b)
		{
			allUsed = b; 
			if(allUsed)
			{frmRect->setVisible(used); }
			else
			{frmRect->setVisible(false);}
		}

		void setVisiblity(bool b)
		{
			visible = b;
			if(allVisible)
				{graphicsItem->setVisible(visible);}
			else
				{graphicsItem->setVisible(false);}
		}
		void setVisible(bool b)
		{
			allVisible = b;
			if(allVisible)
				{graphicsItem->setVisible(visible);}
			else
				{graphicsItem->setVisible(false);}
		}
		void setZValue(int _z){	frmRect->setZValue(_z); graphicsItem->setZValue(_z);}
		void setData(int _x,int _y,int _w,int _h, QString _tex){ tex_x=_x; tex_y=_y; tex_w=_w; tex_h=_h; texture=_tex;}
};


#endif