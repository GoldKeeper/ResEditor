#ifndef DRAW_H
#define DRAW_H

#include <QObject>
#include <QGraphicsRectItem>

class draw : public QObject
{
	Q_OBJECT
	public:
	draw(){}
	draw(QString _name, int _w, int _h, int _offset_x, int _offset_y)
		:w(_w), h(_h), offset_x(_offset_x), offset_y(_offset_y){}

	QString name;
	int w;
	int h;
	int offset_x;
	int offset_y;
	QGraphicsRectItem * drawRect;

	public slots:
		void setOffset_x(int _x){offset_x=_x; drawRect->setRect(-offset_x, -offset_y, w, h); emit updatePos();
		}
		void setOffset_y(int _y){offset_y=_y; drawRect->setRect(-offset_x, -offset_y, w, h); emit updatePos();
		}
		void setW(int _w){w=_w; drawRect->setRect(-offset_x, -offset_y, w, h); emit updatePos();
		}
		void setH(int _h){h=_h; drawRect->setRect(-offset_x, -offset_y, w, h); emit updatePos();
		}

signals:
	void updatePos(void);

};


#endif
