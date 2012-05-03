#ifndef TREEWIDGETITEM_H
#define TREEWIDGETITEM_H

#include <QTreeWidgetItem>
#include <QDomNode>
#include <QMap>
#include "animatron/animatron.h"
//enum itemType{TEXTURE = 1001, OBJECT=1002, SPRITE = 1003 };

class TreeWidgetItem : public QTreeWidgetItem
{
	
//	Q_OBJECT

public:
	enum itemType{TEXTURE = 1001, OBJECT, SPRITE, TILE, OTHER };

        TreeWidgetItem ( int _type = Type ):QTreeWidgetItem (_type){}
        TreeWidgetItem ( const QStringList & _strings, int _type = Type ):QTreeWidgetItem (_strings, _type ){}
        TreeWidgetItem ( QTreeWidget * _parent, int _type = Type ):QTreeWidgetItem (_parent,_type){}
        TreeWidgetItem ( QTreeWidget * _parent, const QStringList & _strings, int _type = Type ):QTreeWidgetItem (_parent, _strings, _type ){}
        TreeWidgetItem ( QTreeWidget * _parent, QTreeWidgetItem * _preceding, int _type = Type ):QTreeWidgetItem ( _parent, _preceding, _type ){}
        TreeWidgetItem ( QTreeWidgetItem * _parent, int _type = Type ):QTreeWidgetItem ( _parent, _type ){}
        TreeWidgetItem ( QTreeWidgetItem * _parent, const QStringList & _strings, int _type = Type ):QTreeWidgetItem ( _parent, _strings, _type ){}
        TreeWidgetItem ( QTreeWidgetItem * _parent, QTreeWidgetItem * _preceding, int _type = Type ):QTreeWidgetItem ( _parent, _preceding, _type ){}
        TreeWidgetItem ( const QTreeWidgetItem & _other ):QTreeWidgetItem ( _other ){}
        virtual ~TreeWidgetItem(){};

	QDomNode node;
	QMap<QString, QString> attrData;
        animatron * animator;
};




#endif // TREEWIDGETITEM_H
