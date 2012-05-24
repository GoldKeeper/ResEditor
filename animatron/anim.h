#ifndef ANIM_H
#define ANIM_H

#include "draw.h"
#include "sprite.h"
#include "frame.h"
#include <QMessageBox>
#include <QTimer>
#include <QDomDocument>

class anim: public QObject
{
	Q_OBJECT
public:
	anim(){construct(); }
	anim(int _z, int _offset_x, int _offset_y, int _time) 
		:z(_z),offset_x(_offset_x),offset_y(_offset_y),time(_time){construct(); }

	~anim(){ delete timer;}

	QDomNode node;
	bool newAnim;
	QList<frame*> frames;
	QList<frame*> framesToAnim;
	int visibleFrames;
	int timeForFrame;
	int currentFrame;

	int z;
	int offset_x;
	int offset_y;
	int time;
	draw * drawItem;
	WidgetControl * widget;
	bool border; 
	bool visible;
	QTimer * timer;

	QSpinBox * spinBox_time;

	void construct()
	{	drawItem = NULL; 
		timer=new QTimer();
		connect(timer, SIGNAL(timeout()), SLOT(slotTimeOut()));
		newAnim=true;
	}

	void updateAllZ()
	{
		QListIterator<frame*> fit(frames);
		while(fit.hasNext())
		{
			fit.next()->setZValue(z);
		}
	}

	void updateAllVisible()
	{
		QListIterator<frame*> fit(frames);
		while(fit.hasNext())
		{
			frame*frm = fit.next();
			frm->setVisible(visible);
		}
	}

	void updateAllUsible()
	{
		QListIterator<frame*> fit(frames);
		while(fit.hasNext())
		{
			frame*frm = fit.next();
			frm->setUsible(border);
		}
	}

	void activeFrames()
	{
		frame *frm;
		framesToAnim.clear();
		QListIterator<frame*>	fi(frames);
		while(fi.hasNext())
		{
			frm=fi.next();
			if(frm->visible) 
			{
				framesToAnim.append(frm);
				frm->graphicsItem->setVisible(false);
                                frm->grRectItem->setVisible(false);
			}
		}
		visibleFrames=framesToAnim.count();
		timeForFrame = time/visibleFrames;
	}
	void restoreFrames()
	{
		frame* frm;
		QListIterator<frame*>	fi(frames);
			while(fi.hasNext())
			{
				frm=fi.next();
					frm->graphicsItem->setVisible(frm->visible);
                                        frm->grRectItem->setVisible(frm->used);
			}
	}

	public slots:

		void setUsed(bool b){border = b; updateAllUsible();}
		void setVisible(bool b){visible = b;  updateAllVisible();}
		void setOffset_x(int _x){offset_x=_x; updateAllPos();}
		void setOffset_y(int _y){offset_y=_y; updateAllPos();}
		void set_z(int _z){z=_z; updateAllZ();}
		void setTime(int t){time=t; }
		void updateAllPos()
		{
			QListIterator<frame*> fit(frames);
			while(fit.hasNext())
			{
				frame * frm =fit.next();
                                frm->grRectItem->setRect(-drawItem->offset_x+offset_x, -drawItem->offset_y+offset_y, frm->tex_w,frm->tex_h);
				frm->graphicsItem->setPos(-drawItem->offset_x+offset_x,-drawItem->offset_y+offset_y);
			}
		}
	
		void slotStartAnimation()
		{				
			activeFrames();
			//QMessageBox::information(0,0,"frames "+QString::number(frames.count())+" "+QString::number(framesToAnim.count()));
			if(visible) 
			{currentFrame=0;  slotTimeOut(); timer->start(timeForFrame);}
		}
		void slotStopAnimation(){timer->stop();restoreFrames();}
		void slotTimeOut()
		{
			if(currentFrame)
			{
				framesToAnim[currentFrame-1]->graphicsItem->setVisible(false);
                                framesToAnim[currentFrame-1]->grRectItem->setVisible(false);
			}
			else
			{
				framesToAnim[visibleFrames-1]->graphicsItem->setVisible(false);
                                framesToAnim[visibleFrames-1]->grRectItem->setVisible(false);
			}
			framesToAnim[currentFrame]->graphicsItem->setVisible(true);
                        framesToAnim[currentFrame]->grRectItem->setVisible(framesToAnim[currentFrame]->used);
			currentFrame++;
			if(currentFrame==visibleFrames)currentFrame=0;
		}
};



#endif
