#ifndef IMGCUTER_H
#define IMGCUTER_H

#include <QtGui/QMainWindow>
#include "ui_imgcuter.h"
#include "graphicsview.h"
#include "graphicsscene.h"
#include "graphicsitem.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QRectF>
#include <QLabel>
#include <QPointF>
#include <QMap>
#include <QComboBox>
#include <QAction>

class imgCuter : public QMainWindow
{
	Q_OBJECT

public:
	imgCuter(QWidget *parent = 0, Qt::WFlags flags = 0);
	~imgCuter();

	GraphicsScene scene;

private:
	Ui::imgCuterClass ui;
	GraphicsView * graphicsView;

public:
	void createActions();

	QMap<QString,QString> mapTextures;
	QString directory;
	//int z;

	QGraphicsItem * lineLeft;
	QGraphicsItem * lineRight;
	QGraphicsItem * lineTop;
	QGraphicsItem * lineBottom;

	QPointF oldPos;
	QPointF curPos;
	QPointF oldItemPos;
	//QPointF rPos;
	float sceneScale;

	QRectF oldRect;
	QRectF newRect;

	//GraphicsRectItem * movable;	

	GraphicsRectItem * selectedItem;
	QGraphicsPixmapItem * texture;
	QPixmap pixmapTexture;

	GraphicsRectItem * selectionRect;
	GraphicsRectItem * selRL;
	GraphicsRectItem * selRLT;
	GraphicsRectItem * selRLB;
	GraphicsRectItem * selRT;
	GraphicsRectItem * selRB;
	GraphicsRectItem * selRR;
	GraphicsRectItem * selRRT;
	GraphicsRectItem * selRRB;
	enum itemType {NONE, PIXMAP, SEL, L, LT, LB, T, B, R, RT, RB};
	void initializeRect();
	void setVisibleHolders(bool b);
	void rePutHolders();

	void updateTextureMap();

	int presedType;
	bool mousePresed;

	bool moveTexture;
	bool drugRect;


	void getMinimap(bool b=false);
	void getCrop();

	QAction * saveAction;
	QAction * textureUpdate;
	QAction * layerChes;
	QAction * zoomIn;
	QAction * zoomOne;
	QAction * zoomOut;
	

public slots:
	void loadTexture();
	void changeTexture();
	void saveData();
	//void resizeEvent(QResizeEvent *);

	void mouseMoveSlot(QGraphicsSceneMouseEvent * _e);
	void mousePressSlot(QGraphicsSceneMouseEvent * _e);
	void mouseReleaseSlot(QGraphicsSceneMouseEvent * _e);

	void moveRectUP();
	void moveRectDOWN();
	void moveRectLEFT();
	void moveRectRIGTH();
 
	void moveRectOnX(int );
	void moveRectOnY(int );
 
	void changeRectOnWidth(int);
	void changeRectOnHeight(int);

	void changeScale(QWheelEvent*);

	void setSprite(QString textureName, int x, int y, int w, int h);

	void slotTextureUpdate();;
	void slotLayerChes();;
	void slotZoomIn();;
	void slotZoomOne();;
	void slotZoomOut();;


signals:
	void movedRectToX(int);
	void movedRectToY(int);
	void sizedRectToW(int);
	void sizedRectToH(int);

	void saveingData(int,int,int,int, QString);

};

#endif // IMGCUTER_H
