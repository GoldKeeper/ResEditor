#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>

class GraphicsScene : public QGraphicsScene
{
	Q_OBJECT

public:

	GraphicsScene ( QObject * parent = 0 ):QGraphicsScene (parent){};
	GraphicsScene ( const QRectF & sceneRect, QObject * parent = 0 ):QGraphicsScene (sceneRect,parent){};
	GraphicsScene ( qreal x, qreal y, qreal width, qreal height, QObject * parent = 0 ):QGraphicsScene (x,y,width,height, parent ){};


	void mouseMoveEvent(QGraphicsSceneMouseEvent *);
  	void mousePressEvent(QGraphicsSceneMouseEvent *);
  	void mouseReleaseEvent(QGraphicsSceneMouseEvent *);
	

signals:

 	void mouseMoveSignal(QGraphicsSceneMouseEvent * _e);
 	void mousePressSignal(QGraphicsSceneMouseEvent * _e);
 	void mouseReleaseSignal(QGraphicsSceneMouseEvent * _e);
	


};

#endif // GRAPHICSVIEWEVENTED_H
