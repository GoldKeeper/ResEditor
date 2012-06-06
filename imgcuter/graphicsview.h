#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QWheelEvent>

class GraphicsView : public QGraphicsView
{
	Q_OBJECT

public:
        GraphicsView( QWidget * _parent = 0 ):QGraphicsView ( _parent ){}
        GraphicsView ( QGraphicsScene * scene, QWidget * parent = 0 ):QGraphicsView ( scene,parent){}

//	~GraphicsView();

// 	void mouseMoveEvent(QMouseEvent *);
//   	void mousePressEvent(QMouseEvent *);
//   	void mouseReleaseEvent(QMouseEvent *);
	void wheelEvent(QWheelEvent *event){ emit mouseWheelSignal(event);}

signals:

	void mouseWheelSignal(QWheelEvent *event);
 	//void mouseMoveSignal(QMouseEvent * e);
//  	void mousePressSignal(QMouseEvent * e);
//  	void mouseReleaseSignal(QMouseEvent * e);
	


};

#endif // GRAPHICSVIEWEVENTED_H
