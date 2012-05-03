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

    QDomDocument document; //������ ��������� //�.�.
    QDomNode rootNode;

    QTreeWidgetItem * rootItem; //�������� ������� // �������� ����� �������� �������
    QTreeWidgetItem * texturesItem;//������� �������� ��� �������
    QTreeWidgetItem * objectsItem;//������� �������� ��� ��������
    QHash<QTreeWidgetItem*, QDomNode> * treeNodes;//��������� �� ������ ���� � ����� ������ � ��� �������
    QMap<QString, QString>allTextures;

    bool peraseXml (const QString &file, QTreeWidget * pqTreeWidgetee, QHash<QTreeWidgetItem*, QDomNode> * _treeNodes); //��������� ����� ��� ��������

    void addAllres(); //������ ��������
    QTreeWidgetItem * addTexture(QTreeWidgetItem * _parentItem, QDomNode node);//���������� ��������
    QTreeWidgetItem * addObject(QTreeWidgetItem * _parentItem, QDomNode node);//���������� �������
};

#endif // RESXMLPARSER_H
