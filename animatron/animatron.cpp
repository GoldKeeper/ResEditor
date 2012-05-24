#include "animatron.h"
#include <QLayout>
#include <QGridLayout> 
#include <QLabel>
#include <QSpacerItem> 
#include <QTreeWidget>
#include <QPen>


animatron::animatron(QWidget *parent, Qt::WFlags flags, QString translateFile)
	: QMainWindow(parent, flags)
{
        translator = new QTranslator(this);
        if (!translateFile.isEmpty())
        {
            translator->load(translateFile);
        }
        qApp->installTranslator(translator);


	ui.setupUi(this); 
        imgCtr=new imgCuter(this, Qt::Dialog, translateFile);

	connect(imgCtr, SIGNAL(saveingData(int,int,int,int, QString)),this, SLOT(saveEditedImg(int,int,int,int, QString)));

	ui.graphicsView->setScene(&scene);

	ground=true;
	greed=true;
	transp=false;
	zoomState=0;


	drawObject = new draw();
	setupUiImg();	

	scene.setSceneRect(-100,-100,200,200);
	createActions();	
	
	QPen pen = QPen(Qt::DotLine);
	lineH = scene.addLine(-20000, 0, 20000, 0, pen);
	lineV = scene.addLine(0, -20000, 0, 20000, pen);
}

animatron::~animatron()
{
	deleteAllObjects();
	delete imgCtr;
}


void animatron::fillTree()
{
	ui.spinBox_w->setValue(drawObject->w);
	ui.spinBox_h->setValue(drawObject->h);
	ui.spinBox_x->setValue(drawObject->offset_x);
	ui.spinBox_y->setValue(drawObject->offset_y);
	ui.le_name->setText(drawObject->name);

	ui.treeWidget->setRootIsDecorated(true);
	TreeWidgetItemAnim * qtwi = new TreeWidgetItemAnim (ui.treeWidget, TreeWidgetItemAnim::SPRITES);
	spriteTreeItem=	qtwi;
        qtwi->setText(0, tr("Cпрайты"));
		qtwi->setIcon(0,QPixmap(":/ResEditor/Resources/animatron/images.png"));
			qtwi->widget= new QWidget();
			QGridLayout * gridLayoutS = new QGridLayout(qtwi->widget);
			qtwi->	gridLayout= gridLayoutS;
			gridLayoutS->setContentsMargins(0,0,0,0);
			gridLayoutS->setSpacing(0);

			int i=0;
	QListIterator<sprite*> si(sprites);
	while(si.hasNext())
	{	
		sprite * ps =si.next();
		//создние своего итема и передача указателя на спрайт
		TreeWidgetItemAnim * qtwis = new TreeWidgetItemAnim((QTreeWidgetItem *)qtwi, TreeWidgetItemAnim::ONE_SPRITE);
                qtwis->setText(0,tr("Cпрайт"));
			qtwis->setIcon(0,QPixmap(":/ResEditor/Resources/animatron/image.png"));
			qtwis->oneSprite=ps;
			ps->widget = new WidgetControl();//new QWidget();
			ps->widget->treeWidgetItem=qtwis;
			ps->widget->setContentsMargins(0,0,0,0);
			ps->widget->ui.btn_up->setEnabled(false);
			ps->widget->ui.btn_down->setEnabled(false);
			ps->widget->ui.btn_center->setEnabled(false);
 			ps->widget->ui.spinBox_x->setValue(ps->offset_x);
 			ps->widget->ui.spinBox_y->setValue(ps->offset_y);
 			ps->widget->ui.spinBox_z->setValue(ps->z);
			ps->drawItem= drawObject;
                        ps->graphicsItem=createGraphicsItem(ps->texture, ps->tex_x, ps->tex_y, ps->tex_w, ps->tex_h, ps->widget->ui.label);
  			ps->graphicsItem->setZValue(ps->z);
			ps->graphicsItem->setPos(drawObject->drawRect->rect().x()+ps->offset_x, drawObject->drawRect->rect().y()+ps->offset_y);
                        ps->grRectItem=scene.addRect(drawObject->drawRect->rect().x()+ps->offset_x, drawObject->drawRect->rect().y()+ps->offset_y, ps->tex_w, ps->tex_h);
                        ps->grRectItem->setZValue(ps->z);
 				
 			ps->widget->ui.checkBox_use->setChecked(true); ps->used=true;
 			ps->widget->ui.checkBox_visible->setChecked(true); ps->visible=true;
 			connect(ps->widget->ui.checkBox_use, SIGNAL( toggled ( bool)), ps, SLOT(setUsed(bool)));
			connect(ps->widget->ui.checkBox_visible, SIGNAL( toggled ( bool)), ps, SLOT(setVisible(bool)));
			connect(ps->widget->ui.spinBox_x, SIGNAL(valueChanged ( int)), ps, SLOT(setOffset_x(int )));
			connect(ps->widget->ui.spinBox_y, SIGNAL(valueChanged ( int)), ps, SLOT(setOffset_y(int )));
			connect(ps->widget->ui.spinBox_z, SIGNAL(valueChanged ( int)), ps, SLOT(set_z(int )));

			connect(drawObject, SIGNAL(updatePos()), ps, SLOT(updatePos()));

                        connect(ps->widget, SIGNAL(clicked(QTreeWidgetItem *)), this, SLOT(clickedOnWidget(QTreeWidgetItem *)));
                        connect(ps->widget, SIGNAL(doubleClicked(QTreeWidgetItem *)), this, SLOT(dbkClickedOnWidget(QTreeWidgetItem*)));
				
			gridLayoutS->addWidget(ps->widget, i, 0, 1, 1);
			i++;
	}
	QSpacerItem * verticalSpacerS = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	gridLayoutS->addItem(verticalSpacerS, i, 0, 1, 1);

	qtwi = new TreeWidgetItemAnim (ui.treeWidget, TreeWidgetItemAnim::ANIMATIONS);
	animationsTreeItem=qtwi;
        qtwi->setText(0, tr("Анимации"));
		qtwi->setIcon(0,QPixmap(":/ResEditor/Resources/animatron/folder-open-film.png"));
	QListIterator<anim*> ai(animList);
	while(ai.hasNext())
	{	
		anim * pa =ai.next();
		//создние своего итема и передача указателя на спрайт
		TreeWidgetItemAnim * qtwia = new TreeWidgetItemAnim((QTreeWidgetItem *)qtwi, TreeWidgetItemAnim::ANIMATION_SET);
                qtwia->setText(0, tr("Анимация"));
		qtwia->setIcon(0,QPixmap(":/ResEditor/Resources/animatron/films.png"));
		qtwia->oneAnim=pa;
		qtwia->widget= new QWidget();
		QGridLayout * gridLayoutF = new QGridLayout(qtwia->widget);
		qtwia->gridLayout=gridLayoutF;
		gridLayoutF->setContentsMargins(0,0,0,0);
		gridLayoutF->setSpacing(0);

		i=0;
                //установка настроек анимации
		pa->widget = new WidgetControl();//new QWidget();
		pa->widget->ui.label->setText("Time");
		pa->spinBox_time = new QSpinBox(pa->widget->ui.label);
		pa->spinBox_time->setGeometry(pa->widget->ui.label->x(),pa->widget->ui.label->y(),60/*pa->widget->ui.label->width()-5*/, 20 );
		pa->spinBox_time->setMaximum(20000);
		pa->spinBox_time->setValue(pa->time);
		pa->widget->setContentsMargins(0,0,0,0);
		pa->widget->ui.btn_up->setEnabled(false);
		pa->widget->ui.btn_down->setEnabled(false);
		pa->widget->ui.btn_center->setEnabled(false);
		pa->widget->ui.spinBox_x->setValue(pa->offset_x);
		pa->widget->ui.spinBox_y->setValue(pa->offset_y);
		pa->widget->ui.spinBox_z->setValue(pa->z);
		pa->widget->ui.checkBox_use->setChecked(true); pa->border=true;
		pa->widget->ui.checkBox_visible->setChecked(true); pa->visible=true;
		pa->drawItem=drawObject;

		connect(pa->spinBox_time, SIGNAL(valueChanged(int)), pa, SLOT(setTime(int)));
		connect(pa->widget->ui.checkBox_use, SIGNAL( toggled ( bool)), pa, SLOT(setUsed(bool)));
		connect(pa->widget->ui.checkBox_visible, SIGNAL( toggled ( bool)), pa, SLOT(setVisible(bool)));
		
		connect(pa->widget->ui.spinBox_x, SIGNAL(valueChanged ( int)), pa, SLOT(setOffset_x(int )));
		connect(pa->widget->ui.spinBox_y, SIGNAL(valueChanged ( int)), pa, SLOT(setOffset_y(int )));
		connect(pa->widget->ui.spinBox_z, SIGNAL(valueChanged ( int)), pa, SLOT(set_z(int )));

		connect(drawObject, SIGNAL(updatePos()), pa, SLOT(updateAllPos()));

		connect(this, SIGNAL(animationStart()), pa, SLOT(slotStartAnimation()));
		connect(this, SIGNAL(animationStop()), pa, SLOT(slotStopAnimation()));

		gridLayoutF->addWidget(pa->widget, i, 0, 1, 1);
		i++;

		QListIterator<frame*> fi(pa->frames);
		while(fi.hasNext())
		{
			frame * frm = fi.next();
			TreeWidgetItemAnim * ftwi = new TreeWidgetItemAnim(qtwia, TreeWidgetItemAnim::ANIMATION_FRAME);
                        ftwi->setText(0, tr("Фрейм"));
			ftwi->setIcon(0,QPixmap(":/ResEditor/Resources/animatron/film.png"));
			ftwi->oneFrame=frm;

			frm->widget = new WidgetControl();//new QWidget();
			frm->widget->treeWidgetItem=ftwi;
			if(frm->series)frm->widget->lbl= new QLabel(QString("S%1").arg(frm->seriesNum), frm->widget->ui.label);
			frm->widget->setContentsMargins(0,0,0,0);
 			frm->widget->ui.btn_up->setEnabled(false);
			frm->widget->ui.btn_down->setEnabled(false);
			frm->widget->ui.btn_center->setEnabled(false);
				frm->widget->ui.btn_b->setEnabled(false);
				frm->widget->ui.btn_t->setEnabled(false);
				frm->widget->ui.btn_l->setEnabled(false);
				frm->widget->ui.btn_r->setEnabled(false);
				frm->widget->ui.btn_lt->setEnabled(false);
				frm->widget->ui.btn_rt->setEnabled(false);
				frm->widget->ui.btn_lb->setEnabled(false);
				frm->widget->ui.btn_rb->setEnabled(false);
			frm->widget->ui.spinBox_x->setEnabled(false);
			frm->widget->ui.spinBox_y->setEnabled(false);
			frm->widget->ui.spinBox_z->setEnabled(false);
			
			frm->widget->ui.checkBox_use->setChecked(true); frm->used=true;
			frm->widget->ui.checkBox_visible->setChecked(true); frm->visible=true;
			frm->drawItem= drawObject;
                        frm->graphicsItem=createGraphicsItem(frm->texture, frm->tex_x, frm->tex_y, frm->tex_w, frm->tex_h, frm->widget->ui.label);
 			frm->graphicsItem->setPos(drawObject->drawRect->rect().x()+pa->offset_x, drawObject->drawRect->rect().y()+pa->offset_y);
                        frm->grRectItem=scene.addRect(drawObject->drawRect->rect().x()+pa->offset_x, drawObject->drawRect->rect().y()+pa->offset_y, frm->tex_w, frm->tex_h);
			frm->graphicsItem->setZValue(pa->z);
                        frm->grRectItem->setZValue(pa->z);

			connect(frm->widget->ui.checkBox_visible, SIGNAL(toggled ( bool)), frm, SLOT(setVisiblity(bool)));
			connect(frm->widget->ui.checkBox_use, SIGNAL(toggled ( bool)), frm, SLOT(setUsibility(bool)));

                        connect(frm->widget, SIGNAL(clicked(QTreeWidgetItem *)), this, SLOT(clickedOnWidget(QTreeWidgetItem *)));
                        connect(frm->widget, SIGNAL(doubleClicked(QTreeWidgetItem *)), this, SLOT(dbkClickedOnWidget(QTreeWidgetItem*)));

			gridLayoutF->addWidget(frm->widget, i, 0, 1, 1);
			i++;
		}
		QSpacerItem * verticalSpacerF = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
		gridLayoutF->addItem(verticalSpacerF, i, 0, 1, 1);
	}
}


void animatron::setSettings( QDomNode _node )
{
	node = _node;
	drawObject->name=_node.toElement().attribute("name");
	QStringList qstrlS=_node.toElement().attribute("offset").split(",") ;
	if(qstrlS.count()==2)
	{
		drawObject->offset_x=qstrlS[0].toInt();
		drawObject->offset_y=qstrlS[1].toInt();
	}
	QStringList qstrlO=_node.toElement().attribute("size").split(",") ;
	if(qstrlO.count()==2)
	{
		drawObject->w=qstrlO[0].toInt();
		drawObject->h=qstrlO[1].toInt();
	}
        updateScene();        

	ui.spinBox_w->setValue(drawObject->w);
	ui.spinBox_h->setValue(drawObject->h);
	ui.spinBox_x->setValue(drawObject->offset_x);
	ui.spinBox_y->setValue(drawObject->offset_y);
	ui.le_name->setText(drawObject->name);
	connectUi();//!!!!!!!!!!!!!!!!!!!!!!!!!!!

	QPen pen = QPen(Qt::DotLine);
	pen.setColor(Qt::red);
	drawObject->drawRect= new QGraphicsRectItem(-drawObject->offset_x, -drawObject->offset_y, drawObject->w, drawObject->h,0,  &scene);
	drawObject->drawRect->setPen(pen);

	sprite * spr;
	 	QDomNodeList qdnd= _node.toElement().elementsByTagName("sprite");
	 	for (int dnl=0; dnl<qdnd.count(); dnl++)
	 	{
			spr = new sprite();
	 		spr->node=qdnd.at(dnl);	
			spr->newSprite=false;
			spr->z = spr->node.toElement().attribute("z").toInt();
			spr->texture = spr->node.toElement().attribute("texture");
			QStringList qstrlO=spr->node.toElement().attribute("offset").split(",") ;		
			if(qstrlO.count()==2)
				{
					spr->offset_x=qstrlO[0].toInt();
					spr->offset_y=qstrlO[1].toInt();
				}
			QStringList qstrlR=spr->node.toElement().attribute("tex_rect").split(",") ;			
			if(qstrlR.count()==4)
			{
				spr->tex_x=qstrlR[0].toInt();
				spr->tex_y=qstrlR[1].toInt();
				spr->tex_w=qstrlR[2].toInt();
				spr->tex_h=qstrlR[3].toInt();
			}
			 sprites.append(spr);
	 	}
		

		anim * animated;                
		QDomNodeList qdndAnim= _node.toElement().elementsByTagName("anim");
		for (int dnl=0; dnl<qdndAnim.count(); dnl++)
		{
			animated = new anim();
			animated->newAnim=false;
                        QDomNode nodeT=qdndAnim.at(dnl);
                        animated->node=nodeT;
                        animated->z=nodeT.toElement().attribute("z").toInt();
                        animated->time=nodeT.toElement().attribute("time").toInt();
                        QStringList qstrlA=nodeT.toElement().attribute("offset").split(",") ;
			if(qstrlA.count()==2)
				{
					animated->offset_x=qstrlA[0].toInt();
					animated->offset_y=qstrlA[1].toInt();
				}
				frame * oneFrame;



                                QDomNodeList qdndFrameList= nodeT.childNodes ();
                                for(int chlds =0; chlds<qdndFrameList.count(); chlds++)
                                {
                                    QDomNode fnode = qdndFrameList.at(chlds);
                                    //просто фрейм
                                    if (fnode.toElement().tagName()=="frame")
                                    {
                                        oneFrame = new frame();
                                        oneFrame->newFrame=false;
                                        oneFrame->node=fnode;
                                        oneFrame->series=false;
                                        oneFrame->texture=fnode.toElement().attribute("texture");                                        
                                        QStringList qstrlFr=fnode.toElement().attribute("tex_rect").split(",") ;
                                        if(qstrlFr.count()==4)
                                                {
                                                        oneFrame->tex_x=qstrlFr[0].toInt();
                                                        oneFrame->tex_y=qstrlFr[1].toInt();
                                                        oneFrame->tex_w=qstrlFr[2].toInt();
                                                        oneFrame->tex_h=qstrlFr[3].toInt();
                                                }
                                        animated->frames.append(oneFrame);

                                    }
                                    //сеия фреймов
                                    else if (fnode.toElement().tagName()=="frames")
                                    {
                                        int size_w=0;
                                        int size_h=0;
                                        int count = fnode.toElement().attribute("count").toInt();
                                        if(count==0)continue;
                                        QString textureF = fnode.toElement().attribute("texture");
                                        if(!allTextures.contains(textureF))continue;
                                        QStringList sizes = fnode.toElement().attribute("size").split(",");
                                        if(sizes.count()==2)
                                        {
                                                size_w=sizes[0].toInt();
                                                size_h=sizes[1].toInt();
                                        }
                                        QRect rect(0,0,0,0);//= QPixmap(dir+"/"+allTextures.value(textureF)).rect();
                                        QStringList qstrlFr=fnode.toElement().attribute("tex_rect").split(",");
                                        if(qstrlFr.count()==4)
                                                {
                                                        rect.setLeft(qstrlFr[0].toInt());
                                                        rect.setTop(qstrlFr[1].toInt());
                                                        rect.setWidth(qstrlFr[2].toInt());
                                                        rect.setHeight(qstrlFr[3].toInt());
                                                }
                                        if((size_w==0)||(size_h==0))continue;
                                        int columnsF = rect.width()/size_w;
                                        int rowsF = rect.height()/size_h;

                                        int countOfFrames=0;
                                        for(int numRow=0; numRow<rowsF; numRow++)
                                              for(int numCol=0; numCol<columnsF; numCol++)
                                              {
                                                  if(countOfFrames==count)break;
                                                  countOfFrames++;
                                                  oneFrame = new frame();

                                                  if(countOfFrames==1)
                                                  oneFrame->node=fnode;//повторяется для первого(всех) в серии

                                                  oneFrame->newFrame=false;
                                                  oneFrame->series=true;
                                                  oneFrame->seriesNum=countOfFrames-1;                                                  
                                                  oneFrame->texture=textureF;
                                                  oneFrame->tex_x=rect.left()+ size_w*numCol;
                                                  oneFrame->tex_y=rect.top() + size_h*numRow;
                                                  oneFrame->tex_w=size_w;
                                                  oneFrame->tex_h=size_h;
                                                  animated->frames.append(oneFrame);
                                                }
                                    }

                                }
                        animList.append(animated);
		}
	fillTree();
}

QGraphicsPixmapItem * animatron::createGraphicsItem( QString _texture, int x, int y, int w, int h, QLabel * label )
{
    QGraphicsPixmapItem * gpi;
    QPixmap pixmap (dir+"/"+allTextures.value(_texture));
    pixmap=pixmap.copy(x,y,w,h);
    gpi= scene.addPixmap(pixmap);
    if(label!=NULL)
            {
                if ((w>62)||(h>62))
                {
                    if (w>h) pixmap=pixmap.scaledToWidth(63, Qt::SmoothTransformation);
                    else pixmap=pixmap.scaledToHeight(63, Qt::SmoothTransformation);
                }
               label->setPixmap(pixmap);
            }
    return gpi;
}

void animatron::setupUiImg()
{	
	ui.btn_r-> setIcon(QPixmap(":/ResEditor/Resources/animatron/arrow.png"));
	ui.btn_rt->setIcon(QPixmap(":/ResEditor/Resources/animatron/arrow-045.png"));
	ui.btn_t-> setIcon(QPixmap(":/ResEditor/Resources/animatron/arrow-090.png"));
	ui.btn_lt->setIcon(QPixmap(":/ResEditor/Resources/animatron/arrow-135.png"));
	ui.btn_l-> setIcon(QPixmap(":/ResEditor/Resources/animatron/arrow-180.png"));
	ui.btn_lb->setIcon(QPixmap(":/ResEditor/Resources/animatron/arrow-225.png"));
	ui.btn_b-> setIcon(QPixmap(":/ResEditor/Resources/animatron/arrow-270.png"));
	ui.btn_rb->setIcon(QPixmap(":/ResEditor/Resources/animatron/arrow-315.png"));
	ui.btn_center->setIcon(QPixmap(":/ResEditor/Resources/animatron/hand.png"));
	ui.btn_center->setEnabled(false);

	connect(ui.btn_lt, SIGNAL(clicked()), SLOT(moveLT()));
	connect(ui.btn_rt, SIGNAL(clicked()), SLOT(moveRT()));
	connect(ui.btn_lb, SIGNAL(clicked()), SLOT(moveLB()));
	connect(ui.btn_rb, SIGNAL(clicked()), SLOT(moveRB()));
	connect(ui.btn_l, SIGNAL(clicked()), SLOT(moveL()));
	connect(ui.btn_r, SIGNAL(clicked()), SLOT(moveR()));
	connect(ui.btn_b, SIGNAL(clicked()), SLOT(moveB()));
	connect(ui.btn_t, SIGNAL(clicked()), SLOT(moveT()));
}

void animatron::connectUi()
{
	connect(ui.le_name,SIGNAL(textChanged ( const QString &)), this, SLOT(slotObjNameChanged(const QString &)));
	connect(ui.spinBox_x, SIGNAL(valueChanged(int)), drawObject, SLOT(setOffset_x(int)));
	connect(ui.spinBox_y, SIGNAL(valueChanged(int)), drawObject, SLOT(setOffset_y(int)));
	connect(ui.spinBox_w, SIGNAL(valueChanged(int)), drawObject, SLOT(setW(int)));
	connect(ui.spinBox_h, SIGNAL(valueChanged(int)), drawObject, SLOT(setH(int)));
	connect(drawObject, SIGNAL(updatePos()), this, SLOT(updateScene()));
	connect(ui.treeWidget,SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), this, SLOT(changeActiveItem(QTreeWidgetItem*, QTreeWidgetItem*)));
}

void animatron::updateScene()
{
		scene.setSceneRect(-drawObject->offset_x-20, -drawObject->offset_y-20,
		drawObject->w+40,drawObject->h+40);
}

void animatron::changeActiveItem( QTreeWidgetItem* currentItem, QTreeWidgetItem*lastItem )
{
	TreeWidgetItemAnim* twia;
	if((currentItem->type()== TreeWidgetItemAnim::SPRITES)||(currentItem->type()== TreeWidgetItemAnim::ANIMATION_SET))
	{
		ui.scrollArea->takeWidget();
  		twia = (TreeWidgetItemAnim*)currentItem;
  		ui.scrollArea->setWidget(twia->widget);
	}

	switch(currentItem->type())
	{
        case TreeWidgetItemAnim::SPRITES		 : ui.dockWidget_widgets->setWindowTitle(tr("Спрайты объекта")); break;
		case TreeWidgetItemAnim::ONE_SPRITE	 : 
				ui.scrollArea->takeWidget();
				twia = (TreeWidgetItemAnim*)(currentItem->parent());
				ui.scrollArea->setWidget(twia->widget);
                                ui.dockWidget_widgets->setWindowTitle(tr("Спрайты объекта"));
			break;
		case TreeWidgetItemAnim::ANIMATIONS	 : 
			if(animList.count()>0)
			{
				ui.scrollArea->takeWidget();
				twia = (TreeWidgetItemAnim*)(currentItem->child(0));
				ui.scrollArea->setWidget(twia->widget);
                                ui.dockWidget_widgets->setWindowTitle(tr("Фреймы одной анимации"));
			}
			else
			{
				//QMessageBox::information(0,0,"ANIMATIONS childs count 0");
			}
			break;
        case TreeWidgetItemAnim::ANIMATION_SET : ui.dockWidget_widgets->setWindowTitle(tr("Фреймы одной анимации")); break;
		case TreeWidgetItemAnim::ANIMATION_FRAME:
				ui.scrollArea->takeWidget();
				twia = (TreeWidgetItemAnim*)(currentItem->parent());
				ui.scrollArea->setWidget(twia->widget);
                                ui.dockWidget_widgets->setWindowTitle(tr("Фреймы одной анимации"));
			break;

	}
	setEnabledAction(currentItem->type());
	changeActiveWidget( currentItem, lastItem );
}

void animatron::changeActiveWidget( QTreeWidgetItem* currentItem, QTreeWidgetItem*lastItem )
{
	if(lastItem!=NULL)
        {
            if (lastItem->type()==TreeWidgetItemAnim::ONE_SPRITE)
            {
                    if(((TreeWidgetItemAnim*)lastItem)->oneSprite!=NULL)
                    {((TreeWidgetItemAnim*)lastItem)->oneSprite->widget->ui.frame_panel->setFrameStyle(QFrame::Panel | QFrame::Raised);}
            }
            else if(lastItem->type()==TreeWidgetItemAnim::ANIMATION_FRAME)
            {
                    if(((TreeWidgetItemAnim*)lastItem)->oneFrame!=NULL)
                    ((TreeWidgetItemAnim*)lastItem)->oneFrame->widget->ui.frame_panel->setFrameStyle(QFrame::Panel | QFrame::Raised);
            }
        //else	{	}
        }

	if (currentItem->type()==TreeWidgetItemAnim::ONE_SPRITE)
	{
		WidgetControl * wc =((TreeWidgetItemAnim*)currentItem)->oneSprite->widget;
		wc->ui.frame_panel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
		ui.scrollArea->ensureWidgetVisible(wc); //ensureWidgetVisible
	}
	else if(currentItem->type()==TreeWidgetItemAnim::ANIMATION_FRAME)
	{
		WidgetControl * wc =((TreeWidgetItemAnim*)currentItem)->oneFrame->widget;
		wc->ui.frame_panel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
		ui.scrollArea->ensureWidgetVisible(wc); //ensureWidgetVisible
	}
}


void animatron::setEnabledAction( int _t)
{
	actAddSprite->setEnabled(false);
	actRemoveSprite->setEnabled(false);
	actEditSprite->setEnabled(false);
	actAddAnimation->setEnabled(false);
	actRemoveAnimation->setEnabled(false);
	actAddFrame->setEnabled(false);
	actRemoveFrame->setEnabled(false);
	actEditFrame->setEnabled(false);
	actMoveFrameUp->setEnabled(false);
	actMoveFrameDown->setEnabled(false);

	switch(_t)
	{
	case TreeWidgetItemAnim::SPRITES		 : 	
		actAddSprite->setEnabled(true);
		break;
	case TreeWidgetItemAnim::ONE_SPRITE	 : 
		actAddSprite->setEnabled(true);
		actRemoveSprite->setEnabled(true);
		actEditSprite->setEnabled(true);
		break;
	case TreeWidgetItemAnim::ANIMATIONS	 : 
		actAddAnimation->setEnabled(true);
		break;
	case TreeWidgetItemAnim::ANIMATION_SET :
		actAddAnimation->setEnabled(true);
		actRemoveAnimation->setEnabled(true);
		actAddFrame->setEnabled(true);
		break;
	case TreeWidgetItemAnim::ANIMATION_FRAME:	
		actRemoveAnimation->setEnabled(true);
		actAddFrame->setEnabled(true);
		actRemoveFrame->setEnabled(true);
		actEditFrame->setEnabled(true);
		actMoveFrameUp->setEnabled(true);
		actMoveFrameDown->setEnabled(true);
		break;
	}
}


void animatron::deleteAllObjects()
{
	delete drawObject;
	QListIterator<sprite*> sit(sprites);
	while(sit.hasNext())
	{
		sprite* sprt=sit.next();
		delete sprt->widget;
		delete sprt->graphicsItem; //удаление графического объекта
                delete sprt->grRectItem;//удаление рамки
		delete sprt;
	}
	QListIterator<anim*> ait(animList);
	while (ait.hasNext())
	{
		anim * anm = ait.next();	
		QListIterator<frame*> fit(anm->frames);
		while(fit.hasNext())
		{
			frame * frm= fit.next();
			delete frm->graphicsItem;
                        delete frm->grRectItem;
		}
		delete anm->widget;
		delete anm;
	}
}

void animatron::createActions()
{
    actStartAnimation = ui.mainToolBar->addAction(QPixmap(":/ResEditor/Resources/animatron/control.png"),tr("Старт"));
	actStartAnimation->setCheckable(true);
		connect(actStartAnimation,SIGNAL(toggled ( bool)),SLOT(startAnimation(bool)));
                actStopAnimation = ui.mainToolBar->addAction(QPixmap(":/ResEditor/Resources/animatron/control-stop-square.png"),tr("Стоп"));
		connect(actStopAnimation,SIGNAL(triggered ()),SLOT(stopAnimation()));

	ui.mainToolBar->addSeparator();
        actSaveDraw=ui.mainToolBar->addAction(QPixmap(":/ResEditor/Resources/animatron/disk.png"),tr("Сохранить"));
		connect(actSaveDraw, SIGNAL(triggered()), this,SLOT(slotSaveDraw()));

	ui.mainToolBar->addSeparator();
        actZoomIn= ui.mainToolBar->addAction(QPixmap(":/ResEditor/Resources/animatron/magnifier-zoom-in.png"),tr("Приблизить"));
		connect(actZoomIn,SIGNAL(triggered()),SLOT(slotZoomIn()));
                actZoomONE= ui.mainToolBar->addAction(QPixmap(":/ResEditor/Resources/animatron/magnifier-zoom-actual.png"),tr("Масштаб 1:1"));
		connect(actZoomONE,SIGNAL(triggered()),SLOT(slotZoomONE()));
                actZoomOut= ui.mainToolBar->addAction(QPixmap(":/ResEditor/Resources/animatron/magnifier-zoom-out.png"),tr("Отдалить"));
		connect(actZoomOut,SIGNAL(triggered()),SLOT(slotZoomOut()));
                actGround= ui.mainToolBar->addAction(QPixmap(":/ResEditor/Resources/animatron/ground.png"),tr("Текстура земли"));
	actGround->setCheckable(true);
		connect(actGround,SIGNAL(toggled(bool)),SLOT(slotGround(bool)));
                actGreed= ui.mainToolBar->addAction(QPixmap(":/ResEditor/Resources/animatron/grid.png"),tr("Сетка"));
	actGreed->setCheckable(true);
		connect(actGreed,SIGNAL(toggled(bool)),SLOT(slotGreed(bool)));
                actTransparent= ui.mainToolBar->addAction(QPixmap(":/ResEditor/Resources/animatron/layer-transparent.png"),tr("Прозрачность"));
	actTransparent->setCheckable(true);
		connect(actTransparent,SIGNAL(toggled(bool)),SLOT(slotTransparent(bool)));

		actGround->setChecked(true);
		actGreed->setChecked(true);
		actTransparent->setChecked(false);
		transp=false;

	ui.mainToolBar->addSeparator();
        actAddSprite = ui.mainToolBar->addAction(QPixmap(":/ResEditor/Resources/animatron/image--plus.png"),tr("Добавить спрайт"));
		connect(actAddSprite,SIGNAL(triggered()),SLOT(addSprite()));
                actRemoveSprite = ui.mainToolBar->addAction(QPixmap(":/ResEditor/Resources/animatron/image--minus.png"),tr("Удалить спрайт"));
		connect(actRemoveSprite,SIGNAL(triggered()),SLOT(removeSprite()));
                actEditSprite = ui.mainToolBar->addAction(QPixmap(":/ResEditor/Resources/animatron/image--pencil.png"),tr("Изменить спрайт"));
		connect(actEditSprite,SIGNAL(triggered()),SLOT(editSprite()));
                actAddAnimation = ui.mainToolBar->addAction(QPixmap(":/ResEditor/Resources/animatron/films--plus.png"),tr("Добавить анимацию"));
		connect(actAddAnimation,SIGNAL(triggered()),SLOT(addAnimation()));
                actRemoveAnimation = ui.mainToolBar->addAction(QPixmap(":/ResEditor/Resources/animatron/films--minus.png"),tr("Удалить анимацию"));
		connect(actRemoveAnimation,SIGNAL(triggered()),SLOT(removeAnimation()));
                actAddFrame = ui.mainToolBar->addAction(QPixmap(":/ResEditor/Resources/animatron/film--plus.png"),tr("Добавить фрейм"));
		connect(actAddFrame,SIGNAL(triggered()),SLOT(addFrame()));
                actRemoveFrame = ui.mainToolBar->addAction(QPixmap(":/ResEditor/Resources/animatron/film--minus.png"),tr("Удалить фрейм"));
		connect(actRemoveFrame,SIGNAL(triggered()),SLOT(removeFrame()));
                actEditFrame = ui.mainToolBar->addAction(QPixmap(":/ResEditor/Resources/animatron/film--pencil.png"),tr("Изменить фрейм"));
		connect(actEditFrame,SIGNAL(triggered()),SLOT(editFrame()));

                actMoveFrameUp = ui.mainToolBar->addAction(QPixmap(":/ResEditor/Resources/animatron/film--up.png"),tr("Передвинуть фрейм выше"));
		connect(actMoveFrameUp,SIGNAL(triggered()),SLOT(moveFrameUp()));
                actMoveFrameDown = ui.mainToolBar->addAction(QPixmap(":/ResEditor/Resources/animatron/film--down.png"),tr("Передвинуть фрейм ниже"));
		connect(actMoveFrameDown,SIGNAL(triggered()),SLOT(moveFrameDown()));
                actMoveFrameUp->setVisible(false);
                actMoveFrameDown->setVisible(false);
                //TODO: actMoveFrameUp
                //TODO: actMoveFrameDown

		actAddSprite->setEnabled(false);
		actRemoveSprite->setEnabled(false);
		actEditSprite->setEnabled(false);
		actAddAnimation->setEnabled(false);
		actRemoveAnimation->setEnabled(false);
		actAddFrame->setEnabled(false);
		actRemoveFrame->setEnabled(false);
		actEditFrame->setEnabled(false);
		actMoveFrameUp->setEnabled(false);
		actMoveFrameDown->setEnabled(false);
}

void animatron::slotZoomIn()
{
	if (zoomState<5)
	{	zoomState++;
		ui.graphicsView->scale(2,2);
	}
        setWindowTitle(QString("scene rect %1 %2").arg(scene.sceneRect().x()).arg(scene.sceneRect().y()));
}

void animatron::slotZoomONE()
{
	if (zoomState<0)	{while(zoomState!=0){slotZoomIn();}}
	else if(zoomState>0){while(zoomState!=0){slotZoomOut();}}
}

void animatron::slotZoomOut()
{
	if (zoomState>-5)
	{	zoomState--;
		ui.graphicsView->scale(0.5,0.5);
	}
}

void animatron::slotGround( bool _b)
{	ground = _b;
	setBackground();
}

void animatron::slotGreed( bool _b)
{	greed=_b;
	setBackground();
}

void animatron::setBackground()
{
	if(!transp)
        {
            if(ground)
            {	if(greed){ui.graphicsView->setBackgroundBrush(QPixmap(":/ResEditor/Resources/animatron/ground_greed.png"));}
                    else	{ui.graphicsView->setBackgroundBrush(QPixmap(":/ResEditor/Resources/animatron/ground_nogreed.png"));}
            }
            else
            {	if(greed){ui.graphicsView->setBackgroundBrush(QPixmap(":/ResEditor/Resources/animatron/noground_greed.png"));}
                    else	{ui.graphicsView->setBackgroundBrush(QPixmap(""));}
            }
        }
}

void animatron::slotTransparent( bool _b)
{	transp=_b;
	if(transp) {ui.graphicsView->setBackgroundBrush(QPixmap(":/ResEditor/Resources/animatron/bg.png"));}
	else {setBackground();}
}

void animatron::addSprite()
{
        addNewSprite(10,0,0,"",0,0,1,1);
}


void animatron::addNewSprite( int _z, int _dx, int _dy, QString _texture, int _x, int _y, int _w, int _h )
{
	sprite * ps;
	ps= new sprite(_z,_dx,_dy,_texture,_x,_y,_w,_h);		
	sprites.append(ps);

	//создние своего итема и передача указателя на спрайт
 	TreeWidgetItemAnim * qtwis = new TreeWidgetItemAnim((QTreeWidgetItem *)spriteTreeItem, TreeWidgetItemAnim::ONE_SPRITE);
        qtwis->setText(0,tr("спрайт"));
 	qtwis->setIcon(0,QPixmap(":/ResEditor/Resources/animatron/image.png"));
	qtwis->oneSprite=ps;
 	ps->widget = new WidgetControl();//new QWidget();
	ps->widget->treeWidgetItem=qtwis;
	ps->widget->labelNew=new QLabel(ps->widget);
	ps->widget->labelNew->setPixmap(QPixmap(":/ResEditor/Resources/animatron/new-text.png"));
 	ps->widget->setContentsMargins(0,0,0,0);
 	ps->widget->ui.btn_up->setEnabled(false);
 	ps->widget->ui.btn_down->setEnabled(false);
 	ps->widget->ui.btn_center->setEnabled(false);
 	ps->widget->ui.spinBox_x->setValue(ps->offset_x);
 	ps->widget->ui.spinBox_y->setValue(ps->offset_y);
 	ps->widget->ui.spinBox_z->setValue(ps->z);
 	ps->drawItem= drawObject;
        ps->graphicsItem=createGraphicsItem(ps->texture, ps->tex_x, ps->tex_y, ps->tex_w, ps->tex_h, ps->widget->ui.label);
 	ps->graphicsItem->setZValue(ps->z);
 	ps->graphicsItem->setPos(drawObject->drawRect->rect().x()+ps->offset_x, drawObject->drawRect->rect().y()+ps->offset_y);
        ps->grRectItem=scene.addRect(drawObject->drawRect->rect().x()+ps->offset_x, drawObject->drawRect->rect().y()+ps->offset_y, ps->tex_w, ps->tex_h);
        ps->grRectItem->setZValue(ps->z);
 
 	ps->widget->ui.checkBox_use->setChecked(true); ps->used=true;
 	ps->widget->ui.checkBox_visible->setChecked(true); ps->visible=true;
 	connect(ps->widget->ui.checkBox_use, SIGNAL( toggled ( bool)), ps, SLOT(setUsed(bool)));
 	connect(ps->widget->ui.checkBox_visible, SIGNAL( toggled ( bool)), ps, SLOT(setVisible(bool)));
 	connect(ps->widget->ui.spinBox_x, SIGNAL(valueChanged ( int)), ps, SLOT(setOffset_x(int )));
 	connect(ps->widget->ui.spinBox_y, SIGNAL(valueChanged ( int)), ps, SLOT(setOffset_y(int )));
 	connect(ps->widget->ui.spinBox_z, SIGNAL(valueChanged ( int)), ps, SLOT(set_z(int )));
	connect(drawObject, SIGNAL(updatePos()), ps, SLOT(updatePos()));

        connect(ps->widget, SIGNAL(clicked(QTreeWidgetItem *)), this, SLOT(clickedOnWidget(QTreeWidgetItem *)));
        connect(ps->widget, SIGNAL(doubleClicked(QTreeWidgetItem *)), this, SLOT(dbkClickedOnWidget(QTreeWidgetItem*)));

	spriteTreeItem->gridLayout->removeItem(spriteTreeItem->gridLayout->itemAtPosition(spriteTreeItem->gridLayout->rowCount()-1, 0));
	spriteTreeItem->gridLayout->addWidget(ps->widget, spriteTreeItem->gridLayout->rowCount()-1, 0, 1, 1);
  	QSpacerItem * verticalSpacerS = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
  	spriteTreeItem->gridLayout->addItem(verticalSpacerS,spriteTreeItem->gridLayout->rowCount() , 0, 1, 1);

	ui.treeWidget->setCurrentItem(qtwis);
	editSprite();
}


void animatron::removeSprite()
{
	TreeWidgetItemAnim* stwi=(TreeWidgetItemAnim*)ui.treeWidget->currentItem();
	if(stwi->type()==TreeWidgetItemAnim::ONE_SPRITE)
	{
		uslessNodes.append(stwi->oneSprite->node);
		delete stwi->oneSprite->widget;//удалить из грида виджет
		sprites.removeOne(stwi->oneSprite);//удалить из листа
		delete stwi->oneSprite->graphicsItem; //удаление графического объекта
                delete stwi->oneSprite->grRectItem;//удаление рамки
		delete stwi->oneSprite;//удалить сам объект
		stwi->oneSprite=NULL;
		stwi->parent()->removeChild(stwi);//удалить из дерева
	}
}

void animatron::editSprite()
{
	TreeWidgetItemAnim* stwi=(TreeWidgetItemAnim*)ui.treeWidget->currentItem();
	if(stwi->type()==TreeWidgetItemAnim::ONE_SPRITE)
	{
		imgCtr->setVisible(true);
		imgCtr->setSprite(
			stwi->oneSprite->texture,
			stwi->oneSprite->tex_x,
			stwi->oneSprite->tex_y,
			stwi->oneSprite->tex_w,
			stwi->oneSprite->tex_h
			);	
	}
}

void animatron::addAnimation()
{
	addNewAnim(11, 0,0, 2000);
}


void animatron::addNewAnim(int _z, int _offset_x, int _offset_y, int _time)
{	
	anim * pa = new anim(_z, _offset_x, _offset_y, _time);
	animList.append(pa);
 	//создние своего итема и передача указателя на спрайт
 	TreeWidgetItemAnim * qtwia = new TreeWidgetItemAnim(animationsTreeItem, TreeWidgetItemAnim::ANIMATION_SET);
        qtwia->setText(0, tr("анимация"));
 	qtwia->setIcon(0,QPixmap(":/ResEditor/Resources/animatron/films.png"));
 	qtwia->oneAnim=pa;
 	qtwia->widget= new QWidget();
 	QGridLayout * gridLayoutF = new QGridLayout(qtwia->widget);
	qtwia->gridLayout=gridLayoutF;
 	gridLayoutF->setContentsMargins(0,0,0,0);
 	gridLayoutF->setSpacing(0);
 
 	//установка настроек анимации
 	pa->widget = new WidgetControl();
        pa->widget->ui.label->setText(tr("Время"));
 	pa->spinBox_time = new QSpinBox(pa->widget->ui.label);
 	pa->spinBox_time->setGeometry(pa->widget->ui.label->x(),pa->widget->ui.label->y(),60, 20 );
 	pa->spinBox_time->setMaximum(20000);
 	pa->spinBox_time->setValue(pa->time);
 	pa->widget->setContentsMargins(0,0,0,0);
 	pa->widget->ui.btn_up->setEnabled(false);
 	pa->widget->ui.btn_down->setEnabled(false);
 	pa->widget->ui.btn_center->setEnabled(false);
 	pa->widget->ui.spinBox_x->setValue(pa->offset_x);
 	pa->widget->ui.spinBox_y->setValue(pa->offset_y);
 	pa->widget->ui.spinBox_z->setValue(pa->z);
 	pa->widget->ui.checkBox_use->setChecked(true); pa->border=true;
 	pa->widget->ui.checkBox_visible->setChecked(true); pa->visible=true;
 	pa->drawItem=drawObject;
 	connect(pa->spinBox_time, SIGNAL(valueChanged(int)), this, SLOT(setTime(int)));
 	connect(pa->widget->ui.checkBox_use, SIGNAL( toggled ( bool)), pa, SLOT(setUsed(bool)));
 	connect(pa->widget->ui.checkBox_visible, SIGNAL( toggled ( bool)), pa, SLOT(setVisible(bool)));
 	connect(pa->widget->ui.spinBox_x, SIGNAL(valueChanged ( int)), pa, SLOT(setOffset_x(int )));
 	connect(pa->widget->ui.spinBox_y, SIGNAL(valueChanged ( int)), pa, SLOT(setOffset_y(int )));
 	connect(pa->widget->ui.spinBox_z, SIGNAL(valueChanged ( int)), pa, SLOT(set_z(int )));
 	connect(drawObject, SIGNAL(updatePos()), pa, SLOT(updateAllPos()));
	connect(this, SIGNAL(animationStart()), pa, SLOT(slotStartAnimation()));
	connect(this, SIGNAL(animationStop()), pa, SLOT(slotStopAnimation()));
 	gridLayoutF->addWidget(pa->widget, 0, 0, 1, 1);
	QSpacerItem * verticalSpacerF = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	gridLayoutF->addItem(verticalSpacerF, 1, 0, 1, 1);
}

void animatron::removeAnimation()
{
	//обойти и удалить сначала все фреймы
	QTreeWidgetItem * treeWidgetItem = ui.treeWidget->currentItem();
	if (treeWidgetItem->type()==TreeWidgetItemAnim::ANIMATION_FRAME)
	{
		treeWidgetItem= treeWidgetItem->parent();
	}
	else if(treeWidgetItem->type()==TreeWidgetItemAnim::ANIMATION_SET){}
	else
	{
		return;
	}
	while(treeWidgetItem->childCount()>0)
	{
		removeFrame(treeWidgetItem->child(0));
	}
	TreeWidgetItemAnim * treeWItem = (TreeWidgetItemAnim*)treeWidgetItem;
 	uslessNodes.append(treeWItem->oneAnim->node);
	animList.removeOne(treeWItem->oneAnim);
 	delete treeWItem->oneAnim->widget;
 	delete treeWItem->widget;
 	delete treeWItem->oneAnim;	
 	treeWItem->parent()->removeChild(treeWidgetItem);
 	delete treeWItem;
}

void animatron::addFrame()
{
    QTreeWidgetItem * twi = NULL;
    if(ui.treeWidget->currentItem()->type()== TreeWidgetItemAnim::ANIMATION_SET)
    {
        if(ui.treeWidget->currentItem()->childCount()>0)
        {
            twi = ui.treeWidget->currentItem()->child(ui.treeWidget->currentItem()->childCount()-1);
        }
    }
    else if (ui.treeWidget->currentItem()->type()== TreeWidgetItemAnim::ANIMATION_FRAME)
    {
        twi = ui.treeWidget->currentItem()->parent()->child(ui.treeWidget->currentItem()->parent()->childCount()-1);
    }

    if (twi!=NULL)
    {
        TreeWidgetItemAnim * twa = static_cast<TreeWidgetItemAnim *>(twi);
        addNewFrame(twa->oneFrame->texture,
                    twa->oneFrame->tex_x + twa->oneFrame->tex_w,
                    twa->oneFrame->tex_y,
                    twa->oneFrame->tex_w,
                    twa->oneFrame->tex_h);
    }
    else
        addNewFrame("", 0,0,1,1);
}

void animatron::addNewFrame( QString _texture, int _x, int _y, int _w, int _h )
{
	QTreeWidgetItem * treeWidgetItem = ui.treeWidget->currentItem();
	if (treeWidgetItem->type()==TreeWidgetItemAnim::ANIMATION_FRAME)
	{
		treeWidgetItem= treeWidgetItem->parent();
	}
	else if(treeWidgetItem->type()==TreeWidgetItemAnim::ANIMATION_SET){}
	else
	{
		return;
	}

	TreeWidgetItemAnim * ftwi = new TreeWidgetItemAnim(treeWidgetItem, TreeWidgetItemAnim::ANIMATION_FRAME);
	frame * frm = new frame(_texture, _x,_y,_w,_h);
	((TreeWidgetItemAnim*)treeWidgetItem)->oneAnim->frames.append(frm);
        ftwi->setText(0, tr("Фрейм"));
	ftwi->setIcon(0,QPixmap(":/ResEditor/Resources/animatron/film.png"));
	ftwi->oneFrame=frm;
	frm->widget = new WidgetControl();//new QWidget();
	frm->widget->treeWidgetItem=ftwi;
	frm->widget->labelNew=new QLabel(frm->widget);
	frm->widget->labelNew->setPixmap(QPixmap(":/ResEditor/Resources/animatron/new-text.png"));
	frm->widget->setContentsMargins(0,0,0,0);
	frm->widget->ui.btn_up->setEnabled(false);
	frm->widget->ui.btn_down->setEnabled(false);
	frm->widget->ui.btn_center->setEnabled(false);
	frm->widget->ui.btn_b->setEnabled(false);
	frm->widget->ui.btn_t->setEnabled(false);
	frm->widget->ui.btn_l->setEnabled(false);
	frm->widget->ui.btn_r->setEnabled(false);
	frm->widget->ui.btn_lt->setEnabled(false);
	frm->widget->ui.btn_rt->setEnabled(false);
	frm->widget->ui.btn_lb->setEnabled(false);
	frm->widget->ui.btn_rb->setEnabled(false);
	frm->widget->ui.spinBox_x->setEnabled(false);
	frm->widget->ui.spinBox_y->setEnabled(false);
	frm->widget->ui.spinBox_z->setEnabled(false);
	frm->widget->ui.checkBox_use->setChecked(true); frm->used=true;
	frm->widget->ui.checkBox_visible->setChecked(true); frm->visible=true;
	frm->drawItem= drawObject;
        frm->graphicsItem=createGraphicsItem(frm->texture, frm->tex_x, frm->tex_y, frm->tex_w, frm->tex_h, frm->widget->ui.label);
	TreeWidgetItemAnim * twianim = (TreeWidgetItemAnim *)(treeWidgetItem);
 	anim * pa = twianim->oneAnim;
 	frm->graphicsItem->setPos(drawObject->drawRect->rect().x()+ pa->offset_x, drawObject->drawRect->rect().y()+pa->offset_y);
        frm->grRectItem=scene.addRect(drawObject->drawRect->rect().x()+pa->offset_x, drawObject->drawRect->rect().y()+pa->offset_y, frm->tex_w, frm->tex_h);
  	frm->graphicsItem->setZValue(pa->z);
        frm->grRectItem->setZValue(pa->z);
  	connect(frm->widget->ui.checkBox_visible, SIGNAL(toggled ( bool)), frm, SLOT(setVisiblity(bool)));
  	connect(frm->widget->ui.checkBox_use, SIGNAL(toggled ( bool)), frm, SLOT(setUsibility(bool)));

        connect(frm->widget, SIGNAL(clicked(QTreeWidgetItem *)), this, SLOT(clickedOnWidget(QTreeWidgetItem *)));
        connect(frm->widget, SIGNAL(doubleClicked(QTreeWidgetItem *)), this, SLOT(dbkClickedOnWidget(QTreeWidgetItem*)));

        QGridLayout * qgrid = twianim->gridLayout;
 	qgrid->removeItem(qgrid->itemAtPosition(qgrid->rowCount()-1, 0));
 	qgrid->addWidget(frm->widget, qgrid->rowCount()-1, 0, 1, 1);
 	QSpacerItem * verticalSpacerS = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
 	qgrid->addItem(verticalSpacerS,qgrid->rowCount() , 0, 1, 1);
	ui.treeWidget->setCurrentItem(ftwi);
	editFrame();
}

void animatron::removeFrame()
{
	removeFrame(ui.treeWidget->currentItem());
}

void animatron::removeFrame( QTreeWidgetItem * _twi)
{
	TreeWidgetItemAnim* stwi=(TreeWidgetItemAnim*)_twi;
	if(stwi->type()==TreeWidgetItemAnim::ANIMATION_FRAME)
	{
		uslessNodes.append(stwi->oneFrame->node);
		delete stwi->oneFrame->widget;//удалить из грида виджет
		((TreeWidgetItemAnim*)stwi->parent())->oneAnim->frames.removeOne(stwi->oneFrame);//sprites.removeOne(stwi->oneSprite);//удалить из листа
 		delete stwi->oneFrame->graphicsItem; //удаление графического объекта
                delete stwi->oneFrame->grRectItem;//удаление рамки
 		delete stwi->oneFrame;//удалить сам объект
		stwi->oneFrame=NULL;
 		delete  stwi;//удалить из дерева
	}
}



void animatron::editFrame()
{
	TreeWidgetItemAnim* stwi=(TreeWidgetItemAnim*)ui.treeWidget->currentItem();
	if(stwi->type()==TreeWidgetItemAnim::ANIMATION_FRAME)
	{
			imgCtr->setVisible(true);
			imgCtr->setSprite(
				stwi->oneFrame->texture,
				stwi->oneFrame->tex_x,
				stwi->oneFrame->tex_y,
				stwi->oneFrame->tex_w,
				stwi->oneFrame->tex_h
				);
	}
}

void animatron::moveFrameUp()
{
    //TODO: animatron::moveFrameUp()
}

void animatron::moveFrameDown()
{
    //TODO: animatron::moveFrameDown()
}

void animatron::saveEditedImg( int _x,int _y,int _w, int _h, QString _texture)
{
	TreeWidgetItemAnim * treeItem = (TreeWidgetItemAnim *)ui.treeWidget->currentItem();
	if(treeItem==NULL) return;
		if (treeItem->type()==TreeWidgetItemAnim::ONE_SPRITE)
		{
			sprite * oneSprite = ((TreeWidgetItemAnim*)treeItem)->oneSprite;//->widget;//->ui.frame_panel->setFrameStyle(QFrame::Panel | QFrame::Raised);
			oneSprite->tex_x=_x;
			oneSprite->tex_y=_y;
			oneSprite->tex_w=_w;
			oneSprite->tex_h=_h;
			oneSprite->texture=_texture;
                        updateGrUnit(oneSprite);
		}
		else if(treeItem->type()==TreeWidgetItemAnim::ANIMATION_FRAME)
		{
			frame * oneFrame = ((TreeWidgetItemAnim*)treeItem)->oneFrame;//->widget;//->ui.frame_panel->setFrameStyle(QFrame::Panel | QFrame::Raised);
			oneFrame->tex_x=_x;
			oneFrame->tex_y=_y;
			oneFrame->tex_w=_w;
			oneFrame->tex_h=_h;
			oneFrame->texture=_texture;
                        updateGrUnit(oneFrame);
		}
}

void animatron:: updateGrUnit(GraphicsUnit* gu)
{
    gu->graphicsItem->setPixmap(QPixmap(dir+"/"+allTextures.value(gu->texture))
                                                                            .copy(gu->tex_x,gu->tex_y, gu->tex_w, gu->tex_h ));
    QPixmap pxm =gu->graphicsItem->pixmap();
    if((gu->widget->ui.label->width()<pxm.width())||(gu->widget->ui.label->height()<pxm.height()))
    {
        if(pxm.width()>pxm.height())
        {pxm=pxm.scaledToWidth(gu->widget->ui.label->width());}
        else
        {pxm=pxm.scaledToHeight(gu->widget->ui.label->height());}
    }
    gu->widget->ui.label->setPixmap(pxm);
    gu->grRectItem->setRect(gu->grRectItem->rect().x(), gu->grRectItem->rect().y(), gu->tex_w, gu->tex_h);
}

void animatron::slotSaveDraw()
{
	deleteUslessNodes();
		node.toElement().setAttribute("name", drawObject->name);
		node.toElement().setAttribute("offset", QString("%1, %2").arg(drawObject->offset_x).arg(drawObject->offset_y));
		node.toElement().setAttribute("size", QString("%1, %2").arg(drawObject->w).arg(drawObject->h));	
	QDomDocument document0 = QDomDocument();
	QDomNode lastNode;
	bool hasLast=false;
 	QListIterator<sprite *> spriteIterator(sprites);
 	while(spriteIterator.hasNext())
 	{
		sprite * _sprite = spriteIterator.next();

		if(_sprite->newSprite)
		{
			_sprite->node=document0.createElement("sprite");
			if(hasLast)
			{				
				node.insertAfter(_sprite->node, lastNode);
			}
			else
			{
				node.insertBefore(_sprite->node, QDomNode());
			}
		}
		_sprite->node.toElement().setAttribute("z", _sprite->z);
		_sprite->node.toElement().setAttribute("offset", QString("%1, %2").arg(_sprite->offset_x).arg(_sprite->offset_y));
		_sprite->node.toElement().setAttribute("texture", _sprite->texture);
		_sprite->node.toElement().setAttribute("tex_rect", QString("%1, %2, %3, %4").arg(_sprite->tex_x).arg(_sprite->tex_y).arg(_sprite->tex_w).arg(_sprite->tex_h));	

		_sprite->newSprite=false;
		hasLast=true;
		lastNode = _sprite->node;
	}

	QDomNode lastNodeA;
	bool hasLastA=false;
	QListIterator<anim *> animIterator(animList);
	while(animIterator.hasNext())
	{
		anim * _anim = animIterator.next();
			
		if(_anim->newAnim)
		{
			_anim->node=document0.createElement("anim");
			if(hasLastA)
			{				
				node.insertAfter(_anim->node, lastNode);
			}
			else
			{
				node.appendChild(_anim->node);
			}
		}
	_anim->node.toElement().setAttribute("z", _anim->z);
	_anim->node.toElement().setAttribute("time", _anim->time);
	_anim->node.toElement().setAttribute("offset", QString("%1, %2").arg(_anim->offset_x).arg(_anim->offset_y));
	_anim->newAnim=false;
	hasLastA=true;
	lastNodeA = _anim->node;
	
		QDomNode lastAddedNode = QDomNode();
		QDomNode lastNodeF;
		bool seriesBegin=false;
		QDomNode seriesNode;
                int lastSF=0;
                frame * _frame= NULL;
                frame * _LASTframe = NULL;
                frame * _seriesFrame = NULL;

                QListIterator<frame*> frameIterator(_anim->frames);
                while(frameIterator.hasNext())
                {
                    //QMessageBox::information(0,0,"фрейм");
                    _frame = frameIterator.next();
                    if(_frame->series)//если серия
                    {
                         //QMessageBox::information(0,0,"он в серии");
                        if(_frame->seriesNum==0)//если номер в серии 0 - тоесть это первый / серия началась
                        {
                            //ПРОВЕРИТЬ НА ОКОНЧЕННОСТЬ ПРОШЛОЙ СЕРИИ
                           //QMessageBox::information(0,0,"причём ПЕРВЫЙ");
                            seriesBegin=true;//серия началась
                            lastSF=0;//последним фреймом был первый (этот)
                            _LASTframe=_frame;//последний фрейм - это текущий
                            _seriesFrame=_frame;//последний фрейм серии - это текущий


                            if(_LASTframe->newFrame)//если фрейм новый то дабавлена новая серия / нужно создать узел
                               {
                                    //QMessageBox::information(0,0,"и узла пока нету");
                                    seriesNode = document0.createElement("frames");
                                    if(lastAddedNode.isNull())//небыло элементов.. добавляем в начало
                                    {_anim->node.insertBefore(seriesNode, QDomNode());
                                        //QMessageBox::information(0,0,"начало серии добавлен в начало");
                                    }
                                    else //элементы были юю добвляем после последнего
                                    {_anim->node.insertAfter(seriesNode, lastAddedNode);
                                        //QMessageBox::information(0,0,"начало серии добавлен в конец");
                                    }
                                    lastAddedNode = _frame->node;
                                    //атрибут текстуры
                                    seriesNode.toElement().setAttribute("texture", _LASTframe->texture);
                                    //атрибут количества повторяющихся фреймов
                                    seriesNode.toElement().setAttribute("count", (_LASTframe->seriesNum+1)); //_LASTframe->seriesNum;//устанавливается выражение оставлно для логики 1
                                    //размер изображения фрейма
                                    seriesNode.toElement().setAttribute("size", QString("%1, %2").arg(_LASTframe->tex_w).arg(_LASTframe->tex_h));

                                    _seriesFrame->node=seriesNode;
                                }
                            else    //узел уже существовал
                                {
                                    //QMessageBox::information(0,0,"а вот узел уже есть");
                                    //отмечаем(запоминаем) существовавший узел послидним узлом
                                    seriesNode = _frame->node;

                                    if(lastAddedNode.isNull())//небыло элементов.. добавляем в начало
                                    {_anim->node.insertBefore(seriesNode, QDomNode());
                                        //QMessageBox::information(0,0,"начало серии был добавлен в начало");
                                    }
                                    else //элементы были юю добвляем после последнего
                                    {_anim->node.insertAfter(seriesNode, lastAddedNode);
                                        //QMessageBox::information(0,0,"начало серии был добавлен в конец");
                                    }
                                    lastAddedNode = _frame->node;
                                 }
                            //запоминаем какой узел был последним
                            lastNodeF=seriesNode;
                        }
                        else if(_frame->seriesNum==(lastSF+1))//это следующий по очереди в последовательности(не выбивается)
                        {
                            //QMessageBox::information(0,0, QString("он в серии и по номеру %1").arg(_frame->seriesNum));
                            //нужно проверить фрейм на вхождение в серию по вхождению в область
                            QRect rect(0,0,0,0);//= QPixmap(dir+"/"+allTextures.value(textureF)).rect();
                            QStringList qstrlFr=_seriesFrame->node.toElement().attribute("tex_rect").split(",");
                            if(qstrlFr.count()==4)
                                    {
                                            rect.setLeft(qstrlFr[0].toInt());
                                            rect.setTop(qstrlFr[1].toInt());
                                            rect.setWidth(qstrlFr[2].toInt());
                                            rect.setHeight(qstrlFr[3].toInt());
                                    }
                            int columnsF = rect.width()/_frame->tex_w;// size_w;                            
                            int numCol=_frame->seriesNum%columnsF;
                            int numRow=_frame->seriesNum/columnsF;

                            //если совпадают размеры фрейма
                            bool frmInS= false;
                            //QMessageBox::information(0,0,QString("%1 + %2 ==  %3   %4 + %5 == %6   %7 %8")
                            //                        .arg(rect.left()).arg(_frame->tex_w*numCol).arg(_frame->tex_x)
                            //                         .arg(rect.top() ).arg(_frame->tex_h*numRow).arg(_frame->tex_y)
                            //                        );
                            if ((_LASTframe->tex_w == _seriesFrame->tex_w)&&(_LASTframe->tex_h == _seriesFrame->tex_h))
                                //и совпадают координаты (координаты такие какие должны быть)
                                if(((rect.left()+ _frame->tex_w*numCol) ==_frame->tex_x) &&
                                        ((rect.top() + _frame->tex_h*numRow) == _frame->tex_y))
                                    {   //
                                        //QMessageBox::information(0,0," проверено он реально внутри серии");
                                        _seriesFrame->node.toElement().setAttribute("count", _frame->seriesNum+1);
                                        frmInS=true;

                                        lastSF=_frame->seriesNum; //последний фрейм - это текущий
                                    }
                            //фрейм выбивается из серии это отдельный фрейм
                            if (!frmInS)
                            {
                                //QMessageBox::information(0,0," проверено он реально НЕ внутри серии");
                                //помечаем что фрейм больше не в серии
                                _frame->series=false;                                
                                _frame->widget->lbl->setText("");
                               // _frame->widget->ui.
                                //если  там указан нод серии - то создаём новый нод простого фрейма
                               // if((_frame->node.isNull()/*==QDomNode()*/)||
                               //         (_frame->node.toElement().tagName()=="frames"))

                                //создаём новый узел
                                {
                                    _frame->node=document0.createElement("frame");
                                    //_frame->node.toElement().setAttribute("size", QString("%1, %2").arg().arg());
                                    _frame->node.toElement().setAttribute("tex_rect", QString("%1, %2, %3, %4")
                                                                          .arg(_frame->tex_x).arg(_frame->tex_y).arg(_frame->tex_w).arg(_frame->tex_h));
                                    _frame->node.toElement().setAttribute("texture", _frame->texture);

                                    if(lastAddedNode.isNull())//небыло элементов.. добавляем в начало
                                    {_anim->node.insertBefore(_frame->node, QDomNode());}
                                    else //элементы были юю добвляем после последнего
                                    {_anim->node.insertAfter(_frame->node, lastAddedNode);}
                                    lastAddedNode = _frame->node;
                                }
                            }
                             _LASTframe=_frame;
                            //если входит то ОБНОВЛЯЕМ количество фреймов и продолжаем поиск
                            //если не входит значит это новый фрейм и серия кончилась на предыдущем
                        }
                        else //этот выпадает из серии хоть и был помечен как в серии
                        {
                            //QMessageBox::information(0,0, QString("!он НЕ в серии и по номеру %1").arg(_frame->seriesNum));
                            //нужно закончить серию
                            //завершаем
                            seriesBegin=false;

                            //просто добавляем в фрейм узел

                            //узел добавляем анимацию
                            //помечаем его как НЕ в серии и убираем лейбел серии
                            _frame->series=false;
                            _frame->widget->lbl->setText("");
                            //создаём новый узел
                                _frame->node=document0.createElement("frame");
                                _frame->node.toElement().setAttribute("tex_rect", QString("%1, %2, %3, %4")
                                                                      .arg(_frame->tex_x).arg(_frame->tex_y).arg(_frame->tex_w).arg(_frame->tex_h));
                                _frame->node.toElement().setAttribute("texture", _frame->texture);

                                if(lastAddedNode.isNull())//небыло элементов.. добавляем в начало
                                {_anim->node.insertBefore(_frame->node, QDomNode()); //QMessageBox::information(0,0,"создан свободный добавлен в начало");
                                }
                                else //элементы были юю добвляем после последнего
                                {_anim->node.insertAfter(_frame->node, lastAddedNode); //QMessageBox::information(0,0,"создан свободный добавлен в конец");
                                }

                             lastAddedNode = _frame->node;
                             //if(lastAddedNode.isNull())QMessageBox::information(0,0, "NULL. Node not created =(");
                            _LASTframe=_frame;
                        }
                    }//if(_frame->series)//если серия
                    else
                    {
                     //QMessageBox::information(0,0, QString("фрейм свободен "));
                      seriesBegin=false;
                      _frame->series=false;
                      //создаём новый узел
                      if(_frame->newFrame )
                      {
                          _frame->node=document0.createElement("frame");
                      }

                          _frame->node.toElement().setAttribute("tex_rect", QString("%1, %2, %3, %4")
                                                                .arg(_frame->tex_x).arg(_frame->tex_y).arg(_frame->tex_w).arg(_frame->tex_h));
                          _frame->node.toElement().setAttribute("texture", _frame->texture);

                          if(lastAddedNode.isNull())//небыло элементов.. добавляем в начало
                          {_anim->node.insertBefore(_frame->node, QDomNode());
                              //QMessageBox::information(0,0,"lastAddedNode.isNull!! свободный добавлен в начало");
                          }
                          else //элементы были .. добвляем после последнего
                          {_anim->node.insertAfter(_frame->node, lastAddedNode);
                              //QMessageBox::information(0,0,"свободный добавлен в конец");
                          }
                          lastAddedNode = _frame->node;

                      _LASTframe=_frame;
                    }
                }
		//QMessageBox::information(0,0,"фреймов больше не осталось");

		//помечаем все фреймы и анимы как НЕ новые так как их только что сохранили
                _anim->newAnim=false;
                QListIterator<frame*> frameIteratorN(_anim->frames);
		while(frameIteratorN.hasNext())
		{
			_frame = frameIteratorN.next();
			_frame->newFrame=false;
		}
	}
	emit objectSaved();
}

void animatron::deleteUslessNodes()
{
	QListIterator<QDomNode> domnIt(uslessNodes);
	while(domnIt.hasNext())
	{
		QDomNode dn = domnIt.next();
		dn.parentNode().removeChild(dn);
	}
	uslessNodes.clear();
}

void animatron::slotObjNameChanged( const QString & _text)
{
	drawObject->name=_text;
}



