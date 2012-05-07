#ifndef WIDGETCONTROL_H
#define WIDGETCONTROL_H

#include "ui_widget.h"
#include <QMessageBox>
//#include "animatron/types.h"
#include <QTreeWidgetItem>
#include <QMouseEvent>


class WidgetControl: public QWidget
{
	Q_OBJECT
public:
	WidgetControl( QWidget * parent = 0, Qt::WindowFlags f = 0 ):QWidget (parent,f)
	{
		ui.setupUi(this);
		ui.btn_r-> setIcon(QPixmap(":/ResEditor/Resources/animatron/arrow.png"));
		ui.btn_rt->setIcon(QPixmap(":/ResEditor/Resources/animatron/arrow-045.png"));
		ui.btn_t-> setIcon(QPixmap(":/ResEditor/Resources/animatron/arrow-090.png"));
		ui.btn_lt->setIcon(QPixmap(":/ResEditor/Resources/animatron/arrow-135.png"));
		ui.btn_l-> setIcon(QPixmap(":/ResEditor/Resources/animatron/arrow-180.png"));
		ui.btn_lb->setIcon(QPixmap(":/ResEditor/Resources/animatron/arrow-225.png"));
		ui.btn_b-> setIcon(QPixmap(":/ResEditor/Resources/animatron/arrow-270.png"));
		ui.btn_rb->setIcon(QPixmap(":/ResEditor/Resources/animatron/arrow-315.png"));
		ui.btn_center->setIcon(QPixmap(":/ResEditor/Resources/animatron/hand.png"));
		ui.btn_up->setIcon(QPixmap(":/ResEditor/Resources/animatron/arrow-090.png"));
		ui.btn_down->setIcon(QPixmap(":/ResEditor/Resources/animatron/arrow-270.png"));

		connect(ui.btn_lt, SIGNAL(clicked()), SLOT(moveLT()));
		connect(ui.btn_rt, SIGNAL(clicked()), SLOT(moveRT()));
		connect(ui.btn_lb, SIGNAL(clicked()), SLOT(moveLB()));
		connect(ui.btn_rb, SIGNAL(clicked()), SLOT(moveRB()));
		connect(ui.btn_l, SIGNAL(clicked()), SLOT(moveL()));
		connect(ui.btn_r, SIGNAL(clicked()), SLOT(moveR()));
		connect(ui.btn_b, SIGNAL(clicked()), SLOT(moveB()));
		connect(ui.btn_t, SIGNAL(clicked()), SLOT(moveT()));
		labelNew=NULL;
		lbl=NULL;
	}	

public:
	Ui::widget ui;
	QTreeWidgetItem * treeWidgetItem;
	QLabel *labelNew;
	QLabel *lbl;

	public slots:
	void moveT(){ui.spinBox_y->setValue(ui.spinBox_y->value()-1);}
	void moveL(){ui.spinBox_x->setValue(ui.spinBox_x->value()-1);}
	void moveR(){ui.spinBox_x->setValue(ui.spinBox_x->value()+1);}
	void moveB(){ui.spinBox_y->setValue(ui.spinBox_y->value()+1);}
	void moveLT(){moveL(); moveT();}
	void moveRT(){moveR(); moveT();}
	void moveLB(){moveL(); moveB();}
	void moveRB(){moveR(); moveB();}


	void mouseDoubleClickEvent(QMouseEvent *)
	{emit doubleClicked(treeWidgetItem);}
        void mousePressEvent ( QMouseEvent * e )
        {
            if (e->button()==Qt::LeftButton)
            emit clicked(treeWidgetItem);
        }


signals:
	void doubleClicked(QTreeWidgetItem *);
        void clicked(QTreeWidgetItem *);

};


#endif
