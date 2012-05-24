#ifndef TYPES_H
#define TYPES_H

#include <QObject>
#include <QTreeWidget>
#include "ui_widget.h"
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGridLayout>

#include "draw.h"
#include "sprite.h"
#include "frame.h"
#include "anim.h"

class frame;
class anim;

//enum itemType{SPRITES = 1010, ONE_SPRITE=1011, ANIMATIONS = 1012, ANIMATION_SET = 1013, ANIMATION_FRAME = 1014 };
class TreeWidgetItemAnim : public QTreeWidgetItem
{
public:
		enum itemType{SPRITES = 1010, ONE_SPRITE=1011, ANIMATIONS = 1012, ANIMATION_SET = 1013, ANIMATION_FRAME = 1014 };

        TreeWidgetItemAnim ( int _type = Type ):QTreeWidgetItem (_type){nullAll();}
        TreeWidgetItemAnim ( const QStringList & _strings, int _type = Type ):QTreeWidgetItem (_strings, _type ){nullAll();}
        TreeWidgetItemAnim ( QTreeWidget * _parent, int _type = Type ):QTreeWidgetItem (_parent,_type){nullAll();}
        TreeWidgetItemAnim ( QTreeWidget * _parent, const QStringList & _strings, int _type = Type ):QTreeWidgetItem (_parent, _strings, _type ){nullAll();}
        TreeWidgetItemAnim ( QTreeWidget * _parent, QTreeWidgetItem * _preceding, int _type = Type ):QTreeWidgetItem ( _parent, _preceding, _type ){nullAll();}
        TreeWidgetItemAnim ( QTreeWidgetItem * _parent, int _type = Type ):QTreeWidgetItem ( _parent, _type ){nullAll();}
        TreeWidgetItemAnim ( QTreeWidgetItem * _parent, const QStringList & _strings, int _type = Type ):QTreeWidgetItem ( _parent, _strings, _type ){nullAll();}
        TreeWidgetItemAnim ( QTreeWidgetItem * _parent, QTreeWidgetItem * _preceding, int _type = Type ):QTreeWidgetItem ( _parent, _preceding, _type ){nullAll();}
        TreeWidgetItemAnim ( const QTreeWidgetItem & _other ):QTreeWidgetItem ( _other ){nullAll();}
	
	QWidget * widget;
	QScrollArea * scrollArea;
	QGridLayout * gridLayout;
	sprite * oneSprite;
	anim * oneAnim;
	frame * oneFrame;
private:
	void nullAll()
	{
		widget = NULL;
		scrollArea = NULL;
		gridLayout = NULL;
		oneSprite = NULL;
		oneAnim = NULL;
		oneFrame = NULL;
	}
};

#endif
