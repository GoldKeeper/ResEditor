#ifndef FRAME_H
#define FRAME_H

#include "graphicsunit.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include "draw.h"
#include "widgetcontrol.h"
#include <QDomDocument>

class frame: public GraphicsUnit
{
	Q_OBJECT
public:
    frame(QString _texture = QString(), int _x=0, int _y=0, int _w=0, int _h=0)
            :GraphicsUnit(_texture,_x,_y,_w,_h){ startSettings();}
	void startSettings(){visible=allVisible=used=allUsed=true; series=false; seriesNum=-1; newFrame=true;}
	
	QDomNode node;
	bool newFrame;
	bool series;
	int seriesNum;

        bool allUsed;
	bool allVisible;

	public slots:
		void setUsibility(bool b)
		{
			used = b; 
			if(allUsed)
                        {grRectItem->setVisible(used); }
			else
                        {grRectItem->setVisible(false);}
		}
		void setUsible(bool b)
		{
			allUsed = b; 
			if(allUsed)
                        {grRectItem->setVisible(used); }
			else
                        {grRectItem->setVisible(false);}
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
                void setZValue(int _z){	grRectItem->setZValue(_z); graphicsItem->setZValue(_z);}
		void setData(int _x,int _y,int _w,int _h, QString _tex){ tex_x=_x; tex_y=_y; tex_w=_w; tex_h=_h; texture=_tex;}
};


#endif
