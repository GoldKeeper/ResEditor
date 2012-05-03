#ifndef GRAPHICSITEM_H
#define GRAPHICSITEM_H

#include <QGraphicsItem>
#include <QMouseEvent>
#include <QGraphicsRectItem>

class GraphicsItem : public QGraphicsItem
{
public:
	GraphicsItem ( QGraphicsItem * parent = 0 ):QGraphicsItem (parent){ };


};


class GraphicsRectItem : public QGraphicsRectItem
{
public:
	GraphicsRectItem ( QGraphicsItem * parent = 0 ):QGraphicsRectItem (parent){ };

	int itemType;
	int type(){return itemType;};

// 	void mousePressEvent(QGraphicsSceneMouseEvent *event){}
// 	void mouseMoveEvent(QGraphicsSceneMouseEvent *event){}
// 	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event){};
	
};









#endif // GRAPHICSVIEWEVENTED_H
