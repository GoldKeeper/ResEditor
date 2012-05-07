#include "graphicsscene.h"

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent * _e)
{
	emit mouseMoveSignal(_e);

	QGraphicsScene::mouseMoveEvent(_e);
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *_e)
{
	emit mousePressSignal(_e);
	QGraphicsScene::mousePressEvent(_e);
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *_e)
{
	emit mouseReleaseSignal(_e);
	QGraphicsScene::mouseReleaseEvent(_e);
}