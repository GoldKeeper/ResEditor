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
//#include <QStackedWidget>
#include <QAction>
#include <QToolBar>
#include <QDomDocument>

#include <QMouseEvent>
#include <QWheelEvent>
#include <QTranslator>


class animatron : public QMainWindow
{
	Q_OBJECT

public:
        animatron(QWidget *parent = 0, Qt::WFlags flags = 0, QString translateFile = QString());
	~animatron();

	imgCuter *imgCtr;
        QTranslator * translator;

	void setupUiImg();
	void connectUi();
	
	void createActions();	


	void fillTree();
	void setSettings();
	void setSettings(QDomNode _node);
	void deleteUslessNodes();

	void deleteAllObjects();

	QGraphicsPixmapItem * createGraphicsItem(QString texture, int x, int y, int w, int h, QLabel * label=NULL);

 	void closeEvent(QCloseEvent *)
 	{delete this;}

	QString dir;
	QMap<QString, QString> allTextures;
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
	void updateSprite(sprite*);
	void updateFrame(frame* );
	
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
                void dbkClickedOnWidget(QTreeWidgetItem * _treeWI)
                {
                    editSprite();
                            //QMessageBox::information(0,0,"/* ui.treeWidget->setCurrentItem(_treeWI);*/");
                }

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
		
	public slots:
		void changeActiveItem(QTreeWidgetItem*, QTreeWidgetItem*);
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



signals:
		void animationStart(void);
		void animationStop(void);
		void objectSaved(void);

};

#endif // ANIMATRON_H
