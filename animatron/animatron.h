#ifndef ANIMATRON_H
#define ANIMATRON_H

#include <QtGui/QMainWindow>
#include "ui_animatron.h"

#include "imgcuter/imgcuter.h"

#include "draw.h"
#include "sprite.h"
#include "frame.h"
#include "anim.h"
#include "types.h"
#include "widgetcontrol.h"
#include <QMessageBox>
#include <QGraphicsView>
#include <QCheckBox>
#include <QFrame>
#include <QAction>
#include <QToolBar>
#include <QDomDocument>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTranslator>

#include "scrolleventfilter.h"

class animatron : public QMainWindow
{
	Q_OBJECT

public:
        animatron(QWidget *parent = 0, Qt::WFlags flags = 0, QString translateFile = QString());
	~animatron();

	imgCuter *imgCtr;
        QTranslator * translator;

        void setupUiImg();//setup icons to arrows for move object and connect slots for it
        void connectUi();//connects some ui widgets
	
        void createActions();//K.O.


        void fillTree();//filling tree, connecting objects, set UI setups
        void setSettings(QDomNode _node);//convert DOM node to objects Sptites frames and etc
        void deleteUslessNodes();//romove prepeared useless nodes

        void deleteAllObjects();//used in destructor

	QGraphicsPixmapItem * createGraphicsItem(QString texture, int x, int y, int w, int h, QLabel * label=NULL);

        void closeEvent(QCloseEvent *);
        void keyPressEvent(QKeyEvent *);
        //{delete this;}

	QString dir;
        QMap<QString, QString> allTextures;//<name, path>
	QDomNode node;
	QList<QDomNode> uslessNodes;

private:
	Ui::animatronClass ui;
	QGraphicsScene scene;
	draw * drawObject;
	QList<sprite *> sprites;
	QList<anim *> animList;
	QGraphicsItem *lineH, *lineV;
	TreeWidgetItemAnim * spriteTreeItem;
	TreeWidgetItemAnim * animationsTreeItem;
	bool ground;
	bool greed;
	bool transp;
	void setBackground();
	int zoomState;
	void setEnabledAction(int);
	void addNewSprite(int _z, int _dx, int _dy, QString _texture, int _x, int _y, int _w, int _h);
	void addNewAnim(int _z, int _offset_x, int _offset_y, int _time);
	void addNewFrame(QString _texture, int _x, int _y, int _w, int _h);

	void changeActiveWidget( QTreeWidgetItem* currentItem, QTreeWidgetItem*lastItem );

        void updateGrUnit(GraphicsUnit* gu);

        void prepeareSectors();

        bool isedited;
        bool tryExitNotSaved();

        ScrollEventFilter * scrollEventFilter;
	
public:
	QAction * actStartAnimation;
	QAction * actStopAnimation;
	QAction * actPauseAnimation;

	QAction * actSaveDraw;

	QAction * actZoomIn;
	QAction * actZoomONE;
	QAction * actZoomOut;
	QAction * actGround;
	QAction * actGreed;
	QAction * actTransparent;

	QAction * actAddSprite;
	QAction * actRemoveSprite;
	QAction * actEditSprite;
	QAction * actAddAnimation;
	QAction * actRemoveAnimation;
	QAction * actAddFrame;
	QAction * actRemoveFrame;
	QAction * actEditFrame;
	QAction * actMoveFrameUp;
	QAction * actMoveFrameDown;


public slots:
		void startAnimation(bool _b)
		{	if (_b){emit animationStart();}
			else{emit animationStop();}
		}

		void stopAnimation(){actStartAnimation->setChecked(false);}
		void pauseAnimation(){}

		void slotSaveDraw();

		void slotZoomIn();
		void slotZoomONE();
		void slotZoomOut();
		void slotGround(bool);
		void slotGreed(bool);
		void slotTransparent(bool);

		void clickedOnWidget(QTreeWidgetItem * _treeWI){ ui.treeWidget->setCurrentItem(_treeWI);}
                void dbkClickedOnWidget(QTreeWidgetItem * _treeWI);

		void addSprite();
		void removeSprite();
		void editSprite();
		void addAnimation();
		void removeAnimation();
		void addFrame();
		void removeFrame();
		void removeFrame(QTreeWidgetItem *);
		void editFrame();
		void moveFrameUp();
		void moveFrameDown();

		void saveEditedImg(int,int,int,int, QString);
		

		void changeActiveItem(QTreeWidgetItem*, QTreeWidgetItem*);
                void slotTreeWidgetItemDoubleClicked(QTreeWidgetItem*, int);
                //void slotItemEntered ( QTreeWidgetItem * item, int column );
		void slotObjNameChanged(const QString &);	
		void moveT(){ui.spinBox_y->setValue(ui.spinBox_y->value()+1);}
		void moveL(){ui.spinBox_x->setValue(ui.spinBox_x->value()+1);}
		void moveR(){ui.spinBox_x->setValue(ui.spinBox_x->value()-1);}
		void moveB(){ui.spinBox_y->setValue(ui.spinBox_y->value()-1);}
		void moveLT(){moveL(); moveT();}
		void moveRT(){moveR(); moveT();}
		void moveLB(){moveL(); moveB();}
		void moveRB(){moveR(); moveB();}

		void updateScene();

                void edited();
                void saved();//edited

                void changeScale(QWheelEvent*);
                //void scrolled(int);

signals:
		void animationStart(void);
		void animationStop(void);
		void objectSaved(void);                

};

#endif // ANIMATRON_H
