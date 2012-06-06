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
#include <QTranslator>

#include <QMessageBox>
class imgCuter : public QMainWindow
{
	Q_OBJECT

public:
    imgCuter(QWidget *parent = 0, Qt::WFlags flags = 0, QString translateFile = QString());
	~imgCuter();

	GraphicsScene scene;
        QTranslator * translator;

private:
	Ui::imgCuterClass ui;
	GraphicsView * graphicsView;

public:
	void createActions();

	QMap<QString,QString> mapTextures;
	QString directory;	

	QGraphicsItem * lineLeft;
	QGraphicsItem * lineRight;
	QGraphicsItem * lineTop;
	QGraphicsItem * lineBottom;

	QPointF oldPos;
	QPointF curPos;
	QPointF oldItemPos;	
	float sceneScale;

	QRectF oldRect;
	QRectF newRect;	

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

        QAction * showSprites;
        QAction * showFrames;

        enum USEDPIX{SPRITE, FRAME};
        struct usedSectors
        {
           explicit usedSectors(const QString &_texture, int _x, int _y, int _w, int _h, USEDPIX _currentType)
                :texture(_texture), x(_x), y(_y), w(_w), h(_h), currentType(_currentType), gitem(NULL){}
            QString texture;
            int x,y,w,h;
            USEDPIX currentType;            
            GraphicsRectItem * gitem;
        };
        QList<usedSectors*> sectors;
        void addSector(const QString &texture, int x, int y, int w, int h, USEDPIX currentType)
        {
            //imgCtr->addSector("", 5,5,100,100, imgCuter::SPRITE);
            usedSectors *sector = new usedSectors(texture, x,y,w,h, currentType);
            sector->gitem = new GraphicsRectItem();
            scene.addItem((QGraphicsItem*)sector->gitem);
            sector->gitem->setRect(x,y, w,h);
            QPen pen = QPen(Qt::SolidLine);
            QBrush brush = QBrush(Qt::SolidPattern);

            switch(currentType){
            case SPRITE:
                pen.setColor(QColor(0,0,180));
                brush.setColor(QColor(100,255,100,50));
                break;
            case FRAME:
                pen.setColor(QColor(0,180,0));
                brush.setColor(QColor(100,100,255,50));
                break;
            }
            sector->gitem->setZValue(990);
            sector->gitem->setPen(pen);
            sector->gitem->setBrush(brush);            
           sectors.append(sector);
        }
        void clearSectors()
        {
            qDeleteAll(sectors);
            sectors.clear();
        }



public slots:
	void loadTexture();
	void changeTexture();
	void saveData();	

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

        void slotTextureUpdate();
        void slotLayerChes();
        void slotZoomIn();
        void slotZoomOne();
        void slotZoomOut();

        void slotSpritesShow(bool b);
        void slotFramesShow(bool b);
        void resetShownSectors();

signals:
	void movedRectToX(int);
	void movedRectToY(int);
	void sizedRectToW(int);
	void sizedRectToH(int);

	void saveingData(int,int,int,int, QString);

};

#endif // IMGCUTER_H
