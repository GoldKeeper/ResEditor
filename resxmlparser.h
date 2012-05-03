#ifndef RESXMLPARSER_H
#define RESXMLPARSER_H

#include <QMap>
#include <QString>
#include <QDomDocument>
#include <QTreeWidgetItem>
#include <QFile>
#include <QHash>
#include <QMap>

class ResXmlParser
{
private:


    ResXmlParser(){}
    ResXmlParser(ResXmlParser& root);
    ResXmlParser& operator=(ResXmlParser&);

public:

    enum itemType{TEXTURE = 1001, OBJECT, SPRITE, TILE, OTHER };

    static ResXmlParser & Instance()
    {
            static ResXmlParser theSingleInstance;
            return theSingleInstance;
    }

    QDomDocument document; //Объект документа //к.о.
    QDomNode rootNode;

    QTreeWidgetItem * rootItem; //корневой элемент // вероятно общий корневой элемент
    QTreeWidgetItem * texturesItem;//элемент корневой для текстур
    QTreeWidgetItem * objectsItem;//элемент корневой для объектов
    QHash<QTreeWidgetItem*, QDomNode> * treeNodes;//указатель на связку узла и итема дерева в рес эдиторе
    QMap<QString, QString>allTextures;

    bool peraseXml (const QString &file, QTreeWidget * pqTreeWidgetee, QHash<QTreeWidgetItem*, QDomNode> * _treeNodes); //стартовая точка для парсинга

    void addAllres(); //запуск парсинга
    QTreeWidgetItem * addTexture(QTreeWidgetItem * _parentItem, QDomNode node);//добавление текстуры
    QTreeWidgetItem * addObject(QTreeWidgetItem * _parentItem, QDomNode node);//добавление объекта
};

#endif // RESXMLPARSER_H
