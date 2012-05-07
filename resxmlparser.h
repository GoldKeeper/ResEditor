#ifndef RESXMLPARSER_H
#define RESXMLPARSER_H

#include <QMap>
#include <QString>
#include <QDomDocument>
#include <QTreeWidgetItem>
#include <QFile>
#include <QHash>
#include <QMap>
//#include <QTranslator>
#include <QObject>

class ResXmlParser: public QObject
{
   // Q_OBJECT

private:
    ResXmlParser(): QObject(0){}
    ResXmlParser(ResXmlParser& root);
    ResXmlParser& operator=(ResXmlParser&);

public:

    //static QTranslator translator;
    //void setTranslator
    enum itemType{TEXTURE = 1001, OBJECT, SPRITE, TILE, OTHER };

    static ResXmlParser & Instance()
    {
            static ResXmlParser theSingleInstance;            
            return theSingleInstance;
    }

    QDomDocument document; //Объект документа //к.о.
    QDomNode rootNode;

    QTreeWidgetItem * rootItem; //корневой элемент // вероятно общий корневой элемент
    QTreeWidgetItem * texturesItem;//root for Textures//элемент корневой для текстур
    QTreeWidgetItem * objectsItem;//root for Objects//элемент корневой для объектов
    QHash<QTreeWidgetItem*, QDomNode> * treeNodes;//pointer to map ResEditor::treeNodes//указатель на связку узла и итема дерева в рес эдиторе
    QMap<QString, QString>allTextures;//map of all textures//мапа для всех текстур - связка название - имя файла

    bool peraseXml (const QString &file, QTreeWidget * pqTreeWidgetee, QHash<QTreeWidgetItem*, QDomNode> * _treeNodes);//setup and begin parse //стартовая точка для парсинга

    void addAllres(); //запуск парсинга
    QTreeWidgetItem * addTexture(QTreeWidgetItem * _parentItem, QDomNode node);//добавление текстуры
    QTreeWidgetItem * addObject(QTreeWidgetItem * _parentItem, QDomNode node);//добавление объекта
};

#endif // RESXMLPARSER_H
