#include "imgcuter.h"
#include "graphicsitem.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QRectF>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QMessageBox>
#include <QWheelEvent>


imgCuter::imgCuter(QWidget *parent, Qt::WFlags flags, QString translateFile)
	: QMainWindow(parent, flags)
{
    translator = new QTranslator(this);
    if (!translateFile.isEmpty())
    {
        translator->load(translateFile);
    }
    qApp->installTranslator(translator);

	ui.setupUi(this);        
        graphicsView = new GraphicsView(ui.centralWidget);
	graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
	ui.gridLayout_6->addWidget(graphicsView, 0, 0, 1, 1);
	ui.statusBar->showMessage("Ready");
	graphicsView->setMouseTracking(true);
	createActions();
	ui.btnSave->setIcon(QPixmap(":/ResEditor/Resources/imgcuter/disk.png"));

	scene.setSceneRect(0,0,10, 10);
	
	sceneScale=0;

	presedType = imgCuter::NONE;
	mousePresed=false;
	selectedItem=NULL;

	texture=NULL;

	connect( graphicsView, SIGNAL(mouseWheelSignal(QWheelEvent*)), this, SLOT(changeScale(QWheelEvent*)));

	connect( &scene, SIGNAL( mouseMoveSignal(QGraphicsSceneMouseEvent *)),SLOT(mouseMoveSlot( QGraphicsSceneMouseEvent *)));
	connect( &scene, SIGNAL( mousePressSignal(QGraphicsSceneMouseEvent *)),SLOT(mousePressSlot( QGraphicsSceneMouseEvent *)));
	connect( &scene, SIGNAL( mouseReleaseSignal(QGraphicsSceneMouseEvent *)),SLOT(mouseReleaseSlot( QGraphicsSceneMouseEvent *)));

	connect(ui.btnLeft, SIGNAL(clicked()),SLOT(moveRectLEFT()));
	connect(ui.btnRight, SIGNAL(clicked()),SLOT(moveRectRIGTH()));
	connect(ui.btnUp, SIGNAL(clicked()),SLOT(moveRectUP()));
	connect(ui.btnDown, SIGNAL(clicked()),SLOT(moveRectDOWN()));

	connect(ui.spb_X, SIGNAL(valueChanged (int)),SLOT(moveRectOnX(int)));
	connect(ui.spb_Y, SIGNAL(valueChanged (int)),SLOT(moveRectOnY(int)));
	connect(ui.spb_W, SIGNAL(valueChanged (int)),SLOT(changeRectOnWidth( int)));
	connect(ui.spb_H, SIGNAL(valueChanged (int)),SLOT(changeRectOnHeight( int)));

	connect(this, SIGNAL(movedRectToX(int )), ui.spb_X,SLOT(setValue(int))) ;
	connect(this, SIGNAL(movedRectToY(int )), ui.spb_Y, SLOT(setValue( int))) ;
	connect(this, SIGNAL(sizedRectToW(int )), ui.spb_W, SLOT(setValue( int))) ;
	connect(this, SIGNAL(sizedRectToH(int )), ui.spb_H, SLOT(setValue( int))) ;

	graphicsView->setBackgroundBrush(QPixmap(":/ResEditor/Resources/imgcuter/bg.png"));
	graphicsView->setScene((QGraphicsScene*)&scene);

	initializeRect();

	ui.btnChangeTexture->hide();
	connect(ui.btnChangeTexture, SIGNAL(clicked()), SLOT(loadTexture()));

	connect(ui.comboBoxTextures, SIGNAL(currentIndexChanged ( int)),SLOT(changeTexture()));
	connect(ui.btnSave, SIGNAL(clicked()),SLOT(saveData()));


 	directory="NOT initialized";
	updateTextureMap();

	ui.spb_Z->setVisible(false);
        ui.label_5->setVisible(false);
}

imgCuter::~imgCuter()
{
    clearSectors();
    qApp->removeTranslator(translator);
}

void imgCuter::updateTextureMap()
{
	QList<QString> qstrl=mapTextures.keys();
	for(int i=0; i<qstrl.count(); i++)
		ui.comboBoxTextures->addItem(QPixmap(":/ResEditor/Resources/imgcuter/image.png") ,qstrl.value(i));
}

void imgCuter::initializeRect()
{	
	QPen pen = QPen(Qt::DotLine);	
	QBrush brush = QBrush(Qt::SolidPattern);
	brush.setColor(Qt::white);
	 	lineTop = scene.addLine(-20000, 0, 20000, 0, pen);
	 	lineLeft = scene.addLine(0, -20000, 0, 20000, pen);
	 	lineRight = scene.addLine(0, -20000, 0, 20000, pen); 
	 	lineBottom = scene.addLine(-20000, 0, 200000, 0, pen);
        selectionRect = new GraphicsRectItem();
	scene.addItem((QGraphicsItem*)selectionRect);        
	selectionRect->setRect(0,0,20,20);
	selectionRect->itemType=imgCuter::SEL;
	selectionRect->setCursor(Qt::SizeAllCursor);
	selectionRect->setZValue(999);
	QPen rectPen(Qt::DotLine);
	//rectPen.setWidth(2);
	selectionRect->setPen(rectPen);
	selRL = new GraphicsRectItem();
	selRL->setZValue(1000);
	selRL->itemType=imgCuter::L;
	selRL->setCursor(Qt::SizeHorCursor);
	selRL->setBrush(brush);
	scene.addItem((QGraphicsItem*)selRL);
	selRLT = new GraphicsRectItem();
	selRLT->setZValue(1000);
	selRLT->itemType=imgCuter::LT;
	selRLT->setCursor(Qt::SizeFDiagCursor);
	selRLT->setBrush(brush);
	scene.addItem((QGraphicsItem*)selRLT);
	selRLB = new GraphicsRectItem();
	selRLB->setZValue(1000);
	selRLB->itemType=imgCuter::LB;
	selRLB->setCursor(Qt::SizeBDiagCursor);
	selRLB->setBrush(brush);
	scene.addItem((QGraphicsItem*)selRLB);
	selRT = new GraphicsRectItem();
	selRT->setZValue(1000);
	selRT->itemType=imgCuter::T;
	selRT->setCursor(Qt::SizeVerCursor);
	selRT->setBrush(brush);
	scene.addItem((QGraphicsItem*)selRT);
	selRB	= new GraphicsRectItem();
	selRB->setZValue(1000);
	selRB->itemType=imgCuter::B;
	selRB->setCursor(Qt::SizeVerCursor);
	selRB->setBrush(brush);
	scene.addItem((QGraphicsItem*)selRB);
	selRR = new GraphicsRectItem();
	selRR->setCursor(Qt::SizeHorCursor);
	selRR->setBrush(brush);
	selRR->setZValue(1000);
	selRR->itemType=imgCuter::R;
	scene.addItem((QGraphicsItem*)selRR);
	selRRT = new GraphicsRectItem();
	selRRT->setCursor(Qt::SizeBDiagCursor);
	selRRT->setBrush(brush);
	selRRT->setZValue(1000);
	selRRT->itemType=imgCuter::RT;
	scene.addItem((QGraphicsItem*)selRRT);
	selRRB = new GraphicsRectItem();
	selRRB->setCursor(Qt::SizeFDiagCursor);
	selRRB->setBrush(brush);
	selRRB->setZValue(1000);
	selRRB->itemType=imgCuter::RB;
	scene.addItem((QGraphicsItem*)selRRB);
	rePutHolders();
}

void imgCuter::setVisibleHolders( bool b )
{
	selRL->setVisible(b);
	selRLT->setVisible(b);
	selRLB->setVisible(b);
	selRT->setVisible(b);
	selRB->setVisible(b);
	selRR->setVisible(b);
	selRRT->setVisible(b);
	selRRB->setVisible(b);
}

void imgCuter::createActions()
{
    saveAction = ui.mainToolBar->addAction(QPixmap(":/ResEditor/Resources/imgcuter/disk.png") ,tr("Сохранить"));
connect(saveAction,SIGNAL(triggered()), this, SLOT(saveData()));
textureUpdate = ui.mainToolBar->addAction(QPixmap(":/ResEditor/Resources/imgcuter/arrow-circle-double.png"),tr("Обновить текстуру"));
connect(textureUpdate,SIGNAL(triggered()),this,SLOT(slotTextureUpdate()));
layerChes = ui.mainToolBar->addAction(QPixmap(":/ResEditor/Resources/imgcuter/layer-transparent.png"),tr("Шахматный фон"));
connect(layerChes,SIGNAL(triggered()),this,SLOT(slotLayerChes()));
	layerChes->setCheckable(true); 
	layerChes->setChecked(true);
        zoomIn = ui.mainToolBar->addAction(QPixmap(":/ResEditor/Resources/imgcuter/magnifier-zoom-in.png"),tr("Увеличить"));
connect(zoomIn,SIGNAL(triggered()),this,SLOT(slotZoomIn()));
zoomOne = ui.mainToolBar->addAction(QPixmap(":/ResEditor/Resources/imgcuter/magnifier-zoom-actual.png"),tr("Масштаб 1:1"));
connect(zoomOne,SIGNAL(triggered()),this,SLOT(slotZoomOne()));
zoomOut = ui.mainToolBar->addAction(QPixmap(":/ResEditor/Resources/imgcuter/magnifier-zoom-out.png"),tr("Уменьшить"));
connect(zoomOut,SIGNAL(triggered()),this,SLOT(slotZoomOut()));

showSprites=ui.mainToolBar->addAction(QPixmap(":/ResEditor/Resources/animatron/images.png"), tr("Спрайты"));
showSprites->setCheckable(true);
showSprites->setChecked(true);
connect(showSprites, SIGNAL(triggered(bool)), SLOT(slotSpritesShow(bool)));

showFrames=ui.mainToolBar->addAction(QPixmap(":/ResEditor/Resources/animatron/films.png"), tr("Фреймы"));
showFrames->setCheckable(true);
showFrames->setChecked(true);
connect(showFrames, SIGNAL(triggered(bool)), SLOT(slotFramesShow(bool)));

}



void imgCuter::loadTexture()
{
	pixmapTexture = QPixmap("img3.png");
	scene.setSceneRect(pixmapTexture.rect());
	if (texture!=NULL) scene.removeItem(texture);
	texture =  scene.addPixmap(pixmapTexture);
	texture->setZValue(1);
	lineRight->setPos(pixmapTexture.width(), 0);
	lineBottom->setPos(0, pixmapTexture.height());
}

void imgCuter::changeTexture()
{
	QString str = directory+"/"+mapTextures.value(ui.comboBoxTextures->currentText());

	setWindowTitle(str);
	pixmapTexture = QPixmap(str);
	scene.setSceneRect(pixmapTexture.rect());

	if (texture!=NULL) scene.removeItem(texture);
	texture =  scene.addPixmap(pixmapTexture);
	texture->setZValue(1);

	lineRight->setPos(pixmapTexture.width(), 0);
	lineBottom->setPos(0, pixmapTexture.height());	

        resetShownSectors();
}


void imgCuter::rePutHolders()
{	
	selRL->setRect(selectionRect->rect().left()-6, selectionRect->rect().top()+selectionRect->rect().height()/2-3,5,5);
 	selRLT->setRect(selectionRect->rect().left()-6,selectionRect->rect().top()-6,5,5);
 	selRLB->setRect(selectionRect->rect().left()-6,selectionRect->rect().bottom()+1,5,5);
 	selRT->setRect(selectionRect->rect().left()+selectionRect->rect().width()/2-3,selectionRect->rect().top()-6,5,5);
 	selRB->setRect(selectionRect->rect().left()+selectionRect->rect().width()/2-3,selectionRect->rect().bottom()+1,5,5);
 	selRR->setRect(selectionRect->rect().right()+1,selectionRect->rect().top()+selectionRect->rect().height()/2-3,5,5);
 	selRRT->setRect(selectionRect->rect().right()+1,selectionRect->rect().top()-6,5,5);
 	selRRB->setRect(selectionRect->rect().right()+1,selectionRect->rect().bottom()+1,5,5);
}


void imgCuter::mouseMoveSlot( QGraphicsSceneMouseEvent * _e)
{
	curPos=_e->scenePos();
	int xPos = curPos.rx();
	int yPos = curPos.ry();
	int xTPos = graphicsView->mapToScene(xPos, yPos).x();
	int yTPos = graphicsView->mapToScene(xPos, yPos).y();
	QString msg="";

	if (mousePresed)
	{
		msg+="pressed";
		int newX=0;
		int newY=0;
		int newWidth=0;
		int newHeight=0;
		if(selectedItem!=NULL)
		switch(selectedItem->type())
		{
			case imgCuter::SEL :	newX=oldRect.x()-oldPos.rx()+curPos.rx();
									newY = oldRect.y()-oldPos.ry()+curPos.ry();
									if (newX<0)newX=0; if(newY<0) newY=0;
									selectedItem->setRect(newX, newY, oldRect.width(), oldRect.height());
									rePutHolders(); 
									break;
			case imgCuter::L : 
							    newX=oldRect.x()-oldPos.rx()+curPos.rx();
								newWidth=oldRect.width()+oldRect.x()-newX;
 							   if (newX<0){newX=0; newWidth=oldRect.width()+oldRect.x()-newX;}
 							   if (newX>=(oldRect.x()+oldRect.width()-1)){newX=oldRect.x()+oldRect.width()-1;newWidth=oldRect.width()+oldRect.x()-newX;}
								selectionRect->setRect(newX, oldRect.y(), newWidth, oldRect.height());
								rePutHolders();
								break;
			case imgCuter::R : 
								newWidth=oldRect.width() -oldPos.rx()+curPos.rx();
								if((newWidth)<1)newWidth=1;
								selectionRect->setRect(oldRect.x(), oldRect.y(), newWidth, oldRect.height()); 
								rePutHolders();
								break;
			case imgCuter::T :	
								newY=oldRect.y()-oldPos.ry()+curPos.ry();
								newHeight=oldRect.height() +oldRect.y()-newY;
								if (newY<0){newY=0; newHeight=oldRect.y()+oldRect.height()-newY;}
								if (newY>=(oldRect.y()+oldRect.height()-1)){newY=oldRect.y()+oldRect.height()-1; newHeight=oldRect.height()+oldRect.y()-newY;}
								selectionRect->setRect(oldRect.x(), newY, oldRect.width(), newHeight);
								rePutHolders();
								break;
			case imgCuter::B : 
								newHeight=oldRect.height() -oldPos.ry()+curPos.ry(); 
								if((newHeight)<1)newHeight=1;
								selectionRect->setRect(oldRect.x(), oldRect.y(), oldRect.width(), newHeight); 
								rePutHolders();
								break;
			case imgCuter::LT : 
								newX=oldRect.x()-oldPos.rx()+curPos.rx();
								newWidth=oldRect.width()+oldRect.x()-newX;
								 if (newX<0){newX=0; newWidth=oldRect.width()+oldRect.x()-newX;}
								 if (newX>=(oldRect.x()+oldRect.width()-1)){newX=oldRect.x()+oldRect.width()-1;newWidth=oldRect.width()+oldRect.x()-newX;}
								newY=oldRect.y()-oldPos.ry()+curPos.ry();
								newHeight=oldRect.height() +oldRect.y()-newY;
								 if (newY<0){newY=0; newHeight=oldRect.y()+oldRect.height()-newY;}
								 if (newY>=(oldRect.y()+oldRect.height()-1)){newY=oldRect.y()+oldRect.height()-1; newHeight=oldRect.height()+oldRect.y()-newY;}
								selectionRect->setRect(newX, newY, newWidth, newHeight);
								rePutHolders();
								break;
			case imgCuter::LB : 
								newX=oldRect.x()-oldPos.rx()+curPos.rx();
								newWidth=oldRect.width()+oldRect.x()-newX;
								if (newX<0){newX=0; newWidth=oldRect.width()+oldRect.x()-newX;}
								if (newX>=(oldRect.x()+oldRect.width()-1)){newX=oldRect.x()+oldRect.width()-1;newWidth=oldRect.width()+oldRect.x()-newX;}
								newHeight=oldRect.height() -oldPos.ry()+curPos.ry(); 
								if((newHeight)<1)newHeight=1;
								selectionRect->setRect(newX, oldRect.y(), newWidth, newHeight); 
								rePutHolders();				
								break;
			case imgCuter::RT :
								newY=oldRect.y()-oldPos.ry()+curPos.ry();
								newHeight=oldRect.height() +oldRect.y()-newY;
								if (newY<0){newY=0; newHeight=oldRect.y()+oldRect.height()-newY;}
								if (newY>=(oldRect.y()+oldRect.height()-1)){newY=oldRect.y()+oldRect.height()-1; newHeight=oldRect.height()+oldRect.y()-newY;}
								newWidth=oldRect.width() -oldPos.rx()+curPos.rx();
								if((newWidth)<1)newWidth=1;
								selectionRect->setRect(oldRect.x(), newY, newWidth, newHeight);
								rePutHolders();
								break;
			case imgCuter::RB : 
								newWidth=oldRect.width() -oldPos.rx()+curPos.rx();
								if((newWidth)<1)newWidth=1;
								newHeight=oldRect.height() -oldPos.ry()+curPos.ry(); 
								if((newHeight)<1)newHeight=1;
								selectionRect->setRect(oldRect.x(), oldRect.y(), newWidth, newHeight);
								rePutHolders();
								break;
		}
	}
	else 
	{
		msg+="relised";
	}

	msg += QString("on screen:(%1, %2);  on texture:(%3, %4)").arg(xPos).arg(yPos).arg(xTPos).arg(yTPos);
	ui.statusBar->showMessage(msg);
	getCrop();
	getMinimap();
}

void imgCuter::mousePressSlot( QGraphicsSceneMouseEvent * _e )
{
	mousePresed=true;
	QGraphicsItem* gi= scene.itemAt(_e->scenePos());
	if (gi==0)return;
	if (gi->zValue()>=999)
	{
		selectedItem=(GraphicsRectItem*)gi;
		oldItemPos=selectedItem->scenePos();
		oldRect=selectionRect->rect();
	}
	else selectedItem=NULL;
	oldPos=_e->scenePos();

	 		disconnect(ui.spb_X, SIGNAL(valueChanged (int)),this, SLOT(moveRectOnX(int)));
	 		disconnect(ui.spb_Y, SIGNAL(valueChanged (int)),this, SLOT(moveRectOnY(int)));
			disconnect(ui.spb_W, SIGNAL(valueChanged (int)),this,SLOT(changeRectOnWidth( int)));
			disconnect(ui.spb_H, SIGNAL(valueChanged (int)),this,SLOT(changeRectOnHeight( int)));
}

void imgCuter::mouseReleaseSlot( QGraphicsSceneMouseEvent * _e )
{
	if(!mousePresed)return;
	mousePresed=false;
	selectedItem=NULL;

	 	emit movedRectToX(selectionRect->rect().x());
	 	emit movedRectToY(selectionRect->rect().y());
	 	emit sizedRectToW(selectionRect->rect().width());
	 	emit sizedRectToH(selectionRect->rect().height());
	 		connect(ui.spb_X, SIGNAL(valueChanged (int)),SLOT(moveRectOnX(int)));
	 		connect(ui.spb_Y, SIGNAL(valueChanged (int)),SLOT(moveRectOnY(int)));
			connect(ui.spb_W, SIGNAL(valueChanged (int)),SLOT(changeRectOnWidth( int)));
			connect(ui.spb_H, SIGNAL(valueChanged (int)),SLOT(changeRectOnHeight( int)));
}

void imgCuter::getMinimap(bool b)
{
	if(texture!=NULL)
	{
		QPixmap croped=pixmapTexture;

		qreal rectDiv=1;
			if(pixmapTexture.rect().height()!=0)
			rectDiv  = pixmapTexture.rect().width()/pixmapTexture.rect().height();
		qreal labelDiv=1;
			if(ui.label_map->height()!=0)
			labelDiv = ((qreal)(ui.label_map->width())/(qreal)(ui.label_map->height()));		
		if(rectDiv<=labelDiv)
		{
			if(croped.width()>ui.label_map->width());
			croped=croped.scaledToHeight(ui.label_map->height());//,Qt::SmoothTransformation);		
		}
		else if(rectDiv>labelDiv)
		{
			if(croped.height()>ui.label_map->height());
			croped=croped.scaledToWidth(ui.label_map->width());//, Qt::SmoothTransformation);
		}
		ui.label_map ->setPixmap(croped);
	}
}

void imgCuter::getCrop()
{
    //TODO: finde all call this function and remove all extra call (called too often)
    QRectF selRect =  selectionRect->rect();
	if(texture!=NULL)
	{
           if (selRect.x()>scene.width())selRect.setX(scene.width());
           if (selRect.y()>scene.height())selRect.setY(scene.height());
           if (selRect.right()>scene.width())selRect.setRight(scene.width());
           if (selRect.bottom()>scene.height())selRect.setBottom(scene.height());
           //ui.statusBar->showMessage(QString("%1 %2 %3 %4").arg(selRect.x()).arg(selRect.y()).arg(selRect.width()).arg(selRect.height()));

           QPixmap croped;
           if((selRect.width()<1)||(selRect.height()<1))
               croped=QPixmap();
           else
            croped=pixmapTexture.copy(selRect.x(), selRect.y(), selRect.width(), selRect.height());

            qreal rectDiv=1;
                if(selRect.height()!=0)
                rectDiv= selRect.width()/selRect.height();
            qreal labelDiv =1;
		if(ui.label_crop->height()!=0)
		labelDiv =((qreal)(ui.label_crop->width())/(qreal)(ui.label_crop->height()));
            if ((selRect.width()<=ui.label_crop->width())&&(selRect.height()<=ui.label_crop->height()))
            {
		//ui.label_crop ->setPixmap(croped);
            }
            else if(rectDiv<=labelDiv)
            {
		if(croped.width()>ui.label_crop->width());
		croped=croped.scaledToHeight(ui.label_crop->height());//,Qt::SmoothTransformation);		
            }
            else if(rectDiv>labelDiv)
            {
		if(croped.height()>ui.label_crop->height());
		croped=croped.scaledToWidth(ui.label_crop->width());//, Qt::SmoothTransformation);
            }
		ui.label_crop ->setPixmap(croped);
	}
}

void imgCuter::moveRectUP()
{
	int newY =selectionRect->rect().y()-ui.spinBoxDelta->value();
	if(newY<0)newY=0;
	selectionRect->setRect(selectionRect->rect().x(), newY,selectionRect->rect().width(),selectionRect->rect().height());
	getCrop();
	getMinimap();
	rePutHolders();
	
	disconnect(ui.spb_Y, SIGNAL(valueChanged (int)),this,SLOT(moveRectOnY(int)));
	ui.spb_Y->setValue(newY);
	connect(ui.spb_Y, SIGNAL(valueChanged (int)),SLOT(moveRectOnY(int)));
}

void imgCuter::moveRectDOWN()
{
	int newY =selectionRect->rect().y()+ui.spinBoxDelta->value();
	selectionRect->setRect(selectionRect->rect().x(), newY, selectionRect->rect().width(),selectionRect->rect().height());
	getCrop();
	getMinimap();
	rePutHolders();
	disconnect(ui.spb_Y, SIGNAL(valueChanged (int)),this,SLOT(moveRectOnY(int)));
	ui.spb_Y->setValue(newY);
	connect(ui.spb_Y, SIGNAL(valueChanged (int)),SLOT(moveRectOnY(int)));
}

void imgCuter::moveRectLEFT()
{
	int newX =selectionRect->rect().x()-ui.spinBoxDelta->value();
	if(newX<0)newX=0;
	selectionRect->setRect(newX, selectionRect->rect().y(), selectionRect->rect().width(),selectionRect->rect().height());
	getCrop();
	getMinimap();
	rePutHolders();
	disconnect(ui.spb_X, SIGNAL(valueChanged (int)),this,SLOT(moveRectOnX(int)));
	ui.spb_X->setValue(newX);
	connect(ui.spb_X, SIGNAL(valueChanged (int)),SLOT(moveRectOnX(int)));
}

void imgCuter::moveRectRIGTH()
{
	int newX =selectionRect->rect().x()+ui.spinBoxDelta->value();
	selectionRect->setRect(newX, selectionRect->rect().y(), selectionRect->rect().width(),selectionRect->rect().height());
	getCrop();
	getMinimap();
	rePutHolders();
	disconnect(ui.spb_X, SIGNAL(valueChanged (int)),this,SLOT(moveRectOnX(int)));
	ui.spb_X->setValue(newX);
	connect(ui.spb_X, SIGNAL(valueChanged (int)),SLOT(moveRectOnX(int)));
}

void imgCuter::moveRectOnX(int _x)
{
	selectionRect->setRect(_x-1, selectionRect->rect().y(), selectionRect->rect().width(),selectionRect->rect().height());
	getCrop();
	getMinimap();
	rePutHolders();
}

void imgCuter::moveRectOnY(int _y)
{
	selectionRect->setRect(selectionRect->rect().x(), _y-1, selectionRect->rect().width(),selectionRect->rect().height());
 	getCrop();
 	getMinimap();
	rePutHolders();
}

void imgCuter::changeRectOnWidth( int _w)
{
	selectionRect->setRect(selectionRect->rect().x(), selectionRect->rect().y(), _w,selectionRect->rect().height());
	getCrop();
	getMinimap();
	rePutHolders();
}

void imgCuter::changeRectOnHeight( int _h)
{
	selectionRect->setRect(selectionRect->rect().x(), selectionRect->rect().y(), selectionRect->rect().width(),_h);
	getCrop();
	getMinimap();
	rePutHolders();
}

void imgCuter::saveData()
{
// 	QString qstr=QString("%1, %2, %3, %4; %5; ")
// 		.arg(ui.spb_X->value()).arg(ui.spb_Y->value()).arg(ui.spb_W->value()).arg(ui.spb_H->value())
// 		.arg(ui.comboBoxTextures->currentText())
// 		//.arg(ui.spb_Z->value())
// 		;
	//QMessageBox::information(this,0,  qstr);

	emit saveingData(ui.spb_X->value(),ui.spb_Y->value(),ui.spb_W->value(),ui.spb_H->value(), ui.comboBoxTextures->currentText());
}

void imgCuter::changeScale( QWheelEvent * e)
{
	
	if(e->delta()>0)
	{
		if(sceneScale<5)
		{
			sceneScale++;
			graphicsView->scale(2,2);	
		}
	}
	else
	{
		if(sceneScale>-5)
		{
			sceneScale--;
			graphicsView->scale(0.5,0.5);
		}
	}
}

void imgCuter::setSprite( QString textureName, int x, int y, int w, int h )
{
	if(mapTextures.contains(textureName))
	{ui.comboBoxTextures->setCurrentIndex(ui.comboBoxTextures->findText(textureName));}
	else 
	{
		if(ui.comboBoxTextures->count()==0)
			{scene.setSceneRect(-10,-10,20,20);}
		else
			{ui.comboBoxTextures->setCurrentIndex(0);}
        }
	ui.spb_X->setValue(x);
	ui.spb_Y->setValue(y);
	ui.spb_W->setValue(w);
	ui.spb_H->setValue(h);
        selectionRect->setRect(x,y,w,h);
	rePutHolders();
}

void imgCuter::slotTextureUpdate()
{
	changeTexture();
}

void imgCuter::slotLayerChes()
{
	if(layerChes->isChecked())
		graphicsView->setBackgroundBrush(QPixmap(":/ResEditor/Resources/imgcuter/bg.png"));
	else
		graphicsView->setBackgroundBrush(QPixmap(""));
}

void imgCuter::slotZoomIn()
{
	if(sceneScale<5)
	{
		sceneScale++;
		graphicsView->scale(2,2);	
	}
}

void imgCuter::slotZoomOne()
{
	while(sceneScale!=0)
	{
		if(sceneScale>0)
		{
			sceneScale--;
			graphicsView->scale(0.5,0.5);
		}	
		else if(sceneScale<0)
		{
			sceneScale++;
			graphicsView->scale(2,2);	
		}
	}
}

void imgCuter::slotZoomOut()
{
	if(sceneScale>-5)
	{
		sceneScale--;
		graphicsView->scale(0.5,0.5);
        }
}

void imgCuter::slotSpritesShow(bool b)
{
    resetShownSectors();
}

void imgCuter::slotFramesShow(bool b)
{
    resetShownSectors();
}

void imgCuter::resetShownSectors()
{
    QString currentTexture = ui.comboBoxTextures->currentText();
    QListIterator<usedSectors*>iterus(sectors);
    while(iterus.hasNext())
    {
        usedSectors* us= iterus.next();

        if(us->texture == currentTexture)
        {
            if ((us->currentType == SPRITE)&&showSprites->isChecked())
            {
                us->gitem->setVisible(true);
            }
            else if ((us->currentType == FRAME)&&showFrames->isChecked())
            {
                us->gitem->setVisible(true);
            }
            else
                us->gitem->setVisible(false);

        }
        else
        {
            us->gitem->setVisible(false);
        }
    }
}







